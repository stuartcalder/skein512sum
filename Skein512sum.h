#ifndef SKEIN512SUM_H
#define SKEIN512SUM_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <SSC/Macro.h>
#include <TSC/Skein512.h>
#define R_ SSC_RESTRICT
SSC_BEGIN_C_DECLS

#define SKEIN512SUM_MIN_OUTPUT_BITS      CHAR_BIT
#define SKEIN512SUM_MIN_OUTPUT_BYTES     1
#define SKEIN512SUM_MAX_OUTPUT_BITS      4096
#define SKEIN512SUM_MAX_OUTPUT_BYTES     (SKEIN512SUM_MAX_OUTPUT_BITS / CHAR_BIT)
#define SKEIN512SUM_MAX_LENGTH_ARG_CHARS 4

#if (SKEIN512SUM_MAX_OUTPUT_BITS % CHAR_BIT)
 #error "SKEIN512SUM_MAX_OUTPUT_BITS must be divisible into bytes!"
#endif

typedef enum {
  SKEIN512SUM_MODE_FILE,
  SKEIN512SUM_MODE_STRING
} Skein512sum_Mode_t;

typedef struct {
  uint8_t            output_buf [SKEIN512SUM_MAX_OUTPUT_BYTES];
  TSC_UBI512         ubi512;
  char*              input;
  Skein512sum_Mode_t mode;
  int                num_output_bits;
} Skein512sum;
#define SKEIN512SUM_NULL_LITERAL \
 SSC_STRUCT_LITERAL( \
  Skein512sum, \
  {0}, \
  TSC_UBI512_NULL_LITERAL, \
  SSC_NULL, \
  SKEIN512SUM_MODE_FILE, \
  0 \
 )
#define SKEIN512SUM_DEFAULT_LITERAL \
 SSC_STRUCT_LITERAL( \
  Skein512sum, \
  {0}, \
  TSC_UBI512_NULL_LITERAL, \
  SSC_NULL, \
  SKEIN512SUM_MODE_FILE, \
  512 \
 )

SSC_INLINE void
Skein512sum_init(Skein512sum* ctx)
{
  //TODO: DELETEME.
}


void print_help(void);

void hash_file(Skein512sum* R_, const char* R_, const int);

void hash_str(Skein512sum* R_, const char* R_, const int);

SSC_END_C_DECLS
#undef R_

#endif /* ~ SKEIN512SUM_H */
