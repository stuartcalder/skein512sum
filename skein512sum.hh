/*
	Copyright (c) 2020 Stuart Steven Calder
	All rights reserved.
	See accompanying LICENSE file for licensing information.
*/
/* General headers */
#include <ssc/general/integers.hh>
#include <ssc/general/macros.hh>
#if 0
#	include <ssc/general/arg_mapping.hh>
#else
#	include <ssc/general/c_argument_map.hh>
#endif
#include <ssc/general/error_conditions.hh>
#include <ssc/general/print.hh>
/* OS-specific file i/o headers */
#include <ssc/files/os_map.hh>
/* Crypto headers */
#if 0
#include <ssc/crypto/threefish.hh>
#include <ssc/crypto/unique_block_iteration.hh>
#include <ssc/crypto/skein.hh>
#endif
#include <ssc/crypto/unique_block_iteration_f.hh>
#include <ssc/crypto/skein_f.hh>
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
#if 0
	using Arg_Map_t = ssc::Arg_Mapping::Arg_Map_t;
#else
#endif
#if 0
	using Threefish_t = ssc::Threefish<512>;
	using UBI_t = ssc::Unique_Block_Iteration<512>;
	using Skein_t = ssc::Skein<512>;
#endif
	using UBI_f = ssc::Unique_Block_Iteration_F<512>;
	using Skein_f = ssc::Skein_F<512>;
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
#if 0
	Skein512Sum (int const argc, char const *argv[]);
#else
	Skein512Sum (ssc::C_Argument_Map &);
#endif
	/* PUBLIC PROCEDURES */
private:
	/* PRIVATE DATA */
	u8_t         output_buffer [Max_Output_Bytes];
	int num_output_bits = 512;
	/* PRIVATE PROCEDURES */
#if 0
	void process_arguments_ (Arg_Map_t&&);
	void hash_file_   (std::string const &input_filename);
	void hash_string_ (std::string const &input_string);
#else
	void process_arguments_ (ssc::C_Argument_Map &);
	void hash_file_   (char const *input_filename, int const input_filename_size);
	void hash_string_ (char const *string, int const string_size);
#endif
};/* ~ class Skein512Sum */
