#ifndef SKEIN512SUM_ARGS_H
#define SKEIN512SUM_ARGS_H
#include "skein512sum.h"
#include <shim/args.h>

SHIM_BEGIN_DECLS

Shim_Arg_Handler_f *
short_parser (char const *);

Shim_Arg_Handler_f *
long_parser (char const *);

Shim_Arg_Handler_f *
floating_parser (char const *);

Shim_Arg_Parser_f *
arg_processor (char const *, void * SHIM_RESTRICT);

#define PROTOTYPE_HANDLER_(prefix) \
	void \
	prefix##_handler (char **, int const, void * SHIM_RESTRICT)
#define EQUIVALENT_HANDLER_(long_p, short_p) \
	static Shim_Arg_Handler_f * const long_p##_handler = short_p##_handler

PROTOTYPE_HANDLER_ (h);
PROTOTYPE_HANDLER_ (l);
PROTOTYPE_HANDLER_ (s);
EQUIVALENT_HANDLER_ (help, h);
EQUIVALENT_HANDLER_ (length, l);
EQUIVALENT_HANDLER_ (string, s);
PROTOTYPE_HANDLER_ (file);
#undef EQUIVALENT_HANDLER_
#undef PROTOTYPE_HANDLER_

SHIM_END_DECLS

#endif /* ~ SKEIN512SUM_ARGS_H */
