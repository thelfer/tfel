# Modular sanitizers handling, shared by the compiler specific modules
# (gcc.cmake, clang.cmake, ...).
#
# Each compiler module declares the sanitizers it supports using
# `tfel_declare_sanitizer` (or `tfel_declare_unavailable_sanitizer` for
# the sanitizers it does not support), then calls
# `tfel_setup_sanitize_options` to process the sanitizers options.
#
# A sanitizer can be enabled in two ways:
#
# - listing it in the `enable-sanitize-options` option, which takes the
#   list of the sanitizers to enable, e.g.
#   `-Denable-sanitize-options="address;undefined"` (a comma may be used
#   as a separator instead of a semicolon).
#   Passing a boolean to this option is no longer supported since
#   enabling all the sanitizers at once is meaningless: most sanitizers
#   cannot be combined (for instance `address`, `thread` and `memory`).
# - using the dedicated option of a sanitizer, whose name is obtained
#   from the sanitizer name: `enable-<name>-sanitizer`, e.g.
#   `-Denable-bounds-strict-sanitizer=ON` or
#   `-Denable-address-sanitizer=ON`. This is convenient to enable an
#   additional sanitizer, whatever the value of
#   `enable-sanitize-options` is.
#
# A sanitizer is only added to the compilation flags if it is requested
# and if the corresponding compiler flags are available. The flags of a
# sanitizer are checked together, as some sanitizers require additional
# flags to be accepted by the compiler (for instance, with gcc,
# `pointer-compare` requires the address sanitizer).

include_guard(GLOBAL)

# legacy support: `enable-sanitize-options` used to be a boolean option
if((enable-sanitize-options STREQUAL "ON") OR
   (enable-sanitize-options STREQUAL "TRUE") OR
   (enable-sanitize-options STREQUAL "YES"))
  message(FATAL_ERROR
    "option 'enable-sanitize-options' is no longer a boolean option: it now "
    "takes the list of the sanitizers to enable "
    "(e.g. '-Denable-sanitize-options=\"address;undefined\"'). Each sanitizer "
    "can also be enabled individually using the dedicated "
    "'enable-<name>-sanitizer' option.")
endif()

set(enable-sanitize-options "" CACHE STRING
    "list of the sanitizers to enable (e.g. address;undefined). Available sanitizers depend on the compiler")

# tfel_declare_sanitizer <name> [<flag>...]
#
# declare a sanitizer supported by the compiler and the compiler flags it
# requires (defaults to `-fsanitize=<name>` so that additional flags, such
# as the flags a sanitizer depends on, can be specified). Each flag shall
# be given without its leading dash, as in
# `tfel_add_cxx_compiler_flag_if_available`.
#
# This function also defines the dedicated option of the sanitizer, whose
# name is obtained from the sanitizer name: `enable-<name>-sanitizer`,
# whose default value is always `OFF` since a sanitizer can also be
# enabled using the `enable-sanitize-options` option.
function(tfel_declare_sanitizer name)
  if(ARGC GREATER 1)
    set(sanitizers_of_${name} "${ARGN}")
  else(ARGC GREATER 1)
    set(sanitizers_of_${name} "fsanitize=${name}")
  endif(ARGC GREATER 1)
  set(sanitizers_of_${name} "${sanitizers_of_${name}}" PARENT_SCOPE)
  list(APPEND TFEL_SUPPORTED_SANITIZERS ${name})
  set(TFEL_SUPPORTED_SANITIZERS "${TFEL_SUPPORTED_SANITIZERS}" PARENT_SCOPE)
  option("enable-${name}-sanitizer" "enable the ${name} sanitizer" OFF)
endfunction(tfel_declare_sanitizer name)

# tfel_declare_unavailable_sanitizer <name>
#
# declare a sanitizer which is not supported by the compiler (either
# because it never was or because the current version of the compiler is
# too old).
#
# The dedicated option of the sanitizer, `enable-<name>-sanitizer`, is
# still defined so that the same options can be used whatever the
# compiler or its version is. Enabling such an option issues a warning
# and the sanitizer is ignored.
function(tfel_declare_unavailable_sanitizer name)
  option("enable-${name}-sanitizer"
         "the ${name} sanitizer is not supported by ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_VERSION})"
         OFF)
  mark_as_advanced("enable-${name}-sanitizer")
  list(APPEND TFEL_UNAVAILABLE_SANITIZERS ${name})
  set(TFEL_UNAVAILABLE_SANITIZERS "${TFEL_UNAVAILABLE_SANITIZERS}" PARENT_SCOPE)
endfunction(tfel_declare_unavailable_sanitizer name)

