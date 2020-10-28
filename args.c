#include "args.h"
#include <shim/strings.h>
#include <shim/operations.h>

Shim_Arg_Handler_t *
short_parser (char const * str) {
	size_t const str_size = strlen( str );
	switch( str_size ) {
		case 2:
			switch( str[ 1 ] ) {
				case 'h':
					return h_handler;
				case 'l':
					return l_handler;
				case 's':
					return s_handler;
			}
	}
	return NULL;
}

Shim_Arg_Handler_t *
long_parser (char const * str) {
	size_t const str_size = strlen( str );
	switch( str_size ) {
		case 6: {
			if( strcmp( str, "--help" ) == 0 )
				return help_handler;
		} break;
		case 8: {
			if( strcmp( str, "--length" ) == 0 )
				return length_handler;
			if( strcmp( str, "--string" ) == 0 )
				return string_handler;
		} break;
	}
	return NULL;
}

Shim_Arg_Handler_t *
floating_parser (char const * str) {
	return file_handler;
}

Shim_Arg_Parser_t *
arg_processor (char const * str, void * SHIM_RESTRICT v_ctx) {
	int type = shim_argtype( str );
	switch( type ) {
		case SHIM_ARGTYPE_SHORT:
			return short_parser;
		case SHIM_ARGTYPE_LONG:
			return long_parser;
	}
	return floating_parser;
}

#define DEFINE_HANDLER_(prefix) \
	void \
	prefix##_handler (char ** str_arr, int const count, void * SHIM_RESTRICT v_ctx)

DEFINE_HANDLER_ (h) {
	print_help();
	exit( EXIT_SUCCESS );
}
static void
parse_input_ (char * SHIM_RESTRICT input, size_t const buf_size, Skein512sum * ctx) {
	if( ctx->input )
		SHIM_ERRX ("Error: An input was already specified.\n");
	ctx->input = (char *)shim_checked_malloc( buf_size );
	memcpy( ctx->input, input, buf_size );
}
DEFINE_HANDLER_ (l) {
	Skein512sum * ctx = (Skein512sum *)v_ctx;
	if( count >= 2 ) {
		char const * length_str = str_arr[ 1 ];
		if( length_str ) {
			size_t const buf_size = strlen( length_str ) + 1;
			char * temp = (char *)shim_checked_malloc( buf_size );
			memcpy( temp, length_str, buf_size );
			(void)shim_shift_left_digits( temp, (buf_size - 1) );
			int const length = atoi( temp );
			free( temp );
			if( length % 8 != 0 )
				SHIM_ERRX ("Error: Length must be divisible into 8-bit bytes.\n");
			ctx->num_output_bits = length;
			str_arr[ 1 ] = NULL;

		}
	}
}
DEFINE_HANDLER_ (s) {
	Skein512sum * ctx = (Skein512sum *)v_ctx;
	if( !ctx->input ) {
		if( count >= 2 ) {
			char * string_str = str_arr[ 1 ];
			if( string_str ) {
				size_t const buf_size = strlen( string_str ) + 1;
				parse_input_( string_str, buf_size, ctx );
				ctx->mode = SKEIN512SUM_MODE_STRING;
				str_arr[ 1 ] = NULL;
			}
		}
	}
}
DEFINE_HANDLER_ (file) {
	Skein512sum * ctx = (Skein512sum *)v_ctx;
	if( !ctx->input ) {
		if( count >= 1 ) {
			char * filename = str_arr[ 0 ];
			if( filename ) {
				size_t const buf_size = strlen( filename ) + 1;
				parse_input_( filename, buf_size, ctx );
				ctx->mode = SKEIN512SUM_MODE_FILE;
				str_arr[ 0 ] = NULL;
			}
		}
	}
}



