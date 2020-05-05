/*
	Copyright (c) 2020 Stuart Steven Calder
	All rights reserved.
	See accompanying LICENSE file for licensing information.
*/
#include <ssc/general/parse_string.hh>
#include <string>
#include "skein512sum.hh"
using namespace ssc;

#if 0
void Skein512Sum::process_arguments_ (Arg_Map_t &&args)
#else
void Skein512Sum::process_arguments_ (C_Argument_Map &c_arg_map)
#endif
{
	enum class Branch_E {
		None,
		String,
		File
	} branch = Branch_E::None;
#if 0
	std::string str_arg;
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
				int const length = atoi( number.c_str() );
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
		} else if (args[ i ].first == "-s" || args[ i ].first == "--string") {
#if 0
			hash_string_( args[ i ].second );
#endif
			if (branch == Branch_E::None) {
				branch = Branch_E::String;
				str_arg = args[ i ].second;
			}
		} else if (!args[ i ].second.empty()) {// Then it must be a filename.
#if 0
			hash_file_( args[ i ].second );
#endif
			if (branch == Branch_E::None) {
				branch = Branch_E::File;
				str_arg = args[ i ].second;
			}
		}
	}
#else
	int const count = c_arg_map.count;
	char const *str_arg;
	int str_arg_size;
	for( int i = 0; i < count; ++i ) {
		if( c_arg_map.c_strings[ i ] ) {
			if( c_arg_map.argument_cmp( i, "-h"    , (sizeof("-h")     - 1) ) ||
			    c_arg_map.argument_cmp( i, "--help", (sizeof("--help") - 1) ) )
			{
				std::fputs( Help_Output, stdout );
				std::exit( EXIT_SUCCESS );
			} else
			if( c_arg_map.argument_cmp( i, "-l"      , (sizeof("-l")       - 1) ) ||
			    c_arg_map.argument_cmp( i, "--length", (sizeof("--length") - 1) ) )
			{
				c_arg_map.c_strings[ i ] = nullptr;
				if( c_arg_map.next_string_is_valid( i ) ) {
					++i;
					if( c_arg_map.sizes[ i ] > Max_Length_Arg_Chars )
						errx( "Error: The speficied bit length (%zu) is too many characters (max %d).\n",
#if 0
						      number.size(),
#else
						      str_arg_size,
#endif
						      Max_Length_Arg_Chars );
					{
						char *temp = static_cast<char*>(std::malloc( c_arg_map.max_string_size + 1 ));
						size_t const size = c_arg_map.sizes[ i ];
						if( temp == nullptr )
							errx( Generic_Error::Alloc_Failure );
						std::memcpy( temp, c_arg_map.c_strings[ i ], size + 1 );
						int const num_digits = shift_left_digits( temp, size );
						int const number = std::atoi( temp );
						if( number < Min_Output_Bits )
							errx( "Error: The specified bit length (%d) is less than the minimum required (%d).\n",
							      number, Min_Output_Bits );
						else if( number > Max_Output_Bits)
							errx( "Error: The specified bit length (%d) is more than the maximum allowed (%d)\n",
							      number, Max_Output_Bits );
						else if( num_digits % CHAR_BIT != 0 )
							errx( "Error: The specified bit length (%d) is not divisible into 8-bit bytes.\n",
							      number );
						num_output_bits = number;
						std::free( temp );
					}
					c_arg_map.c_strings[ i ] = nullptr;
				}
			} else
			if( c_arg_map.argument_cmp( i, "-s"      , (sizeof("-s")       - 1) ) ||
			    c_arg_map.argument_cmp( i, "--string", (sizeof("--string") - 1) ) )
			{
				c_arg_map.c_strings[ i ] = nullptr;
				if( c_arg_map.next_string_is_valid( i ) ) {
					++i;
					if( branch == Branch_E::None ) {
						branch = Branch_E::String;
						str_arg = c_arg_map.c_strings[ i ];
						str_arg_size = static_cast<int>(c_arg_map.sizes[ i ]);
					}
					c_arg_map.c_strings[ i ] = nullptr;
				}
			} else
			if( c_arg_map.c_strings[ i ] != nullptr )
			{
				if( branch == Branch_E::None ) {
					branch = Branch_E::File;
					str_arg = c_arg_map.c_strings[ i ];
					str_arg_size = static_cast<int>(c_arg_map.sizes[ i ]);
				}
				c_arg_map.c_strings[ i ] = nullptr;
			}
		}/*if( c_arg_map.c_strings[ i ] )*/
	}/*for( int i = 0; i < count; ++i )*/
#endif
	switch (branch) {
	default:
		errx( "Error: Invalid Branch_E\n" );
		break;
	case (Branch_E::String):
		hash_string_( str_arg, str_arg_size );
		break;
	case (Branch_E::File):
		hash_file_( str_arg, str_arg_size );
		break;
	}
}/* ~ void process_arguments_ (Arg_Mapt_t&&) */

