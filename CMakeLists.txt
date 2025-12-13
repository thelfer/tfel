cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0022 NEW)
cmake_policy(SET CMP0054 NEW)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.30.0)
  cmake_policy(SET CMP0167 NEW)  
endif(CMAKE_VERSION VERSION_GREATER_EQUAL 3.30.0)
if(CMAKE_VERSION VERSION_GREATER 3.14.0)
  cmake_policy(SET CMP0083 NEW)  
  include(CheckPIESupported)
  check_pie_supported()
endif(CMAKE_VERSION VERSION_GREATER 3.14.0)

set(TFEL_VERSION_FLAVOUR "" CACHE STRING
  "Append a flavour to the version number")
option(TFEL_APPEND_VERSION
  "append TFEL version number to all libraries, executables and directories" OFF)
option(BUILD_SHARED_LIBS  "enable shared libraries" ON)
if(NOT BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  # when BUILD_SHARED_LIBS=OFF then disable option enable-static
  set(enable-static OFF CACHE BOOL "enable additional static libraries" FORCE)
endif()

if(EXISTS "${PROJECT_BINARY_DIR}/conanbuildinfo.cmake")
  include(${PROJECT_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup(NO_OUTPUT_DIRS)
  set(TFEL_USES_CONAN ON)
endif(EXISTS "${PROJECT_BINARY_DIR}/conanbuildinfo.cmake")

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

# tfel specific macros
include(cmake/modules/tfel.cmake)
# some variables for compatibility with autotools
include(cmake/modules/autotools.cmake)

#major and minor version
set(TFEL_DEVELOPMENT_VERSION ON)
tfel_project(5 0 "3")
set(PACKAGE_BUGREPORT "tfel-contact@cea.fr")

# load the list of rounding modes defined by the IEEE754 standard
include(cmake/modules/ieee754.cmake)

# check for the fenv package
include(CheckIncludeFiles)
check_include_files(fenv.h HAVE_FENV_H)
if(HAVE_FENV_H)
  tfel_add_c_cxx_definitions("HAVE_FENV")
endif(HAVE_FENV_H)

option(enable-include-what-you-use
  "enable include-what-you-use diagnostics" OFF)
if(enable-include-what-you-use)
  find_program(TFEL_IWYU_PATH NAMES include-what-you-use iwyu)
  if(NOT TFEL_IWYU_PATH)
    message(FATAL_ERROR "could not find the program include-what-you-use")
  endif()
endif(enable-include-what-you-use)

find_package(Threads)

if(UNIX)
  if(NOT TFEL_DL_LIBRARY)
    # This is used in mtest tests files
    set(TFEL_LOCAL_LIBRARY_PREFIX "./")
    # Looking for libdl...
    if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR
	(${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
      find_library(TFEL_DL_LIBRARY
	NAMES dl)
      if(NOT TFEL_DL_LIBRARY)
	MESSAGE(FATAL_ERROR "the libdl library not found. You may want to set the `TFEL_DL_LIBRARY` manually")
      endif(NOT TFEL_DL_LIBRARY)
    else((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR
	(${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
      set(TFEL_DL_LIBRARY "")
    endif((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR
      (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
  endif(NOT TFEL_DL_LIBRARY)
else(UNIX)
  set(TFEL_LOCAL_LIBRARY_PREFIX "")
  set(TFEL_DL_LIBRARY "")
endif(UNIX)

# adding support for rpath on MacOs X
if(APPLE)
   set(CMAKE_MACOSX_RPATH ON)
   set(CMAKE_SKIP_BUILD_RPATH FALSE)
   set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
   set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
   set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
   list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
   if("${isSystemDir}" STREQUAL "-1")
      set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
   endif()
endif()

# type of architecture
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  tfel_add_c_cxx_definitions("TFEL_ARCH64")
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  tfel_add_c_cxx_definitions("TFEL_ARCH32")
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

#extraction git hash
if(NOT TFEL_GIT_HASH)
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE TFEL_GIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE)
message("Abbreviated commit hash: ${TFEL_GIT_HASH}")
endif(NOT TFEL_GIT_HASH)
tfel_add_c_cxx_definitions("TFEL_GIT_HASH=\"${TFEL_GIT_HASH}\"")
tfel_add_c_cxx_definitions("TFEL_CMAKE_GENERATOR=\"${CMAKE_GENERATOR}\"")

# testing
set(CTEST_CONFIGURATION_TYPE "${JOB_BUILD_CONFIGURATION}")
# (must be placed *before* any add_subdirectory, cmake bug ?)
option(enable-testing "enable tests" ON)
option(enable-mfront-quantity-tests "enable tests of behaviours using quantities (such tests fails on old compilers)" ON)
if(enable-testing)
  enable_testing()
  if(CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND
      ${CMAKE_CTEST_COMMAND} -T test -C $<CONFIGURATION>)
  else(CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND
      ${CMAKE_CTEST_COMMAND} -T test )
  endif(CMAKE_CONFIGURATION_TYPES)
endif()

option(enable-cppcheck "analyse targets with cppcheck" OFF)
if(enable-cppcheck)
  message(STATUS "cppcheck support enabled")
  set(HAVE_CPPCHECK ON)
  include(cmake/modules/FindCppCheck.cmake)
  include(cmake/modules/CppCheckTargets.cmake)
endif(enable-cppcheck)

option(enable-vera++ "analyse targets with vera++" OFF)
if(enable-vera++)
  message(STATUS "vera++ support enabled")
  find_package(vera++)
  include(${VERA++_USE_FILE})
  if(NOT VERA_ROOT)
    message(fatal "VERA_ROOT is not defined")
  endif(NOT VERA_ROOT)
  add_vera_targets(*.h *.cpp
    RECURSE
    ROOT "${VERA_ROOT}/lib/vera++")
endif(enable-vera++)

# floatting point exceptions (TODO)

# portable-build
option(enable-portable-build "produce binary that can be shared between various machine (same architecture, same gcc version, different processors" OFF)

# madnex
option(enable-madnex "enable madnex file support in mfront (see https://github.com/thelfer/madnex)" OFF)
if(enable-madnex)
  find_package(Madnex REQUIRED)
  set(TFEL_HAVE_MADNEX ON)
endif(enable-madnex)

# fortran (placed before compiler detection)
option(enable-fortran "enable fortran support" OFF)
if(enable-fortran)
  set(HAVE_FORTRAN ON)
  tfel_add_c_cxx_definitions("HAVE_FORTRAN=1")
  enable_language (Fortran)
  message(STATUS "fortran support enabled")
  set(enable-fortran-bindings ON)
else(enable-fortran)
  set(HAVE_FORTRAN OFF)
  message(STATUS "fortran support disabled")
  set(enable-fortran-bindings OFF)
endif(enable-fortran)

#compiler options
include(cmake/modules/compiler.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
  include(cmake/modules/CodeCoverage.cmake)
  SETUP_TARGET_FOR_COVERAGE(coverage ctest coverage)
endif(CMAKE_BUILD_TYPE STREQUAL "Coverage")

#aster
option(enable-aster "build aster interface for mechanical behaviours" OFF)
if(enable-aster)
  message(STATUS "aster support enabled")
  set(HAVE_ASTER ON)
  tfel_add_c_cxx_definitions("HAVE_ASTER=1")
  include(cmake/modules/aster.cmake)
endif(enable-aster)

#abaqus
option(enable-abaqus "build abaqus interface for mechanical behaviours" OFF)
if(enable-abaqus)
  message(STATUS "abaqus support enabled")
  set(HAVE_ABAQUS ON)
  tfel_add_c_cxx_definitions("HAVE_ABAQUS=1")
endif(enable-abaqus)

# LS-DYNA
option(enable-lsdyna "build lsdyna interface for mechanical behaviours" OFF)
if(enable-lsdyna)
  message(STATUS "lsdyna support enabled")
  set(HAVE_LSDYNA ON)
  tfel_add_c_cxx_definitions("HAVE_LSDYNA=1")
endif(enable-lsdyna)

#diana-fea
option(enable-diana-fea "build Diana FEA interface for mechanical behaviours" OFF)
if(enable-diana-fea)
  message(STATUS "Diana FEA support enabled")
  set(HAVE_DIANAFEA ON)
  tfel_add_c_cxx_definitions("HAVE_DIANAFEA=1")
endif(enable-diana-fea)

#calculix
option(enable-calculix "build CalculiX interface for mechanical behaviours" OFF)
if(enable-calculix)
  message(STATUS "CalculiX support enabled")
  set(HAVE_CALCULIX ON)
  tfel_add_c_cxx_definitions("HAVE_CALCULIX=1")
endif(enable-calculix)

#ansys
option(enable-ansys "build ansys interface for mechanical behaviours" OFF)
if(enable-ansys)
  message(STATUS "ansys support enabled")
  set(HAVE_ANSYS ON)
  tfel_add_c_cxx_definitions("HAVE_ANSYS=1")
endif(enable-ansys)

#europlexus
option(enable-europlexus "build europlexus interface for mechanical behaviours" OFF)
if(enable-europlexus)
  message(STATUS "europlexus support enabled")
  set(HAVE_EUROPLEXUS ON)
  tfel_add_c_cxx_definitions("HAVE_EUROPLEXUS=1")
endif(enable-europlexus)

#zmat
option(enable-zmat "build zmat interface for mechanical behaviours" OFF)
if(enable-zmat)
  include(cmake/modules/zset.cmake)
  message(STATUS "zmat support enabled")
  set(HAVE_ZMAT ON)
  tfel_add_c_cxx_definitions("HAVE_ZMAT=1")
endif(enable-zmat)

#cyrano
include(cmake/modules/cyrano.cmake)
option(enable-cyrano "build cyrano interface for mechanical behaviours" OFF)
if(enable-cyrano)
  message(STATUS "cyrano support enabled")
  set(HAVE_CYRANO ON)
  tfel_add_c_cxx_definitions("HAVE_CYRANO=1")
endif(enable-cyrano)

#castem
include(cmake/modules/castem-unix-type.cmake)
option(local-castem-header    "use a local version of castem.h"  OFF)
option(enable-castem-pleiades "use a pleiades version of castem" OFF)
if(local-castem-header)
  tfel_add_c_cxx_definitions("LOCAL_CASTEM_HEADER=1")
  tfel_add_c_cxx_definitions("HAVE_CASTEM=1")
  set(HAVE_CASTEM ON)
  set(LOCAL_CASTEM_HEADER ON)
else(local-castem-header)
  include(cmake/modules/castem.cmake)
endif(local-castem-header)

# comsol
option(enable-comsol "build Comsol interface for mechanical behaviours" OFF)
if(enable-comsol)
  message(STATUS "Comsol support enabled")
  set(HAVE_COMSOL ON)
  tfel_add_c_cxx_definitions("HAVE_COMSOL=1")
endif(enable-comsol)

#gnuplot
include(FindGnuplot)
if(GNUPLOT_FOUND)
  set(HAVE_GNUPLOT ON)
  tfel_add_c_cxx_definitions("HAVE_GNUPLOT=1")
  tfel_add_c_cxx_definitions("GNUPLOT_PATH=\"${GNUPLOT_EXECUTABLE}\"")
  message(STATUS "found gnuplot")
  message(STATUS "gnuplot path ${GNUPLOT_EXECUTABLE}")
else(GNUPLOT_FOUND)
  set(HAVE_GNUPLOT OFF)
endif(GNUPLOT_FOUND)

#python
option(enable-python "enable python support" OFF)
option(enable-python-bindings "enable  python bindings support" OFF)
option(enable-pybind11 "using pybind11 to make the python bindings (otherwise, boost/python is used)" OFF)

option(enable-numpy-support
  "enable numpy support. This option has no effect if python bindings are disabled" ON)
option(enable-broken-boost-python-module-visibility-handling
  "workaround a bug in boost python which occurs on FreeBSD+clang. See https://github.com/boostorg/python/issues/173 for details" OFF)

option(python-static-interpreter-workaround
  "a workaround to handle the fact the conda' python interpreter is statically linked, which causes issues on Mac Os. See for details https://github.com/ContinuumIO/anaconda-issues/issues/9078. A proper solution may only appear with CMake 3.15 with the `Python::module` target, see https://gitlab.kitware.com/cmake/cmake/issues/18100. Until this version is widely available, this flag proposes a hack. Use with care." OFF)

if(enable-pybind11)
  if(NOT enable-python-bindings)
    message(FATAL_ERROR "the option enable-pybind11 is only meaningful if the python bindings are enabled")
  endif(NOT enable-python-bindings)
endif(enable-pybind11)

if(enable-python OR enable-python-bindings)
  if(enable-numpy-support)
    find_package(Python REQUIRED COMPONENTS Interpreter Development NumPy)
  else()
    find_package(Python REQUIRED COMPONENTS Interpreter Development)
  endif()
  set(HAVE_PYTHON ON)
  tfel_add_c_cxx_definitions("HAVE_PYTHON=1")
  tfel_add_c_cxx_definitions("TFEL_PYTHON_INCLUDES=\"-I${Python_INCLUDE_DIRS}\"")
  get_filename_component(PYTHON_LIBRARY_PATH ${Python_LIBRARIES} PATH)
  get_filename_component(PYTHON_LIBRARY_FULL ${Python_LIBRARIES} NAME)
  string(REGEX REPLACE "lib(.+)\\.(.+)$" "\\1" PYTHON_LIBRARY ${PYTHON_LIBRARY_FULL})
  tfel_add_c_cxx_definitions("TFEL_PYTHON_LIBRARY_PATH=\"${PYTHON_LIBRARY_PATH}\"")
  tfel_add_c_cxx_definitions("TFEL_PYTHON_LIBRARY=\"${PYTHON_LIBRARY}\"")
  if(MSVC)
    tfel_add_c_cxx_definitions("TFEL_PYTHON_LIBS=\"${PYTHON_LIBRARY_PATH}\${PYTHON_LIBRARY}\"")
  else(MSVC)
    tfel_add_c_cxx_definitions("TFEL_PYTHON_LIBS=\"-L${PYTHON_LIBRARY_PATH} -l${PYTHON_LIBRARY}\"")
  endif(MSVC) 
  message(STATUS "found python          ${Python_VERSION}")
  message(STATUS "python include path   ${Python_INCLUDE_DIRS}")
  message(STATUS "python libraries path ${PYTHON_LIBRARY_PATH}")
  message(STATUS "python library        ${PYTHON_LIBRARY}")
  message(STATUS "python interpreter    ${Python_EXECUTABLE}")
  if(NOT TFEL_PYTHON_SITE_PACKAGES_DIR)
    if(WIN32)
      set(TFEL_PYTHON_SITE_PACKAGES_DIR "bin/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/site-packages")
    else(WIN32)
      set(TFEL_PYTHON_SITE_PACKAGES_DIR "lib${LIB_SUFFIX}/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/site-packages")
    endif(WIN32)
  endif(NOT TFEL_PYTHON_SITE_PACKAGES_DIR)
  if(enable-numpy-support)
    message(STATUS "Numpy version         ${Python_NumPy_VERSION}")
    message(STATUS "Numpy header path     ${Python_NumPy_INCLUDE_DIRS}")
  endif(enable-numpy-support)
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
  if(enable-pybind11)
    find_package(pybind11 CONFIG REQUIRED)
  else(enable-pybind11)
    include(cmake/modules/boost-python.cmake)
  endif(enable-pybind11)
  include(cmake/modules/tfel-python-modules.cmake)
  list(APPEND CPACK_COMPONENTS_ALL python_bindings)
  set(CPACK_COMPONENT_PYTHON_BINDINGS_DESCRIPTION 
    "Contains the python bindings for TFEL, MFront and MTest")
  set(CPACK_COMPONENT_PYTHON_BINDINGS_DEPENDS  core mfront mtest)
  tfel_add_c_cxx_definitions("TFEL_PYTHON_BINDINGS")
  if(enable-broken-boost-python-module-visibility-handling)
    tfel_add_c_cxx_definitions("BOOST_PYTHON_USE_GCC_SYMBOL_VISIBILITY")
  endif(enable-broken-boost-python-module-visibility-handling)
endif(enable-python-bindings)

option(enable-java "enable java support" OFF)
if(enable-java)
  find_package(Java)
  if(NOT Java_JAVA_EXECUTABLE)
    message(FATAL_ERROR "java not found. java is mandatory for java suppot.")
  endif(NOT Java_JAVA_EXECUTABLE)
  if(NOT Java_JAVAC_EXECUTABLE)
    message(FATAL_ERROR "javac not found. javac is mandatory for java suppot.")
  endif(NOT Java_JAVAC_EXECUTABLE)
  find_package(JNI)
  if(NOT JNI_FOUND)
    message(FATAL_ERROR "Java native interface not found. Java native interface is mandatory for java suppot.")
  endif(NOT JNI_FOUND)
  message(STATUS "java  path       : ${Java_JAVA_EXECUTABLE}")
  message(STATUS "javac path       : ${Java_JAVAC_EXECUTABLE}")
  message(STATUS "jni include path : ${JNI_INCLUDE_DIRS}")
  set(HAVE_JAVA ON)
  tfel_add_c_cxx_definitions("HAVE_JAVA=1")
  foreach(jni_include ${JNI_INCLUDE_DIRS})
    set(JNI_INCLUDES_FLAGS "${JNI_INCLUDES_FLAGS} -I${jni_include}")
  endforeach(jni_include ${JNI_INCLUDE_DIRS})
  tfel_add_c_cxx_definitions("TFEL_JAVA_INCLUDES=\"${JNI_INCLUDES_FLAGS}\"")
endif(enable-java)

# cadna
include(cmake/modules/cadna.cmake)

# sub directories
add_subdirectory(include) 
add_subdirectory(src)
add_subdirectory(bindings)

add_subdirectory(tfel-config)
add_subdirectory(tfel-unicode-filt)
if(UNIX AND (NOT CYGWIN) AND (NOT MSYS))
  add_subdirectory(tfel-doc)
  add_subdirectory(tfel-check)
endif(UNIX AND (NOT CYGWIN) AND (NOT MSYS))

add_subdirectory(mfront)
add_subdirectory(mfront-query)
add_subdirectory(mfm)
add_subdirectory(mfront-doc)
add_subdirectory(mtest)
if(enable-testing)
  add_subdirectory(tests)
endif()
add_subdirectory(mfm-test-generator)

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
if((enable-website) OR (enable-reference-doc))
  # Looking for pandoc (http://johnmacfarlane.net/pandoc)
  include(cmake/modules/pandoc.cmake)
endif((enable-website) OR (enable-reference-doc))

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
if(CMAKE_HOST_WIN32 OR CYGWIN OR MSYS)
  if(MSYS)
     string(REGEX REPLACE "(.*):(.*)" "/\\1\\2"
             TFEL_BINARY_DIR "${PROJECT_BINARY_DIR}")
  else(MSYS)
     set(TFEL_BINARY_DIR "${PROJECT_BINARY_DIR}")
  endif(MSYS)	      
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(TFEL_CONF_DIR "/${conf}")
      configure_file(env-win32.sh.in env-${conf}.sh @ONLY)
      configure_file(env-win32.bat.in env-${conf}.bat @ONLY)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    set(TFEL_CONF_DIR "")
    configure_file(env-win32.sh.in  env.sh  @ONLY)
    configure_file(env-win32.bat.in env.bat @ONLY)
  endif(CMAKE_CONFIGURATION_TYPES)
endif(CMAKE_HOST_WIN32 OR CYGWIN OR MSYS)

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

  if(MSVC)
    if(enable-python-bindings)
      if(Boost_PYTHON_LIBRARY_DEBUG)
	string(REPLACE ".lib" ".dll"
	       Boost_PYTHON_DLL_DEBUG
	       "${Boost_PYTHON_LIBRARY_DEBUG}")
        add_to_system_runtime_libs("${Boost_PYTHON_DLL_DEBUG}")
      endif(Boost_PYTHON_LIBRARY_DEBUG)
      if(Boost_PYTHON_LIBRARY_RELEASE)
	string(REPLACE ".lib" ".dll"
	       Boost_PYTHON_DLL_RELEASE
	       "${Boost_PYTHON_LIBRARY_RELEASE}")
        add_to_system_runtime_libs("${Boost_PYTHON_DLL_RELEASE}")
      endif(Boost_PYTHON_LIBRARY_RELEASE)
    endif(enable-python-bindings)
  endif(MSVC)

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

# install tfel-modules-common.cmake

configure_file(${PROJECT_SOURCE_DIR}/cmake/modules/tfel-modules-common.cmake.in
               ${PROJECT_BINARY_DIR}/cmake/modules/tfel-modules-common.cmake
               @ONLY)

if(TFEL_APPEND_SUFFIX)
  install(FILES ${PROJECT_BINARY_DIR}/cmake/modules/tfel-modules-common.cmake
          DESTINATION "share/tfel-${TFEL_SUFFIX}/cmake"
          COMPONENT core)
else(TFEL_APPEND_SUFFIX)
  install(FILES ${PROJECT_BINARY_DIR}/cmake/modules/tfel-modules-common.cmake
          DESTINATION "share/tfel/cmake"
          COMPONENT core)
endif(TFEL_APPEND_SUFFIX)

# install licences
if(TFEL_APPEND_SUFFIX)
  install (FILES LICENCE-GNU-GPL
    DESTINATION "share/doc/tfel-${TFEL_SUFFIX}"
    COMPONENT core)
  install (FILES LICENCE-CECILL-A-EN
    DESTINATION "share/doc/tfel-${TFEL_SUFFIX}"
    COMPONENT core)
  install (FILES LICENCE-CECILL-A-FR
    DESTINATION "share/doc/tfel-${TFEL_SUFFIX}"
    COMPONENT core)
else(TFEL_APPEND_SUFFIX)
  install (FILES LICENCE-GNU-GPL DESTINATION "share/doc/tfel"
    COMPONENT core)
  install (FILES LICENCE-CECILL-A-EN DESTINATION "share/doc/tfel"
    COMPONENT core)
  install (FILES LICENCE-CECILL-A-FR DESTINATION "share/doc/tfel"
    COMPONENT core)
endif(TFEL_APPEND_SUFFIX)

# install test-suite
if(TFEL_APPEND_SUFFIX)
  install(DIRECTORY test-suite
    DESTINATION share/tfel-${TFEL_SUFFIX}/
    COMPONENT code
    PATTERN ".svn" EXCLUDE)
else(TFEL_APPEND_SUFFIX)
  install(DIRECTORY test-suite
    DESTINATION share/tfel/
    COMPONENT code
    PATTERN ".svn" EXCLUDE)
endif(TFEL_APPEND_SUFFIX)

set(CPACK_PACKAGE_VERSION   "${TFEL_VERSION}")
if(${CMAKE_SYSTEM_PROCESSOR})
  set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_PROCESSOR}") 
else(${CMAKE_SYSTEM_PROCESSOR})
  set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}") 
endif(${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_PACKAGE_INSTALL_DIRECTORY   "${CPACK_PACKAGE_VERSION}")
set(CPACK_PACKAGE_NAME                "${PACKAGE_NAME}")
set(CPACK_PACKAGE_CONTACT             "${PACKAGE_BUGREPORT}")
set(CPACK_PACKAGE_VERSION_PATCH       "${TFEL_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR       "${TFEL_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR       "${TFEL_VERSION_MINOR}")
set(CPACK_PACKAGE_VENDOR              "CEA_EDF")
#set(CPACK_SOURCE_IGNORE_FILES         "build;/\.git/;.*~")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TFEL is a numerical library.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENCE-GNU-GPL")

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
set(CPACK_NSIS_MUI_ICON       "${PROJECT_SOURCE_DIR}/docs/mfront.ico")
set(CPACK_NSIS_MUI_UNIICON    "${PROJECT_SOURCE_DIR}/docs/mfront.ico")
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

set(CPACK_BUNDLE_NAME "TFEL/MFront")

# source packaging
set(CPACK_VERBATIM_VARIABLES YES)
if(NOT CPACK_SOURCE_GENERATOR)
  set(CPACK_SOURCE_GENERATOR "TGZ")
endif()

# packaging
include(CPack)
