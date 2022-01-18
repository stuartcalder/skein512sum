#include "args.h"
#include <Base/strings.h>
#include <Base/operations.h>

#ifdef SKEIN512SUM_EXTERN_STRICT_ARG_PROCESSING
#	define HANDLE_INVALID_ARGS_(arg) Base_errx("Error: Invalid argument: %s\n", arg)
#else
#	define HANDLE_INVALID_ARGS_(arg) /* Nil */
#endif

#define R_(p)		p BASE_RESTRICT
#define PROC_(name)	int name##_argproc(const int argc, char** BASE_RESTRICT argv, const int off, void* BASE_RESTRICT v)
#define CTX_		((Skein512sum*)v)

PROC_(file) {
	if (!CTX_->input) {
		Base_Arg_Parser bap;
		Base_Arg_Parser_init(&bap, argv[0] + off, argc, argv);
		if (bap.to_read) {
			Base_assert_msg((!CTX_->input), "Error: An input was already specified!\n");
			CTX_->input = (char*)Base_malloc_or_die(bap.size + 1);
			memcpy(CTX_->input, bap.to_read, bap.size + 1);
			CTX_->mode = SKEIN512SUM_MODE_FILE;
		}
		return bap.consumed;
	}
	return 0;
}
PROC_(help) {
	print_help();
	exit(EXIT_SUCCESS);
	return 0;
}
PROC_(length) {
	Base_Arg_Parser bap;
	Base_Arg_Parser_init(&bap, argv[0] + off, argc, argv);
	Base_assert_msg(bap.to_read, BASE_ERR_S_GENERIC("No length specified"));
	char* const tmp = (char*)Base_malloc_or_die(bap.size + 1);
	memcpy(tmp, bap.to_read, bap.size + 1);
	Base_shift_left_digits(tmp, bap.size);
	int const len = atoi(tmp);
	free(tmp);
	Base_assert_msg((!(len % 8)), BASE_ERR_S_GENERIC("Length must be divisible into 8-bit bytes"));
	CTX_->num_output_bits = len;
	return bap.consumed;
}
static char always_null = '\0';
PROC_(string) {
	Base_Arg_Parser bap;
	Base_Arg_Parser_init(&bap, argv[0] + off, argc, argv);
	if (bap.to_read == NULL) {
	  bap.to_read = &always_null;
	  bap.size = 0;
	  bap.consumed = 0;
	}
	//Base_assert_msg(bap.to_read, BASE_ERR_S_GENERIC("No string specified"));
	CTX_->input = (char*)Base_malloc_or_die(bap.size + 1);
	memcpy(CTX_->input, bap.to_read, bap.size + 1);
	CTX_->mode = SKEIN512SUM_MODE_STRING;
	return bap.consumed;
}
