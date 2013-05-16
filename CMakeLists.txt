cmake_minimum_required(VERSION 2.4)
# tfel specific macros
include(cmake/modules/tfel.cmake)
# some variables for compatibility with autotools
include(cmake/modules/autotools.cmake)

#major and minor version
tfel_project(1 1)
set(PACKAGE_BUGREPORT "thomas.helfer@cea.fr")

#extraction svn revision
if(NOT TFEL_SVN_REVISION)
execute_process(COMMAND svnversion
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                OUTPUT_VARIABLE TFEL_SVN_REVISION 
		OUTPUT_STRIP_TRAILING_WHITESPACE)
endif(NOT TFEL_SVN_REVISION)

# testing
# (must be placed *before* any add_subdirectory, cmake bug ?)
enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# fortran (placed before compiler detection)
option(enable-fortran "enable fortran support")
if(enable-fortran)
  set(HAVE_FORTRAN ON)
  add_definitions("-DHAVE_FORTRAN=1")
  enable_language (Fortran)
  message(STATUS "fortran support enabled")
else(enable-fortran)
  set(HAVE_FORTRAN OFF)
  message(STATUS "fortran support disabled")
endif(enable-fortran)

#compiler specific options
set(OPTIMISATION_FLAGS "")
set(COMPILER_WARNINGS "")

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  include(cmake/modules/gcc.cmake)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  # the clang compiler has the very same options than the gnu one
  include(cmake/modules/gcc.cmake)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
  include(cmake/modules/intel.cmake)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "PathScale")
  include(cmake/modules/pathscale.cmake)
endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
if(MSVC)
  add_definitions("-DMICROSOFT_COMPILER_MSC_=1")
  add_definitions("/wd4251 /wd4275")
endif(MSVC)

cmake_policy(SET CMP0005 OLD)
add_definitions("-DOPTIMISATION_FLAGS=\\\"\"${OPTIMISATION_FLAGS}\"\\\"")
add_definitions("-DOPTIMISATION_FLAGS=\\\"\"${OPTIMISATION_FLAGS}\"\\\"")
add_definitions("-DCOMPILER_WARNINGS=\\\"\"${COMPILER_WARNINGS}\"\\\"")
add_definitions("-DVERSION=\\\"\"${TFEL_VERSION_MAJOR}.${TFEL_VERSION_MINOR}\"\\\"")
add_definitions("-DTFEL_SVN_REVISION=\\\"\"${TFEL_SVN_REVISION}\"\\\"")

if(CMAKE_BUILD_TYPE STREQUAL "None")
  set(CMAKE_CXX_FLAGS "${OPTIMISATION_FLAGS} ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${COMPILER_WARNINGS}  ${CMAKE_CXX_FLAGS}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS_RELEASE "${OPTIMISATION_FLAGS} ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE "${COMPILER_WARNINGS}  ${CMAKE_CXX_FLAGS}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS_DEBUG "${OPTIMISATION_FLAGS} ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_DEBUG "${COMPILER_WARNINGS}  ${CMAKE_CXX_FLAGS}")
endif(CMAKE_BUILD_TYPE STREQUAL "None")

#aster
option(enable-aster "build aster interface for mechanical behaviours" OFF)
if(enable-aster)
  message(STATUS "aster support enabled")
  set(HAVE_ASTER ON)
  add_definitions("-DHAVE_ASTER=1")
  include(cmake/modules/aster.cmake)
endif(enable-aster)

#castem
include(cmake/modules/castem-unix-type.cmake)
option(local-castem-header "use a local version of castem.h" OFF)
if(local-castem-header)
  if(NOT HAVE_FORTRAN)
    message(FATAL_ERROR "fortran support must be enabled for castem support")
  endif(NOT HAVE_FORTRAN)
  add_definitions(-DLOCAL_CASTEM_HEADER=1)
  add_definitions("-DLOCAL_CASTEM_HEADER=1 -DHAVE_CASTEM=1")
  set(HAVE_CASTEM ON)
  set(LOCAL_CASTEM_HEADER ON)
else(local-castem-header)
  if(HAVE_FORTRAN)
    include(cmake/modules/castem.cmake)
  else(HAVE_FORTRAN)
    message(STATUS "fortran support is not enabled, skipping castem support")
  endif(HAVE_FORTRAN)
