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

option(enable-sanitize-options "enable various gcc sanitize options (undefined, address,...)" OFF)

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

# tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fanalyzer")

if(enable-sanitize-options)
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=bounds-strict")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=undefined")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=float-divide-by-zero")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=float-cast-overflow")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=bounds")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=alignment")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=object-size")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=vpt")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=address")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=null")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=return")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=signed-integer-overflow")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=bool")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=enum")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fstack-check")
  #  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fsanitize=leak")
  tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS "fno-omit-frame-pointer")
endif(enable-sanitize-options)

# unsable flag
# set(COMPILER_CXXFLAGS "${COMPILER_CXXFLAGS} -D_GLIBCXX_CONCEPT_CHECKS")

if(HAVE_FORTRAN)
  include(cmake/modules/gnu-fortran-compiler.cmake)
endif(HAVE_FORTRAN)
