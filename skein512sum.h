#ifndef SKEIN512SUM_H
#define SKEIN512SUM_H

#include <stdlib.h>
#include <stdio.h>
#include <shim/macros.h>
#include <symm/skein512.h>

#define SKEIN512SUM_MIN_OUTPUT_BITS		CHAR_BIT
#define SKEIN512SUM_MIN_OUTPUT_BYTES		(SKEIN512SUM_MIN_OUTPUT_BITS / CHAR_BIT)
#define SKEIN512SUM_MAX_OUTPUT_BITS		4096
#define SKEIN512SUM_MAX_OUTPUT_BYTES		(SKEIN512SUM_MAX_OUTPUT_BITS / CHAR_BIT)
#define SKEIN512SUM_MAX_LENGTH_ARG_CHARS	4

enum {
	SKEIN512SUM_MODE_FILE,
	SKEIN512SUM_MODE_STRING
};

typedef struct {
	uint8_t     output_buf [SKEIN512SUM_MAX_OUTPUT_BYTES];
	Symm_UBI512 ubi512;
	char *      input;
	int         mode;
	int         num_output_bits;
} Skein512sum;
#define SKEIN512SUM_NULL_INIT { \
	.output_buf = { 0 }, \
	.ubi512 = SYMM_UBI512_NULL_INIT, \
	.input = NULL, \
	.mode = 0, \
	.num_output_bits = 0 \
}

SHIM_BEGIN_DECLS

void
print_help ();
void
hash_file (Skein512sum * SHIM_RESTRICT,
	   char const * SHIM_RESTRICT, int const);
void
hash_str (Skein512sum * SHIM_RESTRICT,
	  char const * SHIM_RESTRICT, int const);

SHIM_END_DECLS

#endif /* ~ SKEIN512SUM_H */
