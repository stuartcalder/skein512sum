#include "Skein512sum.h"
#include "CommandLineArg.h"

#include <SSC/Error.h>
#include <SSC/File.h>
#include <SSC/MemMap.h>
#include <SSC/Print.h>

static void
file_mode_(Skein512sum*);

static void
str_mode_(Skein512sum*);

#define ARRLEN_(Array, Type) ((sizeof(Array) / sizeof(Type)) - 1)
static const SSC_ArgLong longs[] = {
  SSC_ARGLONG_LITERAL(help_argproc  , "help"),
  SSC_ARGLONG_LITERAL(length_argproc, "length"),
  SSC_ARGLONG_LITERAL(string_argproc, "string"),
  SSC_ARGLONG_NULL_LITERAL
};
#define N_LONGS_ ARRLEN_(longs, SSC_ArgLong)
static const SSC_ArgShort shorts[] = {
  SSC_ARGSHORT_LITERAL(help_argproc  , 'h'),
  SSC_ARGSHORT_LITERAL(length_argproc, 'l'),
  SSC_ARGSHORT_LITERAL(string_argproc, 's'),
  SSC_ARGSHORT_NULL_LITERAL
};
#define N_SHORTS_ ARRLEN_(shorts, SSC_ArgShort)

int main(int argc, char *argv[])
{
  SSC_OPENBSD_UNVEIL("/usr", "r");
  Skein512sum state = SKEIN512SUM_DEFAULT_LITERAL;
  Skein512sum_init(&state);
  SSC_assert(argc);
  SSC_processCommandLineArgs(argc - 1, argv + 1, N_SHORTS_, shorts, N_LONGS_, longs, &state, file_argproc);
  switch(state.mode) {
    case SKEIN512SUM_MODE_FILE:
      file_mode_(&state);
      break;
    case SKEIN512SUM_MODE_STRING:
      str_mode_(&state);
      break;
    default:
      SSC_errx("Error: Invalid skein512sum mode (%d).\n", state.mode);
  }
  free(state.input);
  return EXIT_SUCCESS;
}

/* Static procedures. */
void file_mode_(Skein512sum* ctx)
{
  if (!ctx->input)
    SSC_errx("Error: No input filename specified.\n");
  SSC_OPENBSD_UNVEIL(ctx->input, "r");
  SSC_OPENBSD_UNVEIL(SSC_NULL, SSC_NULL);
  SSC_MemMap map;
  map.file = SSC_FilePath_openOrDie(ctx->input, true);
  map.size = SSC_File_getSizeOrDie(map.file);
  SSC_MemMap_mapOrDie(&map, true);
  SSC_assertMsg((ctx->num_output_bits <= SKEIN512SUM_MAX_OUTPUT_BITS),
   "Num output bits %d over maximum %d!", ctx->num_output_bits, SKEIN512SUM_MAX_OUTPUT_BITS);
  const int num_output_bytes = ctx->num_output_bits / CHAR_BIT;
  if (num_output_bytes == PPQ_THREEFISH512_BLOCK_BYTES)
    PPQ_Skein512_hashNative(&ctx->ubi512,
     ctx->output_buf,
     map.ptr,
     map.size);
  else
    PPQ_Skein512_hash(&ctx->ubi512,
     ctx->output_buf,
     map.ptr,
     map.size,
     num_output_bytes);
  SSC_MemMap_unmapOrDie(&map);
  SSC_File_closeOrDie(map.file);
  SSC_printBytes(ctx->output_buf, num_output_bytes);
  printf("  %s\n", ctx->input);
}

void str_mode_(Skein512sum* ctx)
{
  if (!ctx->input)
    SSC_errx("Error: No input string specified.\n");
  SSC_OPENBSD_UNVEIL(SSC_NULL, SSC_NULL);
  SSC_assertMsg((ctx->num_output_bits <= SKEIN512SUM_MAX_OUTPUT_BITS),
      "Num output bits %d over maximum %d!", ctx->num_output_bits, SKEIN512SUM_MAX_OUTPUT_BITS);
  const int num_output_bytes = ctx->num_output_bits / CHAR_BIT;
  if (num_output_bytes == PPQ_THREEFISH512_BLOCK_BYTES)
    PPQ_Skein512_hashNative(&ctx->ubi512,
     ctx->output_buf,
     (const uint8_t*)ctx->input,
     strlen(ctx->input));
  else
    PPQ_Skein512_hash(&ctx->ubi512,
     ctx->output_buf,
     (const uint8_t*)ctx->input,
     strlen(ctx->input),
     num_output_bytes);
  SSC_printBytes(ctx->output_buf, num_output_bytes);
  printf( "  \"%s\"\n", ctx->input );
}
