#ifndef SKEIN512SUM_COMMANDLINEARG_H
#define SKEIN512SUM_COMMANDLINEARG_H
#include "Skein512sum.h"
#include <SSC/CommandLineArg.h>

#define R_ SSC_RESTRICT
SSC_BEGIN_C_DECLS

int
file_argproc(
 const int argc,
 char** R_ argv,
 const int off,
 void* R_  v
);

int
help_argproc(
 const int argc,
 char** R_ argv,
 const int off,
 void* R_  v
);

int
length_argproc(
 const int argc,
 char** R_ argv,
 const int off,
 void* R_  v
);

int
string_argproc(
 const int argc,
 char** R_ argv,
 const int off,
 void* R_  v
);

SSC_END_C_DECLS
#undef R_
#endif