# tfel_add_sanitizer_flags_if_available <name>
#
# add the flags of the given sanitizer, stored by
# `tfel_declare_sanitizer`, to the `COMPILER_FLAGS` variable if those
# flags are available. The flags are checked together so that sanitizers
# requiring specific companion flags are handled correctly.
#
# note: the flags are passed using `TFEL_CMAKE_REQUIRED_FLAGS`, as opposed
# to `TFEL_CHECK_CXX_COMPILER_FLAG` which uses
# `TFEL_CMAKE_REQUIRED_DEFINITIONS`. This is essential for sanitizers:
# the definitions are only added to the compilation command line, while
# the required flags are also added to the link command line, which is
# required to link the runtime of the sanitizer.
function(tfel_add_sanitizer_flags_if_available name)
  set(available_flags "TFEL_SANITIZER_${name}_AVAILABLE")
  string(REPLACE ";" " -" sanitizer_flags "${sanitizers_of_${name}}")
  get_property(is_cached CACHE "${available_flags}" PROPERTY TYPE SET)
  if(NOT ${is_cached})
    set(safe_cmake_required_flags "${TFEL_CMAKE_REQUIRED_FLAGS}")
    set(TFEL_CMAKE_REQUIRED_FLAGS "-${sanitizer_flags} ")
    TFEL_CHECK_CXX_SOURCE_COMPILES("int main() { return 0;}" ${available_flags}
      # Some compilers do not fail with a bad flag
      FAIL_REGEX "unrecognized .*option"                     # GNU
      FAIL_REGEX "unknown warning option"                    # CLANG
      FAIL_REGEX "warning: optimization flag"                # CLANG
      FAIL_REGEX "ignoring unknown option"                   # MSVC
      FAIL_REGEX "[Uu]nknown option"                         # HP
      FAIL_REGEX "[Ww]arning: [Oo]ption"                     # SunPro
      FAIL_REGEX "command option .* is not recognized"       # XL
      )
    set(TFEL_CMAKE_REQUIRED_FLAGS "${safe_cmake_required_flags}")
    if(${available_flags})
      MESSAGE(STATUS "enabling sanitizer '${name}'")
    else()
      MESSAGE(STATUS "sanitizer '${name}' disabled")
    endif()
  endif()
  if(${available_flags})
    set(COMPILER_FLAGS "-${sanitizer_flags} ${COMPILER_FLAGS}" PARENT_SCOPE)
  endif()
endfunction(tfel_add_sanitizer_flags_if_available name)

# tfel_setup_sanitize_options
#
# process the sanitizers declared with `tfel_declare_sanitizer`: add the
# compiler flags of the requested sanitizers to the `COMPILER_FLAGS`
# variable.
function(tfel_setup_sanitize_options)
  if(NOT TFEL_SUPPORTED_SANITIZERS)
    message(FATAL_ERROR "tfel_setup_sanitize_options: no sanitizer has been declared")
  endif()
  # the list of sanitizers may be given as a semicolon or comma separated
  # list
  string(REPLACE "," ";" sanitize_options "${enable-sanitize-options}")
  # check that the sanitizers listed in the enable-sanitize-options option
  # are supported by the compiler
  foreach(s ${sanitize_options})
    list(FIND TFEL_SUPPORTED_SANITIZERS ${s} index_of_sanitizer)
    if(index_of_sanitizer EQUAL -1)
      message(FATAL_ERROR
        "sanitizer '${s}' listed in option 'enable-sanitize-options' is not supported by "
        "${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_VERSION}). "
        "Supported sanitizers are: ${TFEL_SUPPORTED_SANITIZERS}")
    endif()
    list(APPEND TFEL_SANITIZERS ${s})
  endforeach()
  # gather the sanitizers enabled through their dedicated option
  foreach(name ${TFEL_SUPPORTED_SANITIZERS})
    if(enable-${name}-sanitizer)
      list(APPEND TFEL_SANITIZERS ${name})
    endif()
  endforeach()
  # warn about the sanitizers which are not supported by the compiler but
  # are requested through their dedicated option
  foreach(name ${TFEL_UNAVAILABLE_SANITIZERS})
    if(enable-${name}-sanitizer)
      message(WARNING
        "option 'enable-${name}-sanitizer' is enabled but the ${name} sanitizer "
        "is not supported by ${CMAKE_CXX_COMPILER_ID} "
        "(${CMAKE_CXX_COMPILER_VERSION}). It is ignored")
    endif()
  endforeach()
  list(REMOVE_DUPLICATES TFEL_SANITIZERS)
  # the address, thread and memory sanitizers cannot be used simultaneously
  set(number_of_exclusive_sanitizers 0)
  foreach(s ${TFEL_SANITIZERS})
    if(s MATCHES "^(address|thread|memory)$")
      math(EXPR number_of_exclusive_sanitizers "${number_of_exclusive_sanitizers}+1")
    endif()
  endforeach()
  if(number_of_exclusive_sanitizers GREATER 1)
    message(FATAL_ERROR
      "the address, thread and memory sanitizers cannot be used simultaneously: "
      "requested sanitizers: ${TFEL_SANITIZERS}")
  endif()
  # enable the requested sanitizers
  foreach(name ${TFEL_SANITIZERS})
    tfel_add_sanitizer_flags_if_available(${name})
  endforeach()
  if(TFEL_SANITIZERS)
    # ease the interpretation of the reports emitted by the sanitizers
    list(FIND TFEL_SANITIZERS fno-omit-frame-pointer index_of_sanitizer)
    if(index_of_sanitizer EQUAL -1)
      tfel_add_cxx_compiler_flag_if_available(COMPILER_FLAGS
                                              "fno-omit-frame-pointer")
    endif()
  endif()
  # propagate the modifications of COMPILER_FLAGS to the caller scope
  set(COMPILER_FLAGS "${COMPILER_FLAGS}" PARENT_SCOPE)
endfunction(tfel_setup_sanitize_options)
