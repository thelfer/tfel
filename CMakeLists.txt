cmake_minimum_required(VERSION 2.4)
# tfel specific macros
include(cmake/modules/tfel.cmake)
# some variables for compatibility with autotools
include(cmake/modules/autotools.cmake)

tfel_project(tfel 1 0)

set(PACKAGE_BUGREPORT "thomas.helfer@cea.fr")

#compiler specific options
set(OPTIMISATION_FLAGS "")
if(UNIX)
  if(CMAKE_COMPILER_IS_GNUCC)
    # gcc specific configuration
    include(cmake/modules/gcc.cmake)
  endif(CMAKE_COMPILER_IS_GNUCC)
endif(UNIX)
add_definitions("-DOPTIMISATION_FLAGS=\\\"\"${OPTIMISATION_FLAGS}\"\\\"")

# testing
enable_testing()

if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.6)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
endif (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.6)

# packaging
include(CPack)

# sub directories
add_subdirectory(include) 
add_subdirectory(src)
add_subdirectory(mfront)
add_subdirectory(tests)
