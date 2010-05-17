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

# sub directories
add_subdirectory(include) 
add_subdirectory(src)
add_subdirectory(mfront)
add_subdirectory(tests)

# testing
enable_testing()

if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.6)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
endif (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.6)

# packaging
include(CPack)

set (CPACK_PACKAGE_EXECUTABLES         "mfront;mfront")
set (CPACK_PACKAGE_INSTALL_DIRECTORY   "${PACKAGE_NAME}-${VERSION}")
set (CPACK_PACKAGE_NAME                "${PACKAGE_NAME}")
set (CPACK_PACKAGE_VERSION_MAJOR       "${TFEL_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR       "${TFEL_VERSION_MINOR}")
set (CPACK_PACKAGE_VENDOR              "CEA")
set (CPACK_SOURCE_IGNORE_FILES         "\\.svn/")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "tfel is a mathematical library")
set (CPACK_PACKAGE_DESCRIPTION 	       "tfel is a mathematical library")

# debian  packaging
set (CPACK_DEBIAN_PACKAGE_MAINTAINER   "Helfer Thomas thomas.helfer@cea.fr ${PACKAGE_BUGREPORT}")
set (CPACK_DEBIAN_PACKAGE_SECTION      "Science")
