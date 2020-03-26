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

_Define_Flag_Prefix = '-D'
# The define flag prefix is different on Windows with MSVC compiler.
if os == 'windows' and cc.get_id() == 'msvc'
	_Define_Flag_Prefix = '/D'
endif

# Handle BSD-specifics.
if   os == 'openbsd' or os == 'freebsd'
	include += '/usr/local/include'
	lib     += '/usr/local/lib'
# Handle GNU/Linux-specifics.
elif os == 'linux'
	include += '/usr/include'
# Handle Windows-specifics.
elif os == 'windows'
	include += 'C:/include'
	lib     += 'C:/lib'
else
	assert(false)
endif

if os == 'openbsd' or os == 'freebsd'
	depends += cc.find_library('ssc', dirs: lib)
elif os == 'linux'
	depends += cc.find_library('ssc')
elif os == 'windows'
	depends += cc.find_library('bcrypt')
	depends += cc.find_library('ssc', static: true, dirs: lib)
	_flag = _Define_Flag_Prefix + '__IMPORT_STATIC'
	cpp_flags += _flag
else
	assert(false)
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