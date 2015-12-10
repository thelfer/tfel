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
  if(LIB_SUFFIX)
    add_definitions("-DLIB_SUFFIX=\\\"\"${LIB_SUFFIX}\"\\\"")
  endif(LIB_SUFFIX)
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
  if(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
    set(mfront_executable "$<TARGET_FILE:mfront>")
  else(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
    # retrieve the old behaviour for debian squeeze's version of cmake
    # does not work with configurations
    if(WIN32)
      set(mfront_executable "${PROJECT_BINARY_DIR}/mfront/src/mfront.exe")
    else(WIN32)
      set(mfront_executable "${PROJECT_BINARY_DIR}/mfront/src/mfront")
    endif(WIN32)
  endif(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(mfront_flags "--debug")
  else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(mfront_flags "")
  endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    add_custom_command(
      OUTPUT  "src/${file}.cxx"
      OUTPUT  "src/${interface}${file}.cxx"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELMFront>;%PATH%"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:MFrontLogStream>;%PATH%"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELMaterial>;%PATH%"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELMathParser>;%PATH%"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELGlossary>;%PATH%"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELSystem>;%PATH%"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELUtilities>;%PATH%"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELException>;%PATH%"
      COMMAND "${mfront_executable}"
      ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/behaviours"
      ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties"
      ARGS    "${mfront_flags}" "--interface=${interface}" "${mfront_file}"
      DEPENDS "${PROJECT_BINARY_DIR}/mfront/src/mfront"
      DEPENDS "${mfront_file}"
      COMMENT "treating mfront source ${file}.mfront")
    else((CMAKE_HOST_WIN32) AND (NOT MSYS))
      add_custom_command(
	OUTPUT  "src/${file}.cxx"
	OUTPUT  "src/${interface}${file}.cxx"
	COMMAND "${mfront_executable}"
	ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/behaviours"
	ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties"
	ARGS    "${mfront_flags}" "--interface=${interface}" "${mfront_file}"
	DEPENDS "${PROJECT_BINARY_DIR}/mfront/src/mfront"
	DEPENDS "${mfront_file}"
	COMMENT "treating mfront source ${file}.mfront")
      file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generation-test)
      if(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
	add_test(NAME mfront-${file}-${interface}
	  COMMAND ${mfront_executable}
	  --search-path=${PROJECT_SOURCE_DIR}/mfront/tests/behaviours
	  --search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties
	  ${mfront_flags}
	  --interface=${interface} ${mfront_file}
	  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generation-test")
      endif(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  set(${lib}_SOURCES "src/${file}.cxx" "src/${interface}${file}.cxx"
    ${${lib}_SOURCES})
endmacro(add_mfront_behaviour_generated_source)

macro(mfront_dependencies lib)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "mfront_dependencies : no source specified")
  endif(${ARGC} LESS 1)
  foreach(source ${ARGN})
    if(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
      set(mfront_executable "$<TARGET_FILE:mfront>")
    else(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
      # retrieve the old behaviour for debian squeeze's version of cmake
      # does not work with configurations
      if(WIN32)
	set(mfront_executable "${PROJECT_BINARY_DIR}/mfront/src/mfront.exe")
      else(WIN32)
	set(mfront_executable "${PROJECT_BINARY_DIR}/mfront/src/mfront")
      endif(WIN32)
    endif(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(mfront_flags "--debug")
    else(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(mfront_flags "")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      add_custom_command(
	OUTPUT  "src/${source}-mfront.cxx"
	COMMAND "set"
	ARGS "PATH=$<TARGET_FILE_DIR:TFELMFront>;%PATH%"
	COMMAND "set"
	ARGS "PATH=$<TARGET_FILE_DIR:MFrontLogStream>;%PATH%"
	COMMAND "set"
	ARGS "PATH=$<TARGET_FILE_DIR:TFELMaterial>;%PATH%"
	COMMAND "set"
	ARGS "PATH=$<TARGET_FILE_DIR:TFELMathParser>;%PATH%"
	COMMAND "set"
	ARGS "PATH=$<TARGET_FILE_DIR:TFELGlossary>;%PATH%"
	COMMAND "set"
	ARGS "PATH=$<TARGET_FILE_DIR:TFELSystem>;%PATH%"
	COMMAND "set"
	ARGS "PATH=$<TARGET_FILE_DIR:TFELUtilities>;%PATH%"
	COMMAND "set"
	ARGS "PATH=$<TARGET_FILE_DIR:TFELException>;%PATH%"
	COMMAND "${mfront_executable}"
	ARGS    "${mfront_flags}" "--interface=mfront" "${PROJECT_SOURCE_DIR}/mfront/tests/properties/${source}.mfront"
	DEPENDS "${PROJECT_BINARY_DIR}/mfront/src/mfront"
	DEPENDS "${mfront_file}"
	COMMENT "treating mfront source ${source}.mfront")
    else((CMAKE_HOST_WIN32) AND (NOT MSYS))
      add_custom_command(
	OUTPUT  "src/${source}-mfront.cxx"
	COMMAND "${mfront_executable}"
	ARGS    "${mfront_flags}" "--interface=mfront" "${PROJECT_SOURCE_DIR}/mfront/tests/properties/${source}.mfront"
	DEPENDS "${PROJECT_BINARY_DIR}/mfront/src/mfront"
	DEPENDS "${mfront_file}"
	COMMENT "treating mfront source ${source}.mfront")
      # add_test(NAME mfront-${source}-mfront COMMAND
      # 	COMMAND "${mfront_executable}"
      # 	"${mfront_flags}" "--interface=mfront" "${PROJECT_SOURCE_DIR}/mfront/tests/properties/${source}.mfront"
      # 	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generation-test")
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
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
  add_library(${lib} MODULE EXCLUDE_FROM_ALL
    ${${lib}_SOURCES}
    ${${lib}_ADDITIONAL_SOURCES})
  set_target_properties(${lib} PROPERTIES
      COMPILE_FLAGS "-DMFRONT_COMPILING")
  add_dependencies(check ${lib})
endmacro(mfront_behaviour_check_library)

macro(python_module_base fullname name)
    if(${ARGC} LESS 1)
    message(FATAL_ERROR "python_lib_module : no source specified")
  endif(${ARGC} LESS 1)
  add_library(py_${fullname} MODULE ${ARGN})
  if(WIN32)
    set_target_properties(py_${fullname} PROPERTIES
      COMPILE_FLAGS "-DHAVE_ROUND")
    set_target_properties(py_${fullname} PROPERTIES SUFFIX ".pyd")
  elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(py_${fullname} PROPERTIES SUFFIX ".so")
  endif(WIN32)
  set_target_properties(py_${fullname} PROPERTIES PREFIX "")
  set_target_properties(py_${fullname} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(py_${fullname}
    ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARY})
endmacro(python_module_base)

macro(python_module name)
  python_module_base(${name} ${name} ${ARGN})
  install(TARGETS py_${name}
    DESTINATION lib${LIB_SUFFIX}/${PYTHON_LIBRARY}/site-packages/
    COMPONENT python_bindings)
endmacro(python_module)

macro(python_lib_module name package)
  python_module_base(${package}_${name} ${name} ${ARGN})
  install(TARGETS py_${package}_${name}
    DESTINATION lib${LIB_SUFFIX}/${PYTHON_LIBRARY}/site-packages/${package}
    COMPONENT python_bindings)
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

macro(mtest_python_module name)
  python_lib_module(${name} mtest ${ARGN})
endmacro(mtest_python_module)

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
