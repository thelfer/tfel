cmake_minimum_required(VERSION 2.4)

cmake_policy(SET CMP0005 OLD)
if(CMAKE_MAJOR_VERSION VERSION_GREATER 2.8.12)
  cmake_policy(SET CMP0026 OLD)
endif(CMAKE_MAJOR_VERSION VERSION_GREATER 2.8.12)
# tfel specific macros
include(cmake/modules/tfel.cmake)
# some variables for compatibility with autotools
include(cmake/modules/autotools.cmake)

#major and minor version
tfel_project(3 0 "0-dev")
set(PACKAGE_BUGREPORT "tfel-contact@cea.fr")

# check for the fenv package
include(CheckIncludeFiles)
check_include_files(fenv.h HAVE_FENV_H)
if(HAVE_FENV_H)
  add_definitions("-DHAVE_FENV")
endif(HAVE_FENV_H)

if(UNIX)
  # This is used in mtest tests files
  set(TFEL_LOCAL_LIBRARY_PREFIX "./")
  if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(TFEL_DL_LIBRARY "dl")
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(TFEL_DL_LIBRARY "dl")
  else(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(TFEL_DL_LIBRARY "")
  endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
else(UNIX)
  set(TFEL_LOCAL_LIBRARY_PREFIX "")
  set(TFEL_DL_LIBRARY "")
endif(UNIX)

# type of architecture
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  add_definitions("-DTFEL_ARCH64")
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  add_definitions("-DTFEL_ARCH32")
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

#extraction svn revision
if(NOT TFEL_SVN_REVISION)
execute_process(COMMAND svnversion
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                OUTPUT_VARIABLE TFEL_SVN_REVISION 
		OUTPUT_STRIP_TRAILING_WHITESPACE)
endif(NOT TFEL_SVN_REVISION)
add_definitions("-DTFEL_SVN_REVISION=\\\"\"${TFEL_SVN_REVISION}\"\\\"")

# testing
set(CTEST_CONFIGURATION_TYPE "${JOB_BUILD_CONFIGURATION}")
# (must be placed *before* any add_subdirectory, cmake bug ?)
enable_testing()
add_custom_target(check COMMAND 
  ${CMAKE_CTEST_COMMAND} -T test)

# floatting point exceptions (TODO)

# portable-build
option(enable-portable-build "produce binary that can be shared between various machine (same architecture, same gcc version, different processors" OFF)

# C++-11 (placed before compiler detection)
set(enable-cxx-11 ON)

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

#compiler options
include(cmake/modules/compiler.cmake)

#aster
option(enable-aster "build aster interface for mechanical behaviours" OFF)
if(enable-aster)
  message(STATUS "aster support enabled")
  set(HAVE_ASTER ON)
  add_definitions("-DHAVE_ASTER=1")
  include(cmake/modules/aster.cmake)
endif(enable-aster)

#zmat
option(enable-zmat "build zmat interface for mechanical behaviours" OFF)
if(enable-zmat)
  include(cmake/modules/zset.cmake)
  message(STATUS "zmat support enabled")
  set(HAVE_ZMAT ON)
  add_definitions("-DHAVE_ZMAT=1")
endif(enable-zmat)

#cyrano
option(enable-cyrano "build cyrano interface for mechanical behaviours" OFF)
if(enable-cyrano)
  message(STATUS "cyrano support enabled")
  set(HAVE_CYRANO ON)
  add_definitions("-DHAVE_CYRANO=1")
  include(cmake/modules/cyrano.cmake)
endif(enable-cyrano)

#castem
include(cmake/modules/castem-unix-type.cmake)
option(local-castem-header    "use a local version of castem.h"  OFF)
option(enable-castem-pleiades "use a pleiades version of castem" OFF)
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
option(enable-python "enable python support" OFF)
option(enable-python-bindings "enable  python bindings support" OFF)
if(enable-python OR enable-python-bindings)
  include(FindPythonInterp)
  include(FindPythonLibs)
  if(PYTHONLIBS_FOUND)
    set(HAVE_PYTHON ON)
    add_definitions("-DHAVE_PYTHON=1")
    add_definitions("-DTFEL_PYTHON_INCLUDES=\\\"\"-I${PYTHON_INCLUDE_DIRS}\"\\\"")
    get_filename_component(PYTHON_LIBRARY_PATH ${PYTHON_LIBRARIES} PATH)
    get_filename_component(PYTHON_LIBRARY_FULL ${PYTHON_LIBRARIES} NAME)
    string(REGEX REPLACE "lib(.+)\\.(.+)$" "\\1" PYTHON_LIBRARY ${PYTHON_LIBRARY_FULL})
    add_definitions("-DTFEL_PYTHON_LIBS=\\\"\"-L${PYTHON_LIBRARY_PATH} -l${PYTHON_LIBRARY}\"\\\"")
    message(STATUS "found python          ${PYTHON_VERSION}")
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
      "Required packages are :" ${BOOST_MANDATORY_COMPONENTS})
  endif(Boost_FOUND)
  list(APPEND CPACK_COMPONENTS_ALL python_bindings)
  set(CPACK_COMPONENT_PYTHON_BINDINGS_DESCRIPTION 
    "Contains the python bindings for TFEL, MFront and MTest")
  set(CPACK_COMPONENT_PYTHON_BINDINGS_DEPENDS  core mfront mtest)
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

