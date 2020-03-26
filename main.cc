/*
	Copyright (c) 2020 Stuart Steven Calder
	All rights reserved.
	See accompanying LICENSE file for licensing information.
*/
#include <cstdlib>
#include "skein512sum.hh"

int main (int const argc, char const *argv[])
{

	_OPENBSD_UNVEIL( "/usr", "rx" );

	Skein512Sum{ argc, argv };
	return EXIT_SUCCESS;
} /* int main (argc,argv) */
