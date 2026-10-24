tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "Wno-conversion")
include(cmake/modules/common-compiler-flags.cmake)

set(OPTIMISATION_FLAGS "-O2 -DNDEBUG ${OPTIMISATION_FLAGS}")
set(OPTIMISATION_FLAGS "-DTFEL_NO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "Wdisabled-optimization")
if(NOT i586-mingw32msvc_COMPILER)
  tfel_add_cxx_compiler_flag_if_available(VISIBILITY_FLAGS "fvisibility=hidden")
  tfel_add_cxx_compiler_flag_if_available(VISIBILITY_FLAGS "fvisibility-inlines-hidden")
  set(COMPILER_DEFAULT_VISIBILITY "-fvisibility=default")
endif(NOT i586-mingw32msvc_COMPILER)

tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS_MARCH "march=native")
tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS_MARCH "ftree-vectorize")

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8 )
  # 32 bits machines.
  # using sse and sse2 instructions rather than the
  # i387 FPU du to numerical instabilities
  tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS_MARCH "mfpmath=sse")
  tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS_MARCH "msse")
  tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS_MARCH "msse2")
endif(NOT CMAKE_SIZEOF_VOID_P EQUAL 8 )

if(WIN32)
  if (CMAKE_SIZEOF_VOID_P EQUAL 8 )
    # 64 bits machines
    tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "m64")
  else(CMAKE_SIZEOF_VOID_P EQUAL 4 )
    # 32 bits machines
    tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "m32")
  endif(CMAKE_SIZEOF_VOID_P EQUAL 8 )
endif(WIN32)

if(enable-fast-math)
  tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS  "ffast-math")
else(enable-fast-math)
  tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS  "fno-fast-math")
  tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS2 "ffast-math")
endif(enable-fast-math)

option(enable-static-analysis  "enable gcc static analysis" OFF)

option(enable-glibcxx-debug "use the debug version of the C++ standard as implemented by the glib C++ library" OFF)
if(enable-glibcxx-debug)
  set(TFEL_CMAKE_CXX_FLAGS_DEBUG "-g -D_GLIBCXX_DEBUG" CACHE STRING
      "Flags used by the C++ compiler during debug builds."
      FORCE)
else(enable-glibcxx-debug)
  set(TFEL_CMAKE_CXX_FLAGS_DEBUG "-g" CACHE STRING
      "Flags used by the C++ compiler during debug builds."
      FORCE)
endif(enable-glibcxx-debug)

set(TFEL_CMAKE_C_FLAGS_DEBUG "-g" CACHE STRING
    "Flags used by the C compiler during debug builds."
    FORCE)

if(enable-static-analysis)
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fanalyzer")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "Wno-analyzer-use-of-uninitialized-value")
endif(enable-static-analysis)
  
# coverage
set(TFEL_CMAKE_CXX_FLAGS_COVERAGE "-O0 -g -DNDEBUG -fprofile-arcs -ftest-coverage" CACHE STRING
    "Flags used by the C++ compiler during builds with tests coverage checks."
    FORCE)
set(TFEL_CMAKE_C_FLAGS_COVERAGE "-O0 -g -DNDEBUG -fprofile-arcs -ftest-coverage" CACHE STRING
    "Flags used by the C compiler during builds with tests coverage checks."
    FORCE)
set(TFEL_CMAKE_EXE_LINKER_FLAGS_COVERAGE
  "-fprofile-arcs -ftest-coverage -lgcov")
set(TFEL_CMAKE_MODULE_LINKER_FLAGS_COVERAGE
  "-fprofile-arcs -ftest-coverage -lgcov")
set(TFEL_CMAKE_SHARED_LINKER_FLAGS_COVERAGE
  "-fprofile-arcs -ftest-coverage -lgcov")
mark_as_advanced(TFEL_CMAKE_CXX_FLAGS_COVERAGE
                 TFEL_CMAKE_C_FLAGS_COVERAGE
                 TFEL_CMAKE_EXE_LINKER_FLAGS_COVERAGE
                 TFEL_CMAKE_MODULE_LINKER_FLAGS_COVERAGE
                 TFEL_CMAKE_SHARED_LINKER_FLAGS_COVERAGE)

# profiling
set(TFEL_CMAKE_CXX_FLAGS_PROFILING "-pg" CACHE STRING
    "Flags used by the C++ compiler during profiled builds."
    FORCE)
set(TFEL_CMAKE_C_FLAGS_PROFILING "-pg" CACHE STRING
    "Flags used by the C compiler during profiled builds."
    FORCE)
mark_as_advanced(TFEL_CMAKE_CXX_FLAGS_PROFILING
                 TFEL_CMAKE_C_FLAGS_PROFILING)

# sanitizers, handled by cmake/modules/sanitizers.cmake
# note: the sanitizers available in the generic gcc documentation can be
# used: the sanitizers below are only the sanitizers supported by the
# versions of gcc tested by the CI.
# - pointer-compare and pointer-subtract require the address sanitizer
#   and the link time optimization, which are added automatically
# - hwaddress (HWASan) requires a target with memory tagging support
include(cmake/modules/sanitizers.cmake)
tfel_declare_sanitizer(address)
tfel_declare_sanitizer(thread)
tfel_declare_sanitizer(leak)
tfel_declare_sanitizer(undefined)
# sanitizers corresponding to a subset of the undefined behaviour sanitizer
# (undefined), which can be enabled individually
tfel_declare_sanitizer(alignment)
tfel_declare_sanitizer(bool)
tfel_declare_sanitizer(bounds)
tfel_declare_sanitizer(bounds-strict)
tfel_declare_sanitizer(builtin)
tfel_declare_sanitizer(enum)
tfel_declare_sanitizer(float-cast-overflow)
tfel_declare_sanitizer(float-divide-by-zero)
tfel_declare_sanitizer(null)
tfel_declare_sanitizer(object-size)
tfel_declare_sanitizer(return)
tfel_declare_sanitizer(shift)
tfel_declare_sanitizer(shift-base)
tfel_declare_sanitizer(shift-exponent)
tfel_declare_sanitizer(signed-integer-overflow)
tfel_declare_sanitizer(vptr)
# sanitizers requiring the address sanitizer and the link time optimization
tfel_declare_sanitizer(pointer-compare fsanitize=pointer-compare fsanitize=address flto)
tfel_declare_sanitizer(pointer-subtract fsanitize=pointer-subtract fsanitize=address flto)
# sanitizers requiring a target with memory tagging support
tfel_declare_sanitizer(hwaddress)
# sanitizers which are not supported by the current version of gcc: the
# dedicated options are still defined (and ignored) so that the same
# options can be used whatever the version of gcc is
tfel_declare_unavailable_sanitizer(integer)
tfel_declare_unavailable_sanitizer(vpt)
# hardening flags, which can be combined with the sanitizers above
tfel_declare_sanitizer(fstack-check fstack-check)
tfel_declare_sanitizer(fstack-protector-all fstack-protector-all)
tfel_declare_sanitizer(fno-omit-frame-pointer fno-omit-frame-pointer)
tfel_setup_sanitize_options()

# unsable flag
# set(COMPILER_CXXFLAGS "${COMPILER_CXXFLAGS} -D_GLIBCXX_CONCEPT_CHECKS")

if(HAVE_FORTRAN)
  include(cmake/modules/gnu-fortran-compiler.cmake)
endif(HAVE_FORTRAN)
