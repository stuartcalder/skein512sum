#ifndef SKEIN512SUM_ARGS_H
#define SKEIN512SUM_ARGS_H
#include "skein512sum.h"
#include <Base/args.h>

#define R_(ptr) ptr BASE_RESTRICT
#define PROC_(name) int name##_argproc(const int, char** BASE_RESTRICT, const int, void* BASE_RESTRICT)
BASE_BEGIN_DECLS
PROC_(file);
PROC_(help);
PROC_(length);
PROC_(string);
BASE_END_DECLS
#undef PROC_
#undef R_

#endif /* ~ SKEIN512SUM_ARGS_H */
