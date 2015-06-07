# - Check if the given C++ source code compiles.
# TFEL_CHECK_CXX_SOURCE_COMPILES(<code> <var> [FAIL_REGEX <fail-regex>])
#  <code>       - source code to try to compile
#  <var>        - variable to store whether the source code compiled
#  <fail-regex> - fail if test output matches this regex
# The following variables may be set before calling this macro to
# modify the way the check is run:
#
#  TFEL_CMAKE_REQUIRED_FLAGS = string of compile command line flags
#  TFEL_CMAKE_REQUIRED_DEFINITIONS = list of macros to define (-DFOO=bar)
#  TFEL_CMAKE_REQUIRED_INCLUDES = list of include directories
#  TFEL_CMAKE_REQUIRED_LIBRARIES = list of libraries to link
#
# This macro is a copy of CheckCXXSourceCompiles.cmake
# Copyright 2005-2009 Kitware, Inc.
MACRO(TFEL_CHECK_CXX_SOURCE_COMPILES SOURCE VAR)
  IF("${VAR}" MATCHES "^${VAR}$")
    SET(_FAIL_REGEX)
    SET(_key)
    FOREACH(arg ${ARGN})
      IF("${arg}" MATCHES "^(FAIL_REGEX)$")
        SET(_key "${arg}")
      ELSEIF(_key)
        LIST(APPEND _${_key} "${arg}")
      ELSE()
        MESSAGE(FATAL_ERROR "Unknown argument:\n  ${arg}\n")
      ENDIF()
    ENDFOREACH()

    SET(MACRO_CHECK_FUNCTION_DEFINITIONS
      "-D${VAR} ${TFEL_CMAKE_REQUIRED_FLAGS}")
    IF(TFEL_CMAKE_REQUIRED_LIBRARIES)
      SET(CHECK_CXX_SOURCE_COMPILES_ADD_LIBRARIES
        "-DLINK_LIBRARIES:STRING=${TFEL_CMAKE_REQUIRED_LIBRARIES}")
    ELSE(TFEL_CMAKE_REQUIRED_LIBRARIES)
      SET(CHECK_CXX_SOURCE_COMPILES_ADD_LIBRARIES)
    ENDIF(TFEL_CMAKE_REQUIRED_LIBRARIES)
    IF(TFEL_CMAKE_REQUIRED_INCLUDES)
      SET(CHECK_CXX_SOURCE_COMPILES_ADD_INCLUDES
        "-DINCLUDE_DIRECTORIES:STRING=${TFEL_CMAKE_REQUIRED_INCLUDES}")
    ELSE(TFEL_CMAKE_REQUIRED_INCLUDES)
      SET(CHECK_CXX_SOURCE_COMPILES_ADD_INCLUDES)
    ENDIF(TFEL_CMAKE_REQUIRED_INCLUDES)
    FILE(WRITE "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src.cxx"
      "${SOURCE}\n")

    TRY_COMPILE(${VAR}
      ${CMAKE_BINARY_DIR}
      ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src.cxx
      COMPILE_DEFINITIONS ${TFEL_CMAKE_REQUIRED_DEFINITIONS}
      CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
      "${CHECK_CXX_SOURCE_COMPILES_ADD_LIBRARIES}"
      "${CHECK_CXX_SOURCE_COMPILES_ADD_INCLUDES}"
      OUTPUT_VARIABLE OUTPUT)

    FOREACH(_regex ${_FAIL_REGEX})
      IF("${OUTPUT}" MATCHES "${_regex}")
        SET(${VAR} 0)
      ENDIF()
    ENDFOREACH()

    IF(${VAR})
      SET(${VAR} 1 CACHE INTERNAL "Test ${VAR}")
      FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
        "Performing C++ SOURCE FILE Test ${VAR} succeded with the following output:\n"
        "${OUTPUT}\n"
        "Source file was:\n${SOURCE}\n")
    ELSE(${VAR})
      SET(${VAR} "" CACHE INTERNAL "Test ${VAR}")
      FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
        "Performing C++ SOURCE FILE Test ${VAR} failed with the following output:\n"
        "${OUTPUT}\n"
        "Source file was:\n${SOURCE}\n")
    ENDIF(${VAR})
  ENDIF("${VAR}" MATCHES "^${VAR}$")
ENDMACRO(TFEL_CHECK_CXX_SOURCE_COMPILES)

MACRO (TFEL_CHECK_CXX_COMPILER_FLAG _FLAG _RESULT)
   SET(SAFE_TFEL_CMAKE_REQUIRED_DEFINITIONS "${TFEL_CMAKE_REQUIRED_DEFINITIONS}")
   SET(TFEL_CMAKE_REQUIRED_DEFINITIONS "${_FLAG}")
   TFEL_CHECK_CXX_SOURCE_COMPILES("int main() { return 0;}" ${_RESULT}
     # Some compilers do not fail with a bad flag
     FAIL_REGEX "unrecognized .*option"                     # GNU
     FAIL_REGEX "ignoring unknown option"                   # MSVC
     FAIL_REGEX "[Uu]nknown option"                         # HP
     FAIL_REGEX "[Ww]arning: [Oo]ption"                     # SunPro
     FAIL_REGEX "command option .* is not recognized"       # XL
     )
   SET (TFEL_CMAKE_REQUIRED_DEFINITIONS "${SAFE_TFEL_CMAKE_REQUIRED_DEFINITIONS}")
