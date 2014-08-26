include(cmake/modules/common-compiler-flags.cmake)

tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wdisabled-optimization")
if(NOT i586-mingw32msvc_COMPILER)
  tfel_enable_cxx_compiler_flag(VISIBILITY_FLAGS "fvisibility=hidden")
  tfel_enable_cxx_compiler_flag(VISIBILITY_FLAGS "fvisibility-inlines-hidden")
endif(NOT i586-mingw32msvc_COMPILER)
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "march=native")
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "ftree-vectorize")

tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS2 "ffast-math")

option(enable-test-coverage "enable test coverage support" OFF)

if((enable-test-coverage) AND (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug")))
  message(FATAL_ERROR "test converage is only available in
  conjunction with the be 'Debug' build type")
endif((enable-test-coverage) AND (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug")))

set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
  set(OPTIMISATION_FLAGS "-O2 -DNDEBUG ${OPTIMISATION_FLAGS}")
endif((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions("-g")
  if(enable-test-coverage)
     add_definitions("-O0 -fprofile-arcs -ftest-coverage")
     set(CMAKE_EXE_LINKER_FLAGS
       "-fprofile-arcs -ftest-coverage ${CMAKE_EXE_LINKER_FLAGS} -lgcov")
     set(CMAKE_MODULE_LINKER_FLAGS
       "-fprofile-arcs -ftest-coverage ${CMAKE_MODULE_LINKER_FLAGS} -lgcov")
     set(CMAKE_SHARED_LINKER_FLAGS
       "-fprofile-arcs -ftest-coverage ${CMAKE_SHARED_LINKER_FLAGS} -lgcov")
  endif(enable-test-coverage)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if(HAVE_FORTRAN)
  include(cmake/modules/gnu-fortran-compiler.cmake)
endif(HAVE_FORTRAN)
