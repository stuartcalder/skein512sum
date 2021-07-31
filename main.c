#include "skein512sum.h"
#include "args.h"
#include <Base/errors.h>
#include <Base/files.h>
#include <Base/mmap.h>
#include <Base/print.h>

static void file_mode_ (Skein512sum*);
static void str_mode_  (Skein512sum*);

int main (int argc, char ** argv) {
	BASE_OPENBSD_UNVEIL("/usr", "r");
	Skein512sum state = SKEIN512SUM_DEFAULT_LITERAL;
	Base_process_args(argc, argv, arg_processor, &state);
	switch(state.mode) {
		case SKEIN512SUM_MODE_FILE: {
			file_mode_(&state);
		} break;
		case SKEIN512SUM_MODE_STRING: {
			str_mode_(&state);
		} break;
		default:
			Base_errx("Error: Invalid skein512sum mode (%d).\n", state.mode);
	}
	free(state.input);
	return EXIT_SUCCESS;
}

void file_mode_ (Skein512sum* ctx) {
	if (!ctx->input)
		Base_errx("Error: No input filename specified.\n");
	BASE_OPENBSD_UNVEIL(ctx->input, "r");
	BASE_OPENBSD_UNVEIL(NULL, NULL);
	Base_MMap map;
	map.file = Base_open_filepath_or_die(ctx->input, true);
	map.size = Base_get_file_size_or_die(map.file);
	Base_MMap_map_or_die(&map, true);
	const int num_output_bytes = ctx->num_output_bits / CHAR_BIT;
	switch (num_output_bytes) {
		case SKC_THREEFISH512_BLOCK_BYTES: {
			Skc_Skein512_hash_native(&ctx->ubi512,
						 ctx->output_buf,
						 map.ptr,
						 map.size);
		} break;
		default: {
			Skc_Skein512_hash(&ctx->ubi512,
					  ctx->output_buf,
					  map.ptr,
					  map.size,
					  num_output_bytes);
		} break;
	}
	Base_MMap_unmap_or_die(&map);
	Base_close_file_or_die(map.file);
	Base_print_bytes(ctx->output_buf, num_output_bytes);
	printf("  %s\n", ctx->input);
}
void str_mode_ (Skein512sum* ctx) {
	if (!ctx->input)
		Base_errx("Error: No input string specified.\n");
	BASE_OPENBSD_UNVEIL(NULL, NULL);
	const int num_output_bytes = ctx->num_output_bits / CHAR_BIT;
	switch (num_output_bytes) {
		case SKC_THREEFISH512_BLOCK_BYTES: {
			Skc_Skein512_hash_native(&ctx->ubi512,
						 ctx->output_buf,
						 (const uint8_t*)ctx->input,
						 strlen(ctx->input));
		} break;
		default: {
			Skc_Skein512_hash(&ctx->ubi512,
					  ctx->output_buf,
					  (const uint8_t*)ctx->input,
					  strlen(ctx->input),
					  num_output_bytes);
		} break;
	}
	Base_print_bytes(ctx->output_buf, num_output_bytes);
	printf( "  \"%s\"\n", ctx->input );
}
