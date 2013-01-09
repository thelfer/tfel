cmake_minimum_required(VERSION 2.4)
# tfel specific macros
include(cmake/modules/tfel.cmake)
# some variables for compatibility with autotools
include(cmake/modules/autotools.cmake)
# check for castem
include(cmake/modules/autotools.cmake)

tfel_project(tfel 1 1)
set(PACKAGE_BUGREPORT "thomas.helfer@cea.fr")

# testing
enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

#compiler specific options
set(OPTIMISATION_FLAGS "")
set(COMPILER_WARNINGS "")
if(UNIX)
  if(CMAKE_COMPILER_IS_GNUCC)
    # gcc specific configuration
    include(cmake/modules/gcc.cmake)
  endif(CMAKE_COMPILER_IS_GNUCC)
endif(UNIX)
message("${OPTIMISATION_FLAGS}")

cmake_policy(SET CMP0005 OLD)
add_definitions("-DOPTIMISATION_FLAGS=\\\"\"${OPTIMISATION_FLAGS}\"\\\"")
add_definitions("-DOPTIMISATION_FLAGS=\\\"\"${OPTIMISATION_FLAGS}\"\\\"")
add_definitions("-DCOMPILER_WARNINGS=\\\"\"${COMPILER_WARNINGS}\"\\\"")
add_definitions("-DVERSION=\\\"\"${TFEL_VERSION_MAJOR}\"\\\"")
add_definitions("-DTFEL_SVN_REVISION=\\\"\"????\"\\\"")

#castem
include(cmake/modules/castem.cmake)

#gnuplot
include(FindGnuplot)
if(GNUPLOT_FOUND)
  set(HAVE_GNUPLOT ON)
  add_definitions("-DHAVE_GNUPLOT=1")
  add_definitions("-DGNUPLOT_PATH=\\\"\"${GNUPLOT_EXECUTABLE}\"\\\"")
  message(STATUS "found gnuplot")
  message(STATUS "gnuplot path ${GNUPLOT_EXECUTABLE}")
else(GNUPLOT_FOUND)
  set(HAVE_GNUPLOT OFF)
endif(GNUPLOT_FOUND)

#python
include(FindPythonLibs)
if(PYTHONLIBS_FOUND)
  set(HAVE_PYTHON ON)
  add_definitions("-DHAVE_PYTHON=1")
  add_definitions("-DTFEL_PYTHON_INCLUDES=\\\"\"-I${PYTHON_INCLUDE_DIRS}\"\\\"")
  get_filename_component(PYTHON_LIBRARY_PATH ${PYTHON_LIBRARIES} PATH)
  get_filename_component(PYTHON_LIBRARY      ${PYTHON_LIBRARIES} NAME)
  add_definitions("-DTFEL_PYTHON_LIBS=\\\"\"-L${PYTHON_LIBRARY_PATH} -l ${PYTHON_LIBRARY}\"\\\"")
  message(STATUS "found python")
  message(STATUS "python include path   ${PYTHON_INCLUDE_DIRS}")
  message(STATUS "python libraries path ${PYTHON_LIBRARY_PATH}")
  message(STATUS "python library        ${PYTHON_LIBRARY}")
else(PYTHONLIBS_FOUND)
  set(HAVE_PYTHON OFF)
endif(PYTHONLIBS_FOUND)


# sub directories
add_subdirectory(include) 
add_subdirectory(src)

if(UNIX)
add_subdirectory(tfel-doc)
endif(UNIX)

add_subdirectory(mfront)
add_subdirectory(tests)

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
