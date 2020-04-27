#  Copyright (c) 2020 Stuart Steven Calder
#  All rights reserved.
#  See accompanying LICENSE file for licensing information.
###########################################################
# Define project.
project('skein512sum', 'cpp', default_options: ['cpp_std=c++17', 'strip=true', 'buildtype=release',
                                                'optimization=3', 'cpp_eh=none'])
# Get C++ compiler.
cc = meson.get_compiler('cpp')
# Get the host's operating system.
os = host_machine.system()
# Default source files.
source    = ['main.cc', 'skein512sum.cc']
include   = []
lib       = []
depends   = []
cpp_flags = []

# The define flag prefix is different on Windows with MSVC compiler.
if os == 'windows' and cc.get_id() == 'msvc'
	_Define_Flag_Prefix = '/D'
else
	_Define_Flag_Prefix = '-D'
endif
# Handle OSX-specifics.
if   os == 'darwin'
	# On OSX we use memset_s for secure memory zeroing, which requires this macro to be defined.
	_flag = _Define_Flag_Prefix + '__STDC_WANT_LIB_EXT1__=1'
	cpp_flags += _flag
endif
# Handle BSD-specifics.
if   os == 'openbsd' or os == 'freebsd' or os == 'darwin'
	include += '/usr/local/include'
	lib     += '/usr/local/lib'
	depends += cc.find_library('ssc', dirs: lib)
# Handle GNU/Linux-specifics.
elif os == 'linux'
	include += '/usr/include'
	depends += cc.find_library('ssc')
# Handle Windows-specifics.
elif os == 'windows'
	include += 'C:/include'
	lib     += 'C:/lib'
	depends += cc.find_library('bcrypt')
	depends += cc.find_library('ssc', static: true, dirs: lib)
	_flag = _Define_Flag_Prefix + '__IMPORT_STATIC'
	cpp_flags += _flag
else
	assert(false)
endif

if get_option('enable_debugging_symbols')
	cpp_flags += '-g'
endif

if os != 'windows'
	executable('skein512sum', sources: source, dependencies: depends,
		   include_directories: include, install: true,
		   cpp_args: cpp_flags)
else
	executable('skein512sum', sources: source, dependencies: depends,
	           include_directories: include, install: true,
		   cpp_args: cpp_flags, install_dir: 'C:/bin')
endif
