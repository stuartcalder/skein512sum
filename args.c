#include "args.h"
#include <Base/strings.h>
#include <Base/operations.h>

#ifdef SKEIN512SUM_EXTERN_STRICT_ARG_PROCESSING
#	define HANDLE_INVALID_ARGS_(arg) Base_errx("Error: Invalid argument: %s\n", arg)
#else
#	define HANDLE_INVALID_ARGS_(arg) /* Nil */
#endif

#define R_(p)            p BASE_RESTRICT
#define STR_EQ_(s0, s1)  (!strcmp(s0, s1))
#define STR_TO_F_(s, fn) if (STR_EQ_(str, s)) return fn

Base_Arg_Handler_f* short_parser (const char* str) {
	const size_t str_size = strlen(str);
	switch (str_size) {
		case 2:
			switch (str[1]) {
				case 'h': return h_handler;
				case 'l': return l_handler;
				case 's': return s_handler;
			}
	}
	HANDLE_INVALID_ARGS_(str);
	return NULL;
}

Base_Arg_Handler_f* long_parser (const char* str) {
	const size_t str_size = strlen(str);
	switch (str_size) {
		case 6:
			STR_TO_F_("--help", help_handler);
			break;
		case 8:
			STR_TO_F_("--length", length_handler);
			STR_TO_F_("--string", string_handler);
			break;
	}
	HANDLE_INVALID_ARGS_(str);
	return NULL;
}

Base_Arg_Handler_f* floating_parser (const char* str) {
	return file_handler;
}

Base_Arg_Parser_f* arg_processor (const char* str, R_(void*) v_ctx) {
	const int type = Base_argtype(str);
	switch (type) {
		case BASE_ARGTYPE_SHORT: return short_parser;
		case BASE_ARGTYPE_LONG:  return long_parser;
	}
	return floating_parser;
}

#define HANDLER_(pfx) \
  void pfx##_handler (char** str_arr, const int count, void* BASE_RESTRICT v_ctx)
#define CTX_ ((Skein512sum *)v_ctx)

HANDLER_(h) {
	print_help();
	exit(EXIT_SUCCESS);
}
static void parse_input_ (R_(char*) input, const size_t buf_size, Skein512sum* ctx) {
	Base_assert_msg(!ctx->input, "Error: An input was already specified!\n");
	ctx->input = (char*)Base_malloc_or_die(buf_size);
	memcpy(ctx->input, input, buf_size);
}
HANDLER_(l) {
	if (count >= 2) {
		const char* const length_str = str_arr[1];
		if (length_str) {
			const size_t buf_size = strlen(length_str) + 1;
			char* temp = (char*)Base_malloc_or_die(buf_size);
			memcpy(temp, length_str, buf_size);
			Base_shift_left_digits(temp, (buf_size - 1));
			int const length = atoi(temp);
			free(temp);
			Base_assert_msg(!(length % 8), "Error: Length must be divisible into 8-bit bytes.\n");
			CTX_->num_output_bits = length;
			str_arr[1] = NULL;
		}
	}
}
HANDLER_(s) {
	if (!CTX_->input) {
		if (count >= 2) {
			char* const string_str = str_arr[1];
			if (string_str) {
				size_t const buf_size = strlen(string_str) + 1;
				parse_input_(string_str, buf_size, CTX_);
				CTX_->mode = SKEIN512SUM_MODE_STRING;
				str_arr[1] = NULL;
			}
		}
	}
}
HANDLER_(file) {
	if (!CTX_->input) {
		if (count >= 1) {
			char* const filename = str_arr[0];
			if (filename) {
				const size_t buf_size = strlen(filename) + 1;
				parse_input_(filename, buf_size, CTX_);
				CTX_->mode = SKEIN512SUM_MODE_FILE;
				str_arr[0] = NULL;
			}
		}
	}
}
