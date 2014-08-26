include(cmake/modules/common-compiler-flags.cmake)

tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wdisabled-optimization")
if(NOT i586-mingw32msvc_COMPILER)
  tfel_enable_cxx_compiler_flag(VISIBILITY_FLAGS "fvisibility=hidden")
  tfel_enable_cxx_compiler_flag(VISIBILITY_FLAGS "fvisibility-inlines-hidden")
endif(NOT i586-mingw32msvc_COMPILER)
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "march=native")
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "ftree-vectorize")
#tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fvect-cost-model")
# tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "floop-interchange")

tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS2 "ffast-math")
# tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fno-math-errno")
# tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fno-trapping-math")
# tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "funsafe-math-optimizations")
# #tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "ffinite-math-only")
# tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fno-rounding-math")
# tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fno-signaling-nans")
# tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fcx-limited-range")

option(enable-sanitize-options "enable various gcc sanitize options (undefined, address,...)" OFF)

set(OPTIMISATION_FLAGS "-O2 -DNDEBUG ${OPTIMISATION_FLAGS}")
set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

SET(CMAKE_CXX_FLAGS_DEBUG "-g" CACHE STRING
    "Flags used by the C++ compiler during debug builds."
    FORCE)
SET(CMAKE_C_FLAGS_DEBUG "-g" CACHE STRING
    "Flags used by the C compiler during debug builds."
    FORCE)

# coverage
SET(CMAKE_CXX_FLAGS_COVERAGE "-O0 -fprofile-arcs -ftest-coverage" CACHE STRING
    "Flags used by the C++ compiler during builds with tests coverage checks."
    FORCE)
SET(CMAKE_C_FLAGS_COVERAGE "-O0 -fprofile-arcs -ftest-coverage" CACHE STRING
    "Flags used by the C compiler during builds with tests coverage checks."
    FORCE)
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE
  "-fprofile-arcs -ftest-coverage -lgcov")
set(CMAKE_MODULE_LINKER_FLAGS_COVERAGE
  "-fprofile-arcs -ftest-coverage -lgcov")
set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
  "-fprofile-arcs -ftest-coverage -lgcov")
MARK_AS_ADVANCED(CMAKE_CXX_FLAGS_COVERAGE
  CMAKE_C_FLAGS_COVERAGE
  CMAKE_EXE_LINKER_FLAGS_COVERAGE
  CMAKE_MODULE_LINKER_FLAGS_COVERAGE
  CMAKE_SHARED_LINKER_FLAGS_COVERAGE)

# profiling
SET(CMAKE_CXX_FLAGS_PROFILING "-pg" CACHE STRING
    "Flags used by the C++ compiler during profiled builds."
    FORCE)
SET(CMAKE_C_FLAGS_PROFILING "-pg" CACHE STRING
    "Flags used by the C compiler during profiled builds."
    FORCE)
MARK_AS_ADVANCED(CMAKE_CXX_FLAGS_PROFILING
  CMAKE_C_FLAGS_PROFILING)

if(enable-sanitize-options)
  tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "fsanitize=undefined")
  tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "fsanitize=address")
  tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "fno-omit-frame-pointer")
endif(enable-sanitize-options)

if(enable-cxx-11)
  if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 4.7)
    message(FATAL_ERROR "TFEL C++11 support is only available for gcc version >= 4.7")
  endif(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 4.7)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG     "-std=c++11 ${CMAKE_CXX_FLAGS_DEBUG}")
  elseif(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS_COVERAGE  "-std=c++11 ${CMAKE_CXX_FLAGS_COVERAGE}")
  elseif(CMAKE_BUILD_TYPE STREQUAL "Profiling")
    set(CMAKE_CXX_FLAGS_PROFILING "-std=c++11 ${CMAKE_CXX_FLAGS_PROFILING}")
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE   "-std=c++11 ${CMAKE_CXX_FLAGS_RELEASE}")
  else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS           "-std=c++11 ${CMAKE_CXX_FLAGS}")
  endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(OPTIMISATION_FLAGS "-std=c++11 ${OPTIMISATION_FLAGS}")
endif(enable-cxx-11)

if(HAVE_FORTRAN)
  include(cmake/modules/gnu-fortran-compiler.cmake)
endif(HAVE_FORTRAN)
