#include "skein512sum.h"

#define STRING_IMPL_(text) 	#text
#define STRING_(text)		STRING_IMPL_ (text)

void
print_help () {
	puts( 
		"Usage: skein512sum [-l <output bits>] [-s] <input>\n"
                "Arguments to switches MUST be in seperate words. (i.e. skein512sum -l 1024 file; NOT skein512sum -l1024 file)\n"
                "-h, --help           : Print this help output.\n"
		"-l, --length <number>: Hash output bits (max " STRING_ (SKEIN512SUM_MAX_OUTPUT_BITS) ").\n"
                "-s, --string <input> : Interpret <input> as a string to be hashed instead of file, which is the default."
	);
}

