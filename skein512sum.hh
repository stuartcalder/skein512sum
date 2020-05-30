/*
	Copyright (c) 2020 Stuart Steven Calder
	All rights reserved.
	See accompanying LICENSE file for licensing information.
*/
/* General headers */
#include <ssc/general/integers.hh>
#include <ssc/general/macros.hh>
#include <ssc/general/c_argument_map.hh>
#include <ssc/general/error_conditions.hh>
#include <ssc/general/print.hh>
/* OS-specific file i/o headers */
#include <ssc/files/os_map.hh>
/* Crypto headers */
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
	using UBI_f = ssc::Unique_Block_Iteration_F<512>;
	using Skein_f = ssc::Skein_F<512>;
	enum Int_Constants: int {
		Min_Output_Bits = 8,
		Min_Output_Bytes = Min_Output_Bits / CHAR_BIT,
		Max_Output_Bits = 4096,
		Max_Output_Bytes = Max_Output_Bits / CHAR_BIT,
		Max_Length_Arg_Chars = 4
	};
	static constexpr auto &Help_Output = "Usage: skein512sum [-l <output bits>] [-s] <input>\n"
			 		     "Arguments to switches MUST be in seperate words. (i.e. skein512sum -l 1024 file; NOT skein512sum -l1024 file)\n"
		                             "-h, --help           : Print this help output.\n"
					     "-l, --length <number>: Hash output bits (max 4096).\n"
					     "-s, --string <input> : Interpret <input> as a string to be hashed instead of file, which is the default.\n";
/* CONSTRUCTORS */
	Skein512Sum (void) = delete;
	Skein512Sum (ssc::C_Argument_Map &);
/* PUBLIC PROCEDURES */
private:
/* PRIVATE DATA */
	u8_t output_buffer [Max_Output_Bytes];
	int  num_output_bits = 512;
/* PRIVATE PROCEDURES */
	void
	process_arguments_ (ssc::C_Argument_Map &);

	void
	hash_file_ (char const *input_filename, int const input_filename_size);
	
	void
	hash_string_ (char const *string, int const string_size);
};// ~ class Skein512Sum
