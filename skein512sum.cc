/*
	Copyright (c) 2020 Stuart Steven Calder
	All rights reserved.
	See accompanying LICENSE file for licensing information.
*/
#include <ssc/general/parse_string.hh>
#include <string>
#include "skein512sum.hh"

void Skein512Sum::process_arguments_ (Arg_Map_t &&args)
{
	for (size_t i = 1; i < args.size(); ++i) {
		if (args[ i ].first == "-h" || args[ i ].first == "--help") {
			std::fputs( Help_Output, stdout );
			std::exit( EXIT_SUCCESS );
		} else if (args[ i ].first == "-l" || args[ i ].first == "--length") {//If a size other than the default 512 bits is specified...
			std::string number = std::move( args[ i ].second );
			if (number.size() > Max_Length_Arg_Chars)
				errx( "Error: The speficied bit length (%zu) is too many characters (max %d).\n",
				      number.size(), Max_Length_Arg_Chars );
			if (ssc::enforce_integer( number )) {
				unsigned int const length = atoi( number.c_str() );
				if (length < Min_Output_Bits)
					errx( "Error: The specified bit length (%d) is less than the minimum required (%d).\n",
					      length, Min_Output_Bits );
				else if (length > Max_Output_Bits)
					errx( "Error: The specified bit length (%d) is more than the maximum allowed (%d).\n",
					      length, Max_Output_Bits );
				else if (length % CHAR_BIT != 0)
					errx( "Error: Illegal bit length (%d). The specified bit length must be divisible into 8-bit bytes.\n",
					      length );
				else
					num_output_bits = length;
			}
		} else if (!args[ i ].second.empty() && input_filename.empty()) {
			input_filename = std::move( args[ i ].second );
		}
	}
}/* ~ void process_arguments_ (Arg_Mapt_t&&) */

Skein512Sum::Skein512Sum (int const argc, char const *argv[])
{
	process_arguments_( ssc::Arg_Mapping{ argc, argv }.consume() );
	if (input_filename.empty())
		errx( "Error: No input file specified.\n\n%s", Help_Output );

	_OPENBSD_UNVEIL( input_filename.c_str(), "r" );
	_OPENBSD_UNVEIL( nullptr, nullptr );

	ssc::OS_Map os_map;
	// Open input file.
	os_map.os_file = ssc::open_existing_os_file( input_filename.c_str(), true );
	// Determine input file size.
	os_map.size = ssc::get_file_size( os_map.os_file );
	// Memory-map the input file.
	ssc::map_file( os_map, true );

	// Setup crypto buffer.
	_CTIME_CONST(int) Crypto_Buffer_Size = []() -> int {
		int size = 0;
		size += Threefish_t::Buffer_Bytes;
		size += UBI_t::Buffer_Bytes;
		size += (size % sizeof(u64_t));
		return size;
	}();
	alignas(sizeof(u64_t)) u8_t crypto_buffer [Crypto_Buffer_Size];
	// Setup crypto objects that reside in the crypto buffer.
	Threefish_t threefish{ reinterpret_cast<u64_t *>(crypto_buffer) };
	UBI_t       ubi{ &threefish, (crypto_buffer + Threefish_t::Buffer_Bytes) };
	Skein_t     skein{ &ubi };

	unsigned int num_output_bytes = num_output_bits / CHAR_BIT;
	skein.hash( output_buffer, os_map.ptr, os_map.size, num_output_bytes );

	ssc::unmap_file( os_map );
	ssc::close_os_file( os_map.os_file );

	ssc::print_integral_buffer<u8_t>( output_buffer, num_output_bytes );
	std::printf( "  %s\n", input_filename.c_str() );
}/* ~ Skein512Sum (int const,char const *[]) */
