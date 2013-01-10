macro(tfel_project tfel_version_major tfel_version_minor)
  project("tfel")
  set(PACKAGE_NAME "tfel")
  set(VERSION "${tfel_version_major}.${tfel_version_minor}")
  # the version number.
  set(TFEL_VERSION_MAJOR "${tfel_version_major}")
  set(TFEL_VERSION_MINOR "${tfel_version_minor}")
endmacro(tfel_project)

macro(install_header dir file)
  install(FILES ${dir}/${file}
          DESTINATION "include/${dir}")
endmacro(install_header)

macro(install_data dir file)
  install(FILES ${file}
          DESTINATION "share/tfel/${dir}")
endmacro(install_data)

macro(tfel_library name)
if(${ARGC} LESS 2)
   message(FATAL_ERROR "tfel_library : no source specified")
endif(${ARGC} LESS 2)
add_library(${name} SHARED ${ARGN})
install(TARGETS ${name} DESTINATION lib)
if(ENABLE_STATIC)
 add_library(${name}-static STATIC ${ARGN})
 set_target_properties(${name}-static PROPERTIES OUTPUT_NAME "${name}")
 # Now the library target "${name}-static" will be named "${name}.lib"
 # with MS tools.
 # This conflicts with the "${name}.lib" import library corresponding
 # to "${name}.dll",
 # so we add a "lib" prefix (which is default on other platforms
 # anyway):
 set_target_properties(${name}-static PROPERTIES PREFIX "lib")
 # Help CMake 2.6.x and lower (not necessary for 2.8 and above, but
 # doesn't hurt):
 set_target_properties(${name}        PROPERTIES CLEAN_DIRECT_OUTPUT 1)
 set_target_properties(${name}-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
 install(TARGETS ${name}-static DESTINATION lib)
endif(ENABLE_STATIC)
endmacro(tfel_library)
