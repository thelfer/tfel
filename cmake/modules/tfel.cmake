macro(tfel_project tfel_version_major tfel_version_minor tfel_version_patch)
  project("tfel")
  set(PACKAGE_NAME "tfel")
  set(VERSION "${tfel_version_major}.${tfel_version_minor}.${tfel_version_patch}")
  set(TFEL_VERSION "${VERSION}")
  set(TFEL_WEBSITE "http://tfel.sourceforce.net")
  # the version number.
  set(TFEL_VERSION_MAJOR "${tfel_version_major}")
  set(TFEL_VERSION_MINOR "${tfel_version_minor}")
  set(TFEL_VERSION_PATCH "${tfel_version_patch}")
  add_definitions("-DVERSION=\\\"\"${TFEL_VERSION}\"\\\"")
  add_definitions("-DLIB_SUFFIX=\\\"\"${LIB_SUFFIX}\"\\\"")
endmacro(tfel_project)

set(CPACK_COMPONENTS_ALL core mfront mtest)

set(CPACK_COMPONENT_CORE_DESCRIPTION 
  "Contains all the core libraries developped within TFEL")
set(CPACK_COMPONENT_MFRONT_DESCRIPTION
  "Contains the MFront code generator and the associated libraries")
set(CPACK_COMPONENT_MTESTS_DESCRIPTION
  "Contains the MTest tool")

set(CPACK_COMPONENT_MFRONT_DEPENDS core)
set(CPACK_COMPONENT_MTEST_DEPENDS  core mfront)

macro(install_header dir file)
  install(FILES ${dir}/${file}
          DESTINATION "include/${dir}"
	  COMPONENT core)
endmacro(install_header)

macro(install_mfront_header dir file)
  install(FILES ${dir}/${file}
          DESTINATION "include/${dir}"
	  COMPONENT mfront)
endmacro(install_mfront_header)

macro(install_mtest_header dir file)
  install(FILES ${dir}/${file}
          DESTINATION "include/${dir}"
	  COMPONENT mtest)
endmacro(install_mtest_header)

macro(install_data dir file)
  install(FILES ${file}
          DESTINATION "share/tfel/${dir}")
endmacro(install_data)

macro(install_mfront_data dir file)
  install(FILES ${file}
          DESTINATION "share/mfront/${dir}"
	  COMPONENT mfront)
endmacro(install_mfront_data)

macro(tfel_library_internal name component)
  if(${ARGC} LESS 2)
    message(FATAL_ERROR "tfel_library_internal : no source specified")
  endif(${ARGC} LESS 2)
  add_library(${name} SHARED ${ARGN})
  if(WIN32)
    install(TARGETS ${name} DESTINATION bin
      COMPONENT ${component})
  else(WIN32)
    install(TARGETS ${name}
      DESTINATION lib${LIB_SUFFIX}
      COMPONENT ${component})
  endif(WIN32)
  if(enable-static)
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
    set_target_properties(${name}-static PROPERTIES COMPILE_FLAGS "-D${name}_EXPORTS -DTFEL_STATIC_BUILD")
    if(WIN32)
      install(TARGETS ${name}-static DESTINATION bin)
    else(WIN32)
      install(TARGETS ${name}-static DESTINATION lib${LIB_SUFFIX})
    endif(WIN32)
  endif(enable-static)
endmacro(tfel_library_internal)

macro(tfel_library name)
  tfel_library_internal(${name} core ${ARGN})
endmacro(tfel_library)

macro(mfront_library name)
  tfel_library_internal(${name} mfront ${ARGN})
endmacro(mfront_library)

macro(mtest_library name)
  tfel_library_internal(${name} mtest ${ARGN})
endmacro(mtest_library)

macro(add_mfront_behaviour_generated_source lib interface file)
  set(mfront_file   "${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/${file}.mfront")
  if(WIN32)
    set(mfront_executable "${PROJECT_BINARY_DIR}/mfront/src/mfront.exe")
  else(WIN32)
    set(mfront_executable "${PROJECT_BINARY_DIR}/mfront/src/mfront")
  endif(WIN32)
  add_custom_command(
    OUTPUT  "src/${file}.cxx"
    OUTPUT  "src/${interface}${file}.cxx"
    COMMAND "${mfront_executable}"
    ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/behaviours"
    ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties"
    ARGS    "--interface=${interface}" "${mfront_file}"
    DEPENDS "${PROJECT_BINARY_DIR}/mfront/src/mfront"
    DEPENDS "${mfront_file}"
    COMMENT "treating mfront source ${file}.mfront")
  set(${lib}_SOURCES "src/${file}.cxx" "src/${interface}${file}.cxx"
    ${${lib}_SOURCES})
endmacro(add_mfront_behaviour_generated_source)

macro(mfront_dependencies lib)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "mfront_dependencies : no source specified")
  endif(${ARGC} LESS 1)
  foreach(source ${ARGN})
    add_custom_command(
      OUTPUT  "src/${source}-mfront.cxx"
      COMMAND "${mfront_executable}"
      ARGS    "--interface=mfront" "${PROJECT_SOURCE_DIR}/mfront/tests/properties/${source}.mfront"
      DEPENDS "${PROJECT_BINARY_DIR}/mfront/src/mfront"
      DEPENDS "${mfront_file}"
      COMMENT "treating mfront source ${source}.mfront")
    set(${lib}_ADDITIONAL_SOURCES "src/${source}-mfront.cxx" ${${lib}_ADDITIONAL_SOURCES})
  endforeach(source)
endmacro(mfront_dependencies)

macro(mfront_behaviour_check_library lib interface)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "mfront_library : no source specified")
  endif(${ARGC} LESS 1)
  foreach(source ${ARGN})
    add_mfront_behaviour_generated_source(${lib} ${interface} ${source})
  endforeach(source)
  add_library(${lib} SHARED EXCLUDE_FROM_ALL
    ${${lib}_SOURCES}
    ${${lib}_ADDITIONAL_SOURCES})
  add_dependencies(check ${lib})
endmacro(mfront_behaviour_check_library)

macro(python_lib_module name package)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "python_lib_module : no source specified")
  endif(${ARGC} LESS 1)
  add_library(py_${package}_${name} SHARED ${ARGN})
  install(TARGETS py_${package}_${name}
    DESTINATION lib${LIB_SUFFIX}/${PYTHON_LIBRARY}/site-packages/${package}
    COMPONENT python_bindings)
  set_target_properties(py_${package}_${name} PROPERTIES PREFIX "")
  set_target_properties(py_${package}_${name} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(py_${package}_${name}
    ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARY})
endmacro(python_lib_module)

macro(std_python_module name)
  python_lib_module(${name} std ${ARGN})
endmacro(std_python_module)

macro(tfel_python_module name)
  python_lib_module(${name} tfel ${ARGN})
endmacro(tfel_python_module)

macro(mfront_python_module name)
  python_lib_module(${name} mfront ${ARGN})
endmacro(mfront_python_module)

macro(tfel_python_script dir)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "tfel_python_script : no script specified")
  endif(${ARGC} LESS 1)
  foreach(pyscript ${ARGN})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
      configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${pyscript}" @ONLY)
      set(python_script "${CMAKE_CURRENT_BINARY_DIR}/${pyscript}")
    else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
      set(python_script "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}")
    endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
    install(PROGRAMS ${python_script}
      DESTINATION lib${LIB_SUFFIX}/${PYTHON_LIBRARY}/site-packages/${dir}/
      COMPONENT python_bindings)
  endforeach(pyscript ${ARGN})
endmacro(tfel_python_script)
