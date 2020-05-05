/*
	Copyright (c) 2020 Stuart Steven Calder
	All rights reserved.
	See accompanying LICENSE file for licensing information.
*/
#include <cstdlib>
#include "skein512sum.hh"
using namespace ssc;

int main (int const argc, char const *argv[])
{

	_OPENBSD_UNVEIL( "/usr", "rx" );

	C_Argument_Map c_arg_map{ argc, argv };
	Skein512Sum{ c_arg_map };
	return EXIT_SUCCESS;
} /* int main (argc,argv) */