# add a target to generate API documentation with Doxygen
option(enable-doxygen-doc "enable generation of the Doxygen documentation" OFF)
if(enable-doxygen-doc)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
      ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    add_custom_target(doc-html
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM)
    add_dependencies(doc doc-html)
    list(APPEND CPACK_COMPONENTS_ALL doxygen)
    set(CPACK_COMPONENT_DOXYGEN_DESCRIPTION
      "Contains the TFEL doxygen documentation")
    set(GENERATE_DOXYGEN ON)
  else(DOXYGEN_FOUND)
    message(FATAL_ERROR "doxygen is required")
  endif(DOXYGEN_FOUND)
else(enable-doxygen-doc)
  message(STATUS "doxygen documentation is not enabled")
endif(enable-doxygen-doc)

# Looking for LaTeX
option(enable-reference-doc "enable generation of the reference documentation" OFF)
if(enable-reference-doc)
  include(cmake/modules/latex.cmake)
endif(enable-reference-doc)

option(enable-website "enable generation of the website" ON)
if((enable-website) AND (enable-reference-doc))
  # Looking for pandoc (http://johnmacfarlane.net/pandoc)
  include(cmake/modules/pandoc.cmake)
endif((enable-website) AND (enable-reference-doc))

if(enable-reference-doc)
  if(HAVE_PANDOC OR HAVE_LATEX)
    list(APPEND CPACK_COMPONENTS_ALL docs)
    set(CPACK_COMPONENT_DOCS_DESCRIPTION
      "Contains the documentation related to TFEL, MFront an MTest")
  endif(HAVE_PANDOC OR HAVE_LATEX)
  set(GENERATE_REFERENCE_DOC ON)
else(enable-reference-doc)
  message(STATUS "Generation of the reference documentation disabled")
endif(enable-reference-doc)

if(enable-website)
  if(HAVE_PANDOC)
    list(APPEND CPACK_COMPONENTS_ALL website)
    set(CPACK_COMPONENT_WEBSITE_DESCRIPTION
      "Contains the TFEL web site")
    set(GENERATE_WEBSITE ON)
  endif(HAVE_PANDOC)
else(enable-website)
  message(STATUS "Generation of the TFEL website disabled")
endif(enable-website)

# docs is always included because of the documentation txt files of
# mfront and mtest (--help-command option of mfront and mtest)
add_subdirectory(docs)

## environment under msys
if (MSYS)
  configure_file(env-msys.sh.in env-msys.sh @ONLY)
endif(MSYS)