#if 0
Skein512Sum::Skein512Sum (int const argc, char const *argv[])
{
	process_arguments_( ssc::Arg_Mapping{ argc, argv }.consume() );
}/* ~ Skein512Sum (int const,char const *[]) */
#else
Skein512Sum::Skein512Sum (C_Argument_Map &c_arg_map)
{
	process_arguments_( c_arg_map );
}
#endif

#if 0
void Skein512Sum::hash_file_ (std::string const &input_filename)
#else
void Skein512Sum::hash_file_ (char const *str_arg, int const str_arg_size)
#endif
{
#if 0
	if (input_filename.empty())
		errx( "Error: No input file specified.\n\n%s", Help_Output );
#else
	if( str_arg_size == 0 )
		errx( "Error: No input file specified.\n\n%s", Help_Output );
#endif

#if 0
	_OPENBSD_UNVEIL (input_filename.c_str(),"r");
#else
	_OPENBSD_UNVEIL (str_arg,"r");
#endif
	_OPENBSD_UNVEIL (nullptr,nullptr);

	ssc::OS_Map os_map;
	// Open input file.
#if 0
	os_map.os_file = ssc::open_existing_os_file( input_filename.c_str(), true );
#else
	os_map.os_file = ssc::open_existing_os_file( str_arg, true );
#endif
	// Determine input file size.
	os_map.size = ssc::get_file_size( os_map.os_file );
	// Memory-map the input file.
	ssc::map_file( os_map, true );

#if 0
	// Setup crypto buffer.
	_CTIME_CONST(int) Crypto_Buffer_Size = Threefish_t::Buffer_Bytes + UBI_t::Buffer_Bytes;

	alignas(sizeof(u64_t)) u8_t crypto_buffer [Crypto_Buffer_Size];
	// Setup crypto objects that reside in the crypto buffer.
	Threefish_t threefish{ reinterpret_cast<u64_t *>(crypto_buffer) };
	UBI_t       ubi{ &threefish, (crypto_buffer + Threefish_t::Buffer_Bytes) };
	Skein_t     skein{ &ubi };
#endif
	typename UBI_f::Data ubi_data;

	int num_output_bytes = num_output_bits / CHAR_BIT;
	if (Skein_f::State_Bytes == num_output_bytes) {
		Skein_f::hash_native( &ubi_data,
				      output_buffer,
				      os_map.ptr,
				      os_map.size );
	} else {
		Skein_f::hash( &ubi_data,
				output_buffer,
				os_map.ptr,
				os_map.size,
				num_output_bytes );
	}

	ssc::unmap_file( os_map );
	ssc::close_os_file( os_map.os_file );

	ssc::print_integral_buffer<u8_t>( output_buffer, num_output_bytes );
#if 0
	std::printf( "  %s\n", input_filename.c_str() );
#else
	std::printf( "  %s\n", str_arg );
#endif
}

#if 0
void Skein512Sum::hash_string_ (std::string const &input_string)
#else
void Skein512Sum::hash_string_ (char const *str_arg, int const str_arg_size)
#endif
{
	_OPENBSD_UNVEIL (nullptr,nullptr);

#if 0
	_CTIME_CONST(int) Crypto_Buffer_Size = Threefish_t::Buffer_Bytes + UBI_t::Buffer_Bytes;

	alignas(sizeof(u64_t)) u8_t crypto_buffer [Crypto_Buffer_Size];
	Threefish_t threefish{ reinterpret_cast<u64_t *>(crypto_buffer) };
	UBI_t       ubi{ &threefish, (crypto_buffer + Threefish_t::Buffer_Bytes) };
	Skein_t     skein{ &ubi };
#endif
	typename UBI_f::Data ubi_data;

	int num_output_bytes = num_output_bits / CHAR_BIT;
	if (Skein_f::State_Bytes == num_output_bytes) {
		Skein_f::hash_native( &ubi_data,
				      output_buffer,
#if 0
				      reinterpret_cast<u8_t const*>(input_string.c_str()),
				      input_string.size() );
#else
				      reinterpret_cast<u8_t const*>(str_arg),
				      str_arg_size );
#endif
	} else {
		Skein_f::hash( &ubi_data,
			       output_buffer,
#if 0
			       reinterpret_cast<u8_t const*>(input_string.c_str()),
			       input_string.size(),
#else
			       reinterpret_cast<u8_t const*>(str_arg),
			       str_arg_size,
#endif
			       num_output_bytes );
	}

	ssc::print_integral_buffer<u8_t>( output_buffer, num_output_bytes );
#if 0
	std::printf( "  \"%s\"\n", input_string.c_str() );
#else
	std::printf( "  \"%s\"\n", str_arg );
#endif
}
