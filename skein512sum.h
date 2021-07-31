#ifndef SKEIN512SUM_H
#define SKEIN512SUM_H

#include <stdlib.h>
#include <stdio.h>
#include <Base/macros.h>
#include <Skc/skein512.h>

#define SKEIN512SUM_MIN_OUTPUT_BITS		CHAR_BIT
#define SKEIN512SUM_MIN_OUTPUT_BYTES		(SKEIN512SUM_MIN_OUTPUT_BITS / CHAR_BIT)
#define SKEIN512SUM_MAX_OUTPUT_BITS		4096
#define SKEIN512SUM_MAX_OUTPUT_BYTES		(SKEIN512SUM_MAX_OUTPUT_BITS / CHAR_BIT)
#define SKEIN512SUM_MAX_LENGTH_ARG_CHARS	4

#if (SKEIN512SUM_MAX_OUTPUT_BITS % CHAR_BIT)
#  error "SKEIN512SUM_MAX_OUTPUT_BITS must be divisible into bytes!"
#endif

enum {
	SKEIN512SUM_MODE_FILE,
	SKEIN512SUM_MODE_STRING
};

typedef struct {
	uint8_t    output_buf [SKEIN512SUM_MAX_OUTPUT_BYTES];
	Skc_UBI512 ubi512;
	char*      input;
	int        mode;
	int        num_output_bits;
} Skein512sum;
#define SKEIN512SUM_DEFAULT_LITERAL (Skein512sum){{0}, (Skc_UBI512){0}, NULL, 0, 512}
#define SKEIN512SUM_NULL_LITERAL    (Skein512sum){0}

#define R_(ptr) ptr BASE_RESTRICT
BASE_BEGIN_DECLS

void print_help (void);
void hash_file (R_(Skein512sum*), R_(const char*), const int);
void hash_str  (R_(Skein512sum*), R_(const char*), const int);

BASE_END_DECLS
#undef R_

#endif /* ~ SKEIN512SUM_H */