## Packaging
option(install-system-libraries "install system libraries" OFF)

if(install-system-libraries)
  macro(add_to_system_runtime_libs fn)
    if(EXISTS ${fn})
      message(STATUS "adding '${fn}' to system runtime libraries")
      LIST(APPEND CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${fn})
    endif(EXISTS ${fn})
  endmacro(add_to_system_runtime_libs)
      
  if( MINGW )
    message(STATUS "Installing system-libraries: MinGW DLLs." )
    get_filename_component(Mingw_Path ${CMAKE_CXX_COMPILER} PATH)
    file(GLOB libgcc  "${Mingw_Path}/libgcc_s*")
    add_to_system_runtime_libs("${libgcc}")
    add_to_system_runtime_libs("${Mingw_Path}/libstdc++-6.dll")
  endif( MINGW )

  message(STATUS "runtime libraries : ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}")
  include(InstallRequiredSystemLibraries)

  # Actually install it when make install is called.
  if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
    list(APPEND CPACK_COMPONENTS_ALL system)
    set(CPACK_COMPONENT_SYSTEM_DESCRIPTION
      "build system dependencies (e.g. libstdc++ with MingGW)")
    set(CPACK_COMPONENT_CORE_DEPENDS  system)
    foreach(file ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
      install(PROGRAMS ${file} DESTINATION bin COMPONENT system)
    endforeach(file)
  endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )

endif(install-system-libraries)

# install licences
install (FILES LICENCE-GNU-GPL DESTINATION "share/doc/tfel"
  COMPONENT core)
install (FILES LICENCE-CECILL-A-EN DESTINATION "share/doc/tfel"
  COMPONENT core)
install (FILES LICENCE-CECILL-A-FR DESTINATION "share/doc/tfel"
  COMPONENT core)

set(CPACK_PACKAGE_VERSION   "${CMAKE_PROJECT_NAME}-${TFEL_VERSION}")
if(${CMAKE_SYSTEM_PROCESSOR})
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_PROCESSOR}") 
else(${CMAKE_SYSTEM_PROCESSOR})
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_VERSION}") 
endif(${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_PACKAGE_INSTALL_DIRECTORY   "${CPACK_PACKAGE_VERSION}")
set(CPACK_PACKAGE_NAME                "${PACKAGE_NAME}")
set(CPACK_PACKAGE_CONTACT             "${PACKAGE_BUGREPORT}")
set(CPACK_PACKAGE_VERSION_PATCH       "${TFEL_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR       "${TFEL_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR       "${TFEL_VERSION_MINOR}")
set(CPACK_PACKAGE_VENDOR              "CEA_EDF")
set(CPACK_SOURCE_IGNORE_FILES         "build;/\\\\.svn/;.*~")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TFEL is a numerical library.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENCE-GNU-GPL")

# rpm packaging
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_RPM_PACKAGE_LICENSE "GPL/CECILL-A")

# debian packaging
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER   "Helfer Thomas ${PACKAGE_BUGREPORT}")
set(CPACK_DEBIAN_PACKAGE_SECTION      "Science")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE     "${TFEL_WEBSITE}")

#NSIS
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_NSIS_MUI_ICON       "${CMAKE_SOURCE_DIR}/docs/mfront.ico")
set(CPACK_NSIS_MUI_UNIICON    "${CMAKE_SOURCE_DIR}/docs/mfront.ico")
set(CPACK_NSIS_URL_INFO_ABOUT "${TFEL_WEBSITE}")
set(CPACK_NSIS_HELP_LINK "${TFEL_WEBSITE}")
set(CPACK_NSIS_CONTACT "${PACKAGE_BUGREPORT}")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_INSTALL_ROOT "C:")
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
      SetRegView  64
      WriteRegStr HKCR 'TFELHOME-${TFEL_VERSION}' '' '$INSTDIR'")
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
      SetRegView  64
      DeleteRegKey HKCR 'TFELHOME-${TFEL_VERSION}'")

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
