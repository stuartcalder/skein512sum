project('skein512sum', 'c', default_options: ['c_std=c18', 'optimization=3', 'buildtype=release', 'strip=true'])

_PROJECT = 'skein512sum'
_LANG = 'c'

_UNIXLIKE_OPERATING_SYSTEMS  = ['openbsd', 'freebsd', 'netbsd', 'darwin', 'linux']
_SUPPORTED_OPERATING_SYSTEMS = _UNIXLIKE_OPERATING_SYSTEMS + ['windows']
_GCC_COMPATIBLE_COMPILERS    = ['gcc', 'clang']
_SUPPORTED_COMPILERS         = _GCC_COMPATIBLE_COMPILERS + ['msvc']

compiler = meson.get_compiler(_LANG)
os = host_machine.system()
shim_static = get_option('shim_static')
symm_static = get_option('symm_static')

assert (os in _SUPPORTED_OPERATING_SYSTEMS, 'You are trying to build ' + _PROJECT + ' using an unsupported operating system!')
assert (compiler.get_id() in _SUPPORTED_COMPILERS, 'You are trying to build ' + _PROJECT + ' using an unsupported compiler!')

if os == 'windows' and compiler.get_id() == 'msvc'
	_D_PREFIX = '/D'
else
	_D_PREFIX = '-D'
endif

src = [
  'args.c',
  'main.c',
  'skein512sum.c'
  ]
# Setup these empty arrays that we will be appending to depending on OS, etc.
lib_deps   = []
lib_dir    = []
lang_flags = []
inc_dir    = []

if shim_static
  lang_flags += _D_PREFIX + 'SHIM_EXT_STATIC_LIB'
endif
if symm_static
  lang_flags += _D_PREFIX + 'SYMM_EXT_STATIC_LIB'
endif
# Determine flags to pass to gcc-compatible compilers.
gcc_compatible_flags = [
  '-fvisibility=hidden',
  '-fstrict-aliasing',
  '-Wall',
  '-funroll-loops'
  ]
if os != 'netbsd' and compiler.get_id() != 'clang'
  gcc_compatible_flags += '-flto'
endif

if os == 'darwin' # On MacOS we require __STDC_WANT_LIB_EXT1__ to be defined as 1, for access to memset_s.
  lang_flags += _D_PREFIX + '__STDC_WANT_LIB_EXT1__=1'
endif

if os in _UNIXLIKE_OPERATING_SYSTEMS
  if get_option('native_optimize')
    lang_flags += '-march=native'
  endif
  if compiler.get_id() in _GCC_COMPATIBLE_COMPILERS
    lang_flags += gcc_compatible_flags
  endif
endif

if os in _UNIXLIKE_OPERATING_SYSTEMS and compiler.get_id() in _GCC_COMPATIBLE_COMPILERS
  lang_flags += gcc_compatible_flags
endif

if os == 'openbsd' or os == 'freebsd' or os == 'darwin'
  inc_dir += '/usr/local/include'
  lib_dir += '/usr/local/lib'
elif os == 'netbsd'
  inc_dir += '/usr/local/include'
  inc_dir += '/usr/pkg/include'
  lib_dir += '/usr/local/lib'
  lib_dir += '/usr/pkg/lib'
elif os == 'linux'
  inc_dir += '/usr/include'
  if compiler.get_id() == 'gcc'
    lang_flags += '-flto'
  endif
elif os == 'windows'
  inc_dir += 'C:/include'
  lib_dir += 'C:/lib'
endif

lib_deps += compiler.find_library('shim', dirs: lib_dir, static: shim_static)
lib_deps += compiler.find_library('symm', dirs: lib_dir, static: symm_static)

if get_option('strict_arg_processing')
  lang_flags += _D_PREFIX + 'SKEIN512SUM_EXT_STRICT_ARG_PROCESSING'
endif

if os == 'windows'
  executable(_PROJECT, sources: src, dependencies: lib_deps,
	     include_directories: inc_dir, install: true,
	     c_args: lang_flags, install_dir: 'C:/bin')
else
  assert (os in _UNIXLIKE_OPERATING_SYSTEMS, 'This branch covers unixlike operating systems.')
  executable(_PROJECT, sources: src, dependencies: lib_deps,
	     include_directories: inc_dir, install: true,
	     c_args: lang_flags)
endif
