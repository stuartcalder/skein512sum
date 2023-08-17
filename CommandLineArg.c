#include "CommandLineArg.h"
#include <SSC/String.h>
#include <SSC/Operation.h>

#define R_ SSC_RESTRICT

#ifdef SKEIN512SUM_EXTERN_STRICT_ARG_PROCESSING
 #define HANDLE_INVALID_ARGS_(Arg) SSC_errx("Error: Invalid argument: %s\n", Arg)
#else
 #define HANDLE_INVALID_ARGS_(Arg_) /* Nil */
#endif

int file_argproc(
 const int  argc,
 char** R_  argv,
 const int  off,
 void* R_   v)
{
  Skein512sum* sk = (Skein512sum*)v;
  if (!sk->input) {
    SSC_ArgParser ap;
    SSC_ArgParser_init(&ap, argv[0] + off, argc, argv);
    if (ap.to_read) {
      SSC_assertMsg(!sk->input, "Error: An input was already specified!\n");
      sk->input = (char*)SSC_mallocOrDie(ap.size + 1);
      memcpy(sk->input, ap.to_read, ap.size + 1);
      sk->mode = SKEIN512SUM_MODE_FILE;
    }
    return ap.consumed;
  }
  return 0;
}

int help_argproc(
 const int argc,
 char** R_ argv,
 const int off,
 void* R_  v)
{
  print_help();
  exit(EXIT_SUCCESS);
  return 0; /* Never actually returns, but stops warnings. */
}

int length_argproc(
 const int argc,
 char** R_ argv,
 const int off,
 void* R_  v)
{
  Skein512sum*  sk;
  SSC_ArgParser ap;
  char* tmp;
  int   len;

  sk = (Skein512sum*)v;
  SSC_ArgParser_init(&ap, argv[0] + off, argc, argv);
  SSC_assertMsg(ap.to_read, SSC_ERR_S_GENERIC("No length specified!"));
  tmp = (char*)SSC_mallocOrDie(ap.size + 1);
  memcpy(tmp, ap.to_read, ap.size + 1);
  SSC_Cstr_shiftDigitsToFront(tmp, ap.size);
  len = atoi(tmp);
  free(tmp);
  SSC_assertMsg(!(len % 8), SSC_ERR_S_GENERIC("Length must be divisible into 8-bit bytes!"));
  sk->num_output_bits = len;
  return ap.consumed;
}

int string_argproc(
 const int argc,
 char** R_ argv,
 const int off,
 void* R_  v)
{
  Skein512sum*  sk;
  SSC_ArgParser ap;

  sk = (Skein512sum*)v;
  SSC_ArgParser_init(&ap, argv[0] + off, argc, argv);
  if (!ap.to_read) {
    ap.to_read = "";
    ap.size = 0;
    ap.consumed = 0;
  }
  SSC_assertMsg(!sk->input, SSC_ERR_S_GENERIC("Input already specified!"));
  sk->input = (char*)SSC_mallocOrDie(ap.size + 1);
  memcpy(sk->input, ap.to_read, ap.size + 1);
  sk->mode = SKEIN512SUM_MODE_STRING;
  return ap.consumed;
}