endif(local-castem-header)

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
option(enable-python "enable python support" ON)
option(enable-python-bindings "enable  python bindings support" OFF)
if(enable-python OR enable-python-bindings)
  include(FindPythonLibs)
  if(PYTHONLIBS_FOUND)
    set(HAVE_PYTHON ON)
    add_definitions("-DHAVE_PYTHON=1")
    add_definitions("-DTFEL_PYTHON_INCLUDES=\\\"\"-I${PYTHON_INCLUDE_DIRS}\"\\\"")
    get_filename_component(PYTHON_LIBRARY_PATH ${PYTHON_LIBRARIES} PATH)
    get_filename_component(PYTHON_LIBRARY_FULL ${PYTHON_LIBRARIES} NAME)
    string(REGEX REPLACE "lib(.+)\\.(.+)$" "\\1" PYTHON_LIBRARY ${PYTHON_LIBRARY_FULL})
    add_definitions("-DTFEL_PYTHON_LIBS=\\\"\"-L${PYTHON_LIBRARY_PATH} -l${PYTHON_LIBRARY}\"\\\"")
    message(STATUS "found python")
    message(STATUS "python include path   ${PYTHON_INCLUDE_DIRS}")
    message(STATUS "python libraries path ${PYTHON_LIBRARY_PATH}")
    message(STATUS "python library        ${PYTHON_LIBRARY}")
  else(PYTHONLIBS_FOUND)
    set(HAVE_PYTHON OFF)
  endif(PYTHONLIBS_FOUND)
endif(enable-python OR enable-python-bindings)

#some checks
if(enable-python)
  if(NOT HAVE_PYTHON)
    message(FATAL_ERROR "python not found. python is mandatory for python suppot.")
  endif(NOT HAVE_PYTHON)
endif(enable-python)

#python bindings
# (declared after python to search boost python)
if(enable-python-bindings)
  if(NOT HAVE_PYTHON)
    message(FATAL_ERROR "python not found. python is mandatory for python bindings.")
  endif(NOT HAVE_PYTHON)
  find_package(Boost 1.36.0 COMPONENTS python)
  if(Boost_FOUND)
    get_filename_component(Boost_PYTHON_LIBRARY ${Boost_PYTHON_LIBRARY} NAME)
    message(STATUS "Boost library python found (${Boost_PYTHON_LIBRARY}).")
  else(Boost_FOUND)
    message(FATAL_ERROR "Boost libraries not found.\n"
      "Required packages are :"${BOOST_MANDATORY_COMPONENTS})
  endif(Boost_FOUND)
endif(enable-python-bindings)

# sub directories
add_subdirectory(include) 
add_subdirectory(src)
add_subdirectory(bindings)

if(UNIX)
add_subdirectory(tfel-doc)
endif(UNIX)

add_subdirectory(mfront)
add_subdirectory(tests)

#documentations
add_custom_target(doc)
add_custom_target(doc-install)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                 ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  add_custom_target(doc-html
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM)
  add_dependencies(doc doc-html)
endif(DOXYGEN_FOUND)

find_package(LATEX)
if(LATEX_COMPILER)
  add_custom_target(doc-pdf)
  add_custom_target(doc-pdf-install)
  add_dependencies(doc-install
    doc-pdf-install)
  message(STATUS "latex compiler found : ${LATEX_COMPILER}")
  add_subdirectory(docs)
endif(LATEX_COMPILER)

#set (CPACK_PACKAGE_EXECUTABLES         "mfront;mfront")
set (CPACK_PACKAGE_INSTALL_DIRECTORY   "${PACKAGE_NAME}-${VERSION}")
set (CPACK_PACKAGE_NAME                "${PACKAGE_NAME}")
set (CPACK_PACKAGE_VERSION_PATCH       "0")
set (CPACK_PACKAGE_VERSION_MAJOR       "${TFEL_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR       "${TFEL_VERSION_MINOR}")
set (CPACK_PACKAGE_VENDOR              "CEA")
set (CPACK_SOURCE_IGNORE_FILES         "/\\\\.svn")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "tfel is a mathematical library")
set (CPACK_PACKAGE_DESCRIPTION 	       "tfel is a mathematical library")

# debian  packaging
set (CPACK_DEBIAN_PACKAGE_MAINTAINER   "Helfer Thomas ${PACKAGE_BUGREPORT}")
set (CPACK_DEBIAN_PACKAGE_SECTION      "Science")

# if(UNIX)
#   add_custom_target(dist-bzip2
#     COMMAND svn export svn://localhost/PLEIADES/SVN/TFEL/trunk 
#     ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${VERSION}
#     COMMAND tar -cvjf ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${VERSION}.tar.bz2 
#     ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${VERSION}
#     COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${VERSION}
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
# endif(UNIX)

# packaging
include(CPack)
