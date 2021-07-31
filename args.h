#ifndef SKEIN512SUM_ARGS_H
#define SKEIN512SUM_ARGS_H
#include "skein512sum.h"
#include <Base/args.h>

#define R_(ptr) ptr BASE_RESTRICT
BASE_BEGIN_DECLS

Base_Arg_Handler_f* short_parser (const char*);

Base_Arg_Handler_f* long_parser (const char*);

Base_Arg_Handler_f* floating_parser (const char*);

Base_Arg_Parser_f*  arg_processor (const char*, void* BASE_RESTRICT);

#define PROTOTYPE_HANDLER_(pfx) \
  void pfx##_handler (char**, const int, void* BASE_RESTRICT)
#define EQUIVALENT_HANDLER_(longP, shortP) \
  static Base_Arg_Handler_f* const longP##_handler = shortP##_handler

PROTOTYPE_HANDLER_(h);
PROTOTYPE_HANDLER_(l);
PROTOTYPE_HANDLER_(s);
EQUIVALENT_HANDLER_(help, h);
EQUIVALENT_HANDLER_(length, l);
EQUIVALENT_HANDLER_(string, s);
PROTOTYPE_HANDLER_(file);
#undef EQUIVALENT_HANDLER_
#undef PROTOTYPE_HANDLER_

BASE_END_DECLS
#undef R_

#endif /* ~ SKEIN512SUM_ARGS_H */