ENDMACRO (TFEL_CHECK_CXX_COMPILER_FLAG)

MACRO(tfel_enable_cxx_compiler_flag2 out flag var)
  TFEL_CHECK_CXX_COMPILER_FLAG("-${flag}" ${var})
  IF(${var})
    MESSAGE(STATUS "enabling flag '-${flag}'")
    SET(${out} "-${flag} ${${out}}")
  ELSE(${${var})
    MESSAGE(STATUS "flag '-${flag}' disabled")
  ENDIF(${var})
ENDMACRO(tfel_enable_cxx_compiler_flag2)

MACRO(tfel_enable_cxx_compiler_flag out)
  IF(${ARGC} LESS 1)
    MESSAGE(FATAL_ERROR "enable_compiler_flag : no flag specified")
  ENDIF(${ARGC} LESS 1)
  FOREACH(f ${ARGN})
    tfel_enable_cxx_compiler_flag2(${out} "${f}" ${f}_AVAILABLE)
  ENDFOREACH(f)
ENDMACRO(tfel_enable_cxx_compiler_flag)

#compiler specific options
set(CMAKE_CXX_FLAGS         "")
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_DEBUG   "")
set(VISIBILITY_FLAGS   "")
set(OPTIMISATION_FLAGS "")
set(COMPILER_WARNINGS  "")

option(PATHSCALE_COMPILER "set true if using the PathScale compiler" OFF)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  include(cmake/modules/gcc.cmake)
elseif((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") AND (NOT PATHSCALE_COMPILER))
  include(cmake/modules/clang.cmake)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
  include(cmake/modules/intel.cmake)
elseif((${CMAKE_CXX_COMPILER_ID} STREQUAL "PathScale") OR (PATHSCALE_COMPILER))
  include(cmake/modules/pathscale.cmake)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "SunPro")
  add_definitions("-library=stlport4")
  if(enable-cxx-11)
    message(FATAL_ERROR "TFEL C++11 support is not availabable with the SunPro compiler")
  endif(enable-cxx-11)
else(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
if(MSVC)
  add_definitions("-DMICROSOFT_COMPILER_MSC_=1")
#4251 : warning about export of STL classes
#4710 : warning function not being inlined
#4514 : warning inline function was not used and was removed
  add_definitions("/W4 /MD /O2 /wd4251 /wd4710 /wd4514 /wd4275 /EHsc")
  set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")
else(MSVC)
  if(enable-cxx-11)
    message(FATAL_ERROR "TFEL C++11 support is not availabable for your compiler")
  endif(enable-cxx-11)
endif(MSVC)  
endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")

add_definitions("-DOPTIMISATION_FLAGS0=\\\"\"${VISIBILITY_FLAGS} ${OPTIMISATION_FLAGS}\"\\\"")
add_definitions("-DOPTIMISATION_FLAGS=\\\"\"${OPTIMISATION_FLAGS_MARCH}\"\\\"")
add_definitions("-DOPTIMISATION_FLAGS2=\\\"\"${OPTIMISATION_FLAGS2}\"\\\"")
add_definitions("-DCOMPILER_WARNINGS=\\\"\"${COMPILER_WARNINGS}\"\\\"")
add_definitions("-DCOMPILER_FLAGS=\\\"\"${COMPILER_FLAGS}\"\\\"")

set(CMAKE_C_FLAGS "${COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${VISIBILITY_FLAGS} ${COMPILER_WARNINGS} ${COMPILER_FLAGS}")

if(CMAKE_BUILD_TYPE STREQUAL "Profiling")
  set(CMAKE_CXX_FLAGS_PROFILING "${OPTIMISATION_FLAGS} ${CMAKE_CXX_FLAGS_PROFILING}")
  if(NOT enable-portable-build)
    set(CMAKE_CXX_FLAGS_PROFILING "${OPTIMISATION_FLAGS_MARCH} ${CMAKE_CXX_FLAGS_PROFILING}")
  endif(NOT enable-portable-build)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS_RELEASE   "${OPTIMISATION_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
  if(NOT enable-portable-build)
    set(CMAKE_CXX_FLAGS_RELEASE "${OPTIMISATION_FLAGS_MARCH} ${CMAKE_CXX_FLAGS_RELEASE}")
  endif(NOT enable-portable-build)
else(CMAKE_BUILD_TYPE STREQUAL "Profiling")
  set(CMAKE_CXX_FLAGS           "${OPTIMISATION_FLAGS} ${CMAKE_CXX_FLAGS}")
  if(NOT enable-portable-build)
    set(CMAKE_CXX_FLAGS_RELEASE "${OPTIMISATION_FLAGS_MARCH} ${CMAKE_CXX_FLAGS_RELEASE}")
  endif(NOT enable-portable-build)
endif(CMAKE_BUILD_TYPE STREQUAL "Profiling")
