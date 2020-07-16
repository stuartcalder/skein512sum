/*
	Copyright (c) 2020 Stuart Steven Calder
	All rights reserved.
	See accompanying LICENSE file for licensing information.
*/
#include <ssc/general/parse_string.hh>
#include <string>
#include "skein512sum.hh"
using namespace ssc;

void Skein512Sum::process_arguments_ (C_Argument_Map &c_arg_map)
{
	enum class Branch_E {
		None,
		String,
		File
	} branch = Branch_E::None;
	int const count = c_arg_map.count;
	char const *str_arg = nullptr;
	int str_arg_size = 0;
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
						SHIM_ERRX ("Error: The speficied bit length (%d) is too many characters (max %d).\n",
						      	   str_arg_size, Max_Length_Arg_Chars);
					{
						char *temp = static_cast<char*>(std::malloc( c_arg_map.max_string_size + 1 ));
						size_t const size = c_arg_map.sizes[ i ];
						if( temp == nullptr )
							SHIM_ERRX (Generic_Error::Alloc_Failure);
						std::memcpy( temp, c_arg_map.c_strings[ i ], size + 1 );
						int const num_digits = shift_left_digits( temp, size );
						int const number = std::atoi( temp );
						if( number < Min_Output_Bits )
							SHIM_ERRX ("Error: The specified bit length (%d) is less than the minimum required (%d).\n",
							      	   number, Min_Output_Bits);
						else if( number > Max_Output_Bits)
							SHIM_ERRX ("Error: The specified bit length (%d) is more than the maximum allowed (%d)\n",
							      	   number, Max_Output_Bits);
						else if( number % CHAR_BIT != 0 )
							SHIM_ERRX ("Error: The specified bit length (%d) is not divisible into 8-bit bytes.\n",
							      	   number);
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
	switch (branch) {
	default:
		SHIM_ERRX ("Error: Invalid Branch_E\n");
		break;
	case (Branch_E::String):
		if( str_arg != nullptr )
			hash_string_( str_arg, str_arg_size );
		else
			SHIM_ERRX ("");
		break;
	case (Branch_E::File):
		if( str_arg != nullptr )
			hash_file_( str_arg, str_arg_size );
		break;
	}
}/* ~ void process_arguments_ (Arg_Mapt_t&&) */

Skein512Sum::Skein512Sum (C_Argument_Map &c_arg_map)
{
	process_arguments_( c_arg_map );
}

void Skein512Sum::hash_file_ (char const * SHIM_RESTRICT str_arg, int const str_arg_size)
{
	if( str_arg_size == 0 )
		SHIM_ERRX ("Error: No input file specified.\n\n%s", Help_Output);

	SHIM_OPENBSD_UNVEIL (str_arg, "r");
	SHIM_OPENBSD_UNVEIL (nullptr, nullptr);

	Shim_Map shim_map;
	// Open input file.
	shim_map.shim_file = shim_open_existing_filepath( str_arg, true );
	// Determine input file size.
	shim_map.size = shim_file_size( shim_map.shim_file );
	// Memory-map the input file.
	shim_map_memory( &shim_map, true );

	typename UBI_f::Data ubi_data;

	int num_output_bytes = num_output_bits / CHAR_BIT;
	if (Skein_f::State_Bytes == num_output_bytes) {
		Skein_f::hash_native( &ubi_data,
				      output_buffer,
				      shim_map.ptr,
				      shim_map.size );
	} else {
		Skein_f::hash( &ubi_data,
				output_buffer,
				shim_map.ptr,
				shim_map.size,
				num_output_bytes );
	}

	shim_unmap_memory( &shim_map );
	shim_close_file( shim_map.shim_file );

	shim_print_byte_buffer( output_buffer, num_output_bytes );
	std::printf( "  %s\n", str_arg );
}

void Skein512Sum::hash_string_ (char const * SHIM_RESTRICT str_arg, int const str_arg_size)
{
	SHIM_OPENBSD_UNVEIL (nullptr,nullptr);

	typename UBI_f::Data ubi_data;

	int num_output_bytes = num_output_bits / CHAR_BIT;
	if (Skein_f::State_Bytes == num_output_bytes) {
		Skein_f::hash_native( &ubi_data,
				      output_buffer,
				      reinterpret_cast<uint8_t const*>(str_arg),
				      str_arg_size );
	} else {
		Skein_f::hash( &ubi_data,
			       output_buffer,
			       reinterpret_cast<uint8_t const*>(str_arg),
			       str_arg_size,
			       num_output_bytes );
	}

	shim_print_byte_buffer( output_buffer, num_output_bytes );
	std::printf( "  \"%s\"\n", str_arg );
}
