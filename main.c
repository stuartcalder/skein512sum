#include "skein512sum.h"
#include "args.h"
#include <Base/errors.h>
#include <Base/files.h>
#include <Base/mmap.h>
#include <Base/print.h>

static void file_mode_ (Skein512sum*);
static void str_mode_  (Skein512sum*);

#define ARRLEN_(array, type) ((sizeof(array) / sizeof(type)) - 1)
static const Base_Arg_Long longs[] = {
	BASE_ARG_LONG_LITERAL(help_argproc  , "help"),
	BASE_ARG_LONG_LITERAL(length_argproc, "length"),
	BASE_ARG_LONG_LITERAL(string_argproc, "string"),
	BASE_ARG_LONG_NULL_LITERAL
};
#define N_LONGS_ ARRLEN_(longs, Base_Arg_Long)
static const Base_Arg_Short shorts[] = {
	BASE_ARG_SHORT_LITERAL(help_argproc  , 'h'),
	BASE_ARG_SHORT_LITERAL(length_argproc, 'l'),
	BASE_ARG_SHORT_LITERAL(string_argproc, 's'),
	BASE_ARG_SHORT_NULL_LITERAL
};
#define N_SHORTS_ ARRLEN_(shorts, Base_Arg_Short)

int main (int argc, char ** argv) {
	BASE_OPENBSD_UNVEIL("/usr", "r");
	Skein512sum state = SKEIN512SUM_DEFAULT_LITERAL;
	Base_assert(argc);
	Base_process_args(argc - 1, argv + 1, N_SHORTS_, shorts, N_LONGS_, longs, &state, file_argproc);
	switch(state.mode) {
		case SKEIN512SUM_MODE_FILE:
			file_mode_(&state);
			break;
		case SKEIN512SUM_MODE_STRING:
			str_mode_(&state);
			break;
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
	Base_assert_msg((ctx->num_output_bits <= SKEIN512SUM_MAX_OUTPUT_BITS),
			"Num output bits %d over maximum %d!", ctx->num_output_bits, SKEIN512SUM_MAX_OUTPUT_BITS);
	const int num_output_bytes = ctx->num_output_bits / CHAR_BIT;
	if (num_output_bytes == SKC_THREEFISH512_BLOCK_BYTES)
		Skc_Skein512_hash_native(&ctx->ubi512,
					 ctx->output_buf,
					 map.ptr,
					 map.size);
	else
		Skc_Skein512_hash(&ctx->ubi512,
				  ctx->output_buf,
				  map.ptr,
				  map.size,
				  num_output_bytes);
	Base_MMap_unmap_or_die(&map);
	Base_close_file_or_die(map.file);
	Base_print_bytes(ctx->output_buf, num_output_bytes);
	printf("  %s\n", ctx->input);
}
void str_mode_ (Skein512sum* ctx) {
	if (!ctx->input)
		Base_errx("Error: No input string specified.\n");
	BASE_OPENBSD_UNVEIL(NULL, NULL);
	Base_assert_msg((ctx->num_output_bits <= SKEIN512SUM_MAX_OUTPUT_BITS),
			"Num output bits %d over maximum %d!", ctx->num_output_bits, SKEIN512SUM_MAX_OUTPUT_BITS);
	const int num_output_bytes = ctx->num_output_bits / CHAR_BIT;
	if (num_output_bytes == SKC_THREEFISH512_BLOCK_BYTES)
		Skc_Skein512_hash_native(&ctx->ubi512,
					 ctx->output_buf,
					 (const uint8_t*)ctx->input,
					 strlen(ctx->input));
	else
		Skc_Skein512_hash(&ctx->ubi512,
				  ctx->output_buf,
				  (const uint8_t*)ctx->input,
				  strlen(ctx->input),
				  num_output_bytes);
	Base_print_bytes(ctx->output_buf, num_output_bytes);
	printf( "  \"%s\"\n", ctx->input );
}
