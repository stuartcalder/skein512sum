/*
	Copyright (c) 2020 Stuart Steven Calder
	All rights reserved.
	See accompanying LICENSE file for licensing information.
*/
/* General headers */
#include <ssc/general/integers.hh>
#include <ssc/general/macros.hh>
#include <ssc/general/arg_mapping.hh>
#include <ssc/general/error_conditions.hh>
#include <ssc/general/print.hh>
/* OS-specific file i/o headers */
#include <ssc/files/os_map.hh>
/* Crypto headers */
#include <ssc/crypto/threefish.hh>
#include <ssc/crypto/unique_block_iteration.hh>
#include <ssc/crypto/skein.hh>
/* C library headers */
#include <cstdlib>
#include <cstring>
/* C++ library headers */
#include <string>

using namespace ssc::ints;

class Skein512Sum
{
public:
	static_assert (CHAR_BIT == 8);
	using Arg_Map_t = ssc::Arg_Mapping::Arg_Map_t;
	using Threefish_t = ssc::Threefish<512>;
	using UBI_t = ssc::Unique_Block_Iteration<512>;
	using Skein_t = ssc::Skein<512>;
	_CTIME_CONST(int) Min_Output_Bits = 8;
	_CTIME_CONST(int) Min_Output_Bytes = Min_Output_Bits / CHAR_BIT;
	_CTIME_CONST(int) Max_Output_Bits = 4096;
	_CTIME_CONST(int) Max_Output_Bytes = Max_Output_Bits / CHAR_BIT;
	_CTIME_CONST(int) Max_Length_Arg_Chars = 4;
	_CTIME_CONST(auto&) Help_Output = "Usage: skein512sum [-l <output bits>] [-s] <input>\n"
			 		  "Arguments to switches MUST be in seperate words. (i.e. skein512sum -l 1024 file; NOT skein512sum -l1024 file)\n"
		                          "-h, --help           : Print this help output.\n"
					  "-l, --length <number>: Hash output bits (max 4096).\n"
					  "-s, --string <input> : Interpret <input> as a string to be hashed instead of file, which is the default.\n";
	/* CONSTRUCTORS */
	Skein512Sum (void) = delete;
	Skein512Sum (int const argc, char const *argv[]);
	/* PUBLIC PROCEDURES */
private:
	/* PRIVATE DATA */
	u8_t         output_buffer [Max_Output_Bytes];
	int num_output_bits = 512;
	/* PRIVATE PROCEDURES */
	void process_arguments_ (Arg_Map_t&&);
	void hash_file_   (std::string const &input_filename);
	void hash_string_ (std::string const &input_string);
};/* ~ class Skein512Sum */
