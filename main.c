#include "skein512sum.h"
#include "args.h"
#include <shim/errors.h>
#include <shim/files.h>
#include <shim/map.h>
#include <shim/print.h>

static void
file_mode_ (Skein512sum *);
static void
str_mode_ (Skein512sum *);

int
main (int argc, char ** argv) {
	SHIM_OPENBSD_UNVEIL ("/usr", "r");
	Skein512sum state = { 0 };
	state.num_output_bits = 512;
	shim_process_args( argc, argv, arg_processor, &state );
	switch( state.mode ) {
		case SKEIN512SUM_MODE_FILE: {
			file_mode_( &state );
		} break;
		case SKEIN512SUM_MODE_STRING: {
			str_mode_( &state );
		} break;
		default:
			shim_errx("Error: Invalid skein512sum mode (%d).\n", state.mode);
	}
	free( state.input );
	return EXIT_SUCCESS;
}

void
file_mode_ (Skein512sum * ctx) {
	if( !ctx->input )
		shim_errx("Error: No input filename specified.\n");
	SHIM_OPENBSD_UNVEIL (ctx->input, "r");
	SHIM_OPENBSD_UNVEIL (NULL, NULL);
	Shim_Map map;
	map.file = shim_enforce_open_filepath( ctx->input, true );
	map.size = shim_enforce_get_file_size( map.file );
	shim_enforce_map_memory( &map, true );
	int const num_output_bytes = ctx->num_output_bits / CHAR_BIT;
	switch( num_output_bytes ) {
		case SYMM_THREEFISH512_BLOCK_BYTES: {
			symm_skein512_hash_native( &ctx->ubi512,
						   ctx->output_buf,
						   map.ptr,
						   map.size );
		} break;
		default: {
			symm_skein512_hash( &ctx->ubi512,
					    ctx->output_buf,
					    map.ptr,
					    map.size,
					    num_output_bytes );

		} break;
	}
	shim_enforce_unmap_memory( &map );
	shim_enforce_close_file( map.file );
	shim_print_byte_buffer( ctx->output_buf, num_output_bytes );
	printf( "  %s\n", ctx->input );
}
void
str_mode_ (Skein512sum * ctx) {
	if( !ctx->input )
		shim_errx("Error: No input string specified.\n");
	SHIM_OPENBSD_UNVEIL (NULL, NULL);
	int const num_output_bytes = ctx->num_output_bits / CHAR_BIT;
	switch( num_output_bytes ) {
		case SYMM_THREEFISH512_BLOCK_BYTES: {
			symm_skein512_hash_native( &ctx->ubi512,
						   ctx->output_buf,
						   (uint8_t const *)ctx->input,
						   strlen( ctx->input ) );
		} break;
		default: {
			symm_skein512_hash( &ctx->ubi512,
					    ctx->output_buf,
					    (uint8_t const *)ctx->input,
					    strlen( ctx->input ),
					    num_output_bytes );
		} break;
	}
	shim_print_byte_buffer( ctx->output_buf, num_output_bytes );
	printf( "  \"%s\"\n", ctx->input );
}
