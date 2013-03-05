include(cmake/modules/compiler.cmake)
include(cmake/modules/common-compiler-flags.cmake)

tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "march=auto")

set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
  add_definitions("-O3 -DNDEBUG")
  set(OPTIMISATION_FLAGS "-O3 -DNDEBUG ${OPTIMISATION_FLAGS}")
endif((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions("-g")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if(HAVE_FORTRAN)
  if(${CMAKE_Fortran_COMPILER_ID} STREQUAL "PathScale")
    set(PATHSCALE_FORTRAN_COMPILER ON)
    add_definitions("-D'F77_FUNC(X,Y)=X\\#\\#_' -D'F77_FUNC_(X,Y)=X\\#\\#_'")
  else(${CMAKE_Fortran_COMPILER_ID} STREQUAL "PathScale")
    message(FATAL_ERROR "unsupported fortran compiler ${CMAKE_Fortran_COMPILER_NAME}")
  endif(${CMAKE_Fortran_COMPILER_ID} STREQUAL "PathScale")
endif(HAVE_FORTRAN)
