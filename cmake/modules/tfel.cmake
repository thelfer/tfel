macro(tfel_project tfel_version_major tfel_version_minor tfel_version_patch)
  project("tfel")
  set(PACKAGE_NAME "tfel")
  set(VERSION "${tfel_version_major}.${tfel_version_minor}.${tfel_version_patch}")

  if(TFEL_APPEND_VERSION OR TFEL_VERSION_FLAVOUR)
    set(TFEL_APPEND_SUFFIX ON)
    add_definitions("-DTFEL_APPEND_SUFFIX")
  endif(TFEL_APPEND_VERSION OR TFEL_VERSION_FLAVOUR)

  set(TFEL_WEBSITE "http://tfel.sourceforce.net")
  # the version number.
  set(TFEL_VERSION_MAJOR "${tfel_version_major}")
  set(TFEL_VERSION_MINOR "${tfel_version_minor}")
  set(TFEL_VERSION_PATCH "${tfel_version_patch}")

  set(TFEL_VERSION "${VERSION}")
  if(TFEL_VERSION_FLAVOUR)
    set(TFEL_VERSION "${VERSION}-${TFEL_VERSION_FLAVOUR}")
  else(TFEL_VERSION_FLAVOUR)
    set(TFEL_VERSION "${VERSION}")
  endif(TFEL_VERSION_FLAVOUR)
  add_definitions("-DVERSION=\\\"\"${TFEL_VERSION}\"\\\"")
  
  if(TFEL_APPEND_VERSION)
    set(TFEL_SUFFIX "${TFEL_VERSION}")
    add_definitions("-DTFEL_SUFFIX=\\\"\"${TFEL_SUFFIX}\"\\\"")
  else(TFEL_APPEND_VERSION)
    if(TFEL_VERSION_FLAVOUR)
      set(TFEL_SUFFIX "${TFEL_VERSION_FLAVOUR}")
      add_definitions("-DTFEL_SUFFIX=\\\"\"${TFEL_SUFFIX}\"\\\"")
    endif(TFEL_VERSION_FLAVOUR)
  endif(TFEL_APPEND_VERSION)

  if(TFEL_SUFFIX)
    string(REPLACE "." "_" TFEL_SUFFIX_FOR_PYTHON_MODULES "${TFEL_SUFFIX}")
    string(REPLACE "-" "_" TFEL_SUFFIX_FOR_PYTHON_MODULES "${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
  endif(TFEL_SUFFIX)
  
  if(LIB_SUFFIX)
    add_definitions("-DLIB_SUFFIX=\\\"\"${LIB_SUFFIX}\"\\\"")
  endif(LIB_SUFFIX)
endmacro(tfel_project)

set(CPACK_COMPONENTS_ALL core mfront mtest mfm)

set(CPACK_COMPONENT_CORE_DESCRIPTION 
  "Contains all the core libraries developped within TFEL")
set(CPACK_COMPONENT_MFRONT_DESCRIPTION
  "Contains the MFront code generator and the associated libraries")
set(CPACK_COMPONENT_MTESTS_DESCRIPTION
  "Contains the MTest tool")

set(CPACK_COMPONENT_MFRONT_DEPENDS core)
set(CPACK_COMPONENT_MTEST_DEPENDS  core mfront)

function(install_header dir file)
  if(TFEL_APPEND_SUFFIX)  
    install(FILES ${dir}/${file}
      DESTINATION "include/TFEL-${TFEL_SUFFIX}/${dir}"
      COMPONENT core)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${dir}/${file}
      DESTINATION "include/${dir}"
      COMPONENT core)
  endif(TFEL_APPEND_SUFFIX)
endfunction(install_header)

function(install_mfront_header dir file)
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${dir}/${file}
      DESTINATION "include/TFEL-${TFEL_SUFFIX}/${dir}"
      COMPONENT mfront)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${dir}/${file}
      DESTINATION "include/${dir}"
      COMPONENT mfront)
  endif(TFEL_APPEND_SUFFIX)
endfunction(install_mfront_header)

function(install_mtest_header dir file)
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${dir}/${file}
      DESTINATION "include/TFEL-${TFEL_SUFFIX}/${dir}"
      COMPONENT mtest)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${dir}/${file}
      DESTINATION "include/${dir}"
      COMPONENT mtest)
  endif(TFEL_APPEND_SUFFIX)
endfunction(install_mtest_header)

function(install_mfm_header dir file)
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${dir}/${file}
      DESTINATION "include/TFEL-${TFEL_SUFFIX}/${dir}"
      COMPONENT mfm)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${dir}/${file}
      DESTINATION "include/${dir}"
      COMPONENT mfm)
  endif(TFEL_APPEND_SUFFIX)
endfunction(install_mfm_header)

function(install_data dir file)
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/tfel-${TFEL_SUFFIX}/${dir}")
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/tfel/${dir}")
  endif(TFEL_APPEND_SUFFIX)
endfunction(install_data)

function(install_mfront_data dir file)
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/${dir}"
      COMPONENT mfront)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront/${dir}"
      COMPONENT mfront)
  endif(TFEL_APPEND_SUFFIX)
endfunction(install_mfront_data)

function(install_gallery dir file)
  install_mfront_data("gallery/${dir}" "${file}")
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/tfel-${TFEL_SUFFIX}/web/gallery/${dir}"
      COMPONENT web)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/tfel/web/gallery/${dir}"
      COMPONENT web)
  endif(TFEL_APPEND_SUFFIX)
endfunction(install_gallery)

function(tfel_library_internal name component)
  if(${ARGC} LESS 2)
    message(FATAL_ERROR "tfel_library_internal : no source specified")
  endif(${ARGC} LESS 2)
  add_library(${name} ${ARGN})
  if(TFEL_APPEND_SUFFIX)
    set(export_install_path "share/tfel-${TFEL_SUFFIX}/cmake")
  else(TFEL_APPEND_SUFFIX)
    set(export_install_path "share/tfel/cmake")
  endif(TFEL_APPEND_SUFFIX)
  if(TFEL_APPEND_SUFFIX)
    set_target_properties(${name} PROPERTIES OUTPUT_NAME "${name}-${TFEL_SUFFIX}")
  endif(TFEL_APPEND_SUFFIX)
  set_target_properties(${name}
    PROPERTIES VERSION ${TFEL_VERSION} SOVERSION ${TFEL_VERSION})
  if(enable-include-what-you-use)
    set_property(TARGET ${name}
      PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${TFEL_IWYU_PATH})
  endif(enable-include-what-you-use)
  if(WIN32)
    install(TARGETS ${name} EXPORT ${name} 
      DESTINATION bin
      COMPONENT ${component})
  else(WIN32)
    install(TARGETS ${name} EXPORT ${name}
      DESTINATION lib${LIB_SUFFIX}
      COMPONENT ${component})
  endif(WIN32)
  install(EXPORT ${name} DESTINATION ${export_install_path}
          NAMESPACE tfel:: FILE ${name}Config.cmake)
  if(enable-static)
    add_library(${name}-static STATIC ${ARGN})
    if(TFEL_APPEND_SUFFIX)
      set_target_properties(${name}-static PROPERTIES OUTPUT_NAME "${name}-{TFEL_SUFFIX}")
    else(TFEL_APPEND_SUFFIX)
      set_target_properties(${name}-static PROPERTIES OUTPUT_NAME "${name}")
    endif(TFEL_APPEND_SUFFIX)
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
      install(TARGETS ${name}-static EXPORT ${name}-static DESTINATION bin)
    else(WIN32)
      install(TARGETS ${name}-static EXPORT ${name}-static DESTINATION lib${LIB_SUFFIX})
    endif(WIN32)
    install(EXPORT ${name}-static DESTINATION ${export_install_path}
            NAMESPACE tfel:: FILE ${name}StaticConfig.cmake)
    target_compile_options (${name}-static PRIVATE "${TFEL_CXX_FLAGS}")
  endif(enable-static)
endfunction(tfel_library_internal)

function(tfel_library name)
  tfel_library_internal(${name} core ${ARGN})
endfunction(tfel_library)

function(mfront_library name)
  tfel_library_internal(${name} mfront ${ARGN})
endfunction(mfront_library)

function(mtest_library name)
  tfel_library_internal(${name} mtest ${ARGN})
endfunction(mtest_library)

macro(add_mfront_behaviour_generated_source lib interface dir intrinsic_source file)
  set(mfront_file "${PROJECT_SOURCE_DIR}/mfront/tests/${dir}/${file}.mfront")
  set(iprefix "")
  set(isuffix "")
  if("${interface}" STREQUAL "generic")
    set(isuffix "-generic")
  elseif("${interface}" STREQUAL "castem")
    set(iprefix "umat")
  elseif("${interface}" STREQUAL "castem21")
    set(iprefix "umat")
  elseif("${interface}" STREQUAL "dianafea")
    set(iprefix "DianaFEA")
  else ("${interface}" STREQUAL "generic")
    set(iprefix "${interface}")
  endif("${interface}" STREQUAL "generic")
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
  set(output_files)
  list(APPEND output_files  "src/${iprefix}${file}${isuffix}.cxx")
  if(${intrinsic_source} STREQUAL "ON")
    list(APPEND output_files  "src/${file}.cxx")
  endif(${intrinsic_source} STREQUAL "ON")
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    add_custom_command(
      OUTPUT  ${output_files}
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELMFront>;%PATH%"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:MFrontLogStream>;%PATH%"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELMaterial>;%PATH%"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELNUMODIS>;%PATH%"
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
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELConfig>;%PATH%"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELUnicodeSupport>;%PATH%"
      COMMAND "${mfront_executable}"
      ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/models"
      ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/behaviours"
      ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties"
      ARGS    "${mfront_flags}" "--interface=${interface}" "${mfront_file}"
      DEPENDS "${PROJECT_BINARY_DIR}/mfront/src/mfront"
      DEPENDS "${mfront_file}"
      COMMENT "treating mfront source ${file}.mfront")
    else((CMAKE_HOST_WIN32) AND (NOT MSYS))
      add_custom_command(
    	OUTPUT  ${output_files}
	COMMAND "${mfront_executable}"
	ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/models"
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
	  --search-path=${PROJECT_SOURCE_DIR}/mfront/tests/models
	  --search-path=${PROJECT_SOURCE_DIR}/mfront/tests/behaviours
	  --search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties
	  ${mfront_flags}
	  --interface=${interface} ${mfront_file}
	  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generation-test")
      endif(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  set(${lib}_SOURCES "src/${iprefix}${file}${isuffix}.cxx" ${${lib}_SOURCES})
  if(${intrinsic_source} STREQUAL "ON")
    set(${lib}_SOURCES "src/${file}.cxx" ${${lib}_SOURCES})
  endif(${intrinsic_source} STREQUAL "ON")
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
	ARGS "PATH=$<TARGET_FILE_DIR:TFELNUMODIS>;%PATH%"
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
    COMMAND "set"
    ARGS "PATH=$<TARGET_FILE_DIR:TFELConfig>;%PATH%"
    COMMAND "set"
    ARGS "PATH=$<TARGET_FILE_DIR:TFELUnicodeSupport>;%PATH%"
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

function(mfront_check_library_base lib interface directory intrinsic_sources)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "mfront_library : no source specified")
  endif(${ARGC} LESS 1)
  foreach(source ${ARGN})
    add_mfront_behaviour_generated_source(${lib} ${interface} ${directory}
                                          ${intrinsic_sources} ${source})
  endforeach(source)
  add_library(${lib} MODULE EXCLUDE_FROM_ALL
    ${${lib}_SOURCES}
    ${${lib}_ADDITIONAL_SOURCES})
  set_target_properties(${lib} PROPERTIES
      COMPILE_FLAGS "-DMFRONT_COMPILING")
  target_include_directories(${lib}
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
  target_link_libraries(${lib}
    PRIVATE MFrontProfiling
    PRIVATE TFELMaterial
    PRIVATE TFELMath
    PRIVATE TFELUtilities
    PRIVATE TFELException)
  add_dependencies(check ${lib})
endfunction(mfront_check_library_base)

function(mfront_behaviour_check_library lib interface)
  mfront_check_library_base(${lib} ${interface} "behaviours" ON ${ARGN})
endfunction(mfront_behaviour_check_library)

function(mfront_behaviour_brick_check_library lib dir interface)
  mfront_check_library_base(${lib} ${interface} "behaviours/bricks/${dir}" ON ${ARGN})
endfunction(mfront_behaviour_brick_check_library)

function(mfront_model_check_library lib interface)
  mfront_check_library_base(${lib} ${interface} "models" OFF ${ARGN})
endfunction(mfront_model_check_library)

function(python_module_base fullname name)
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
  target_include_directories(py_${fullname}
    PRIVATE "${PROJECT_SOURCE_DIR}/bindings/python/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/include")
  target_include_directories(py_${fullname}
    SYSTEM
    PRIVATE "${Boost_INCLUDE_DIRS}"
    PRIVATE "${PYTHON_INCLUDE_DIRS}")
  if(python-static-interpreter-workaround)
    if(APPLE)
      target_link_options(py_${fullname}
        PRIVATE "-undefined" "dynamic_lookup")
    endif(APPLE)
  endif(python-static-interpreter-workaround)
endfunction(python_module_base)

function(python_lib_module name package)
  python_module_base(${package}_${name} ${name} ${ARGN})
  if(TFEL_APPEND_SUFFIX)
    install(TARGETS py_${package}_${name}
      DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/${package}_${TFEL_SUFFIX_FOR_PYTHON_MODULES}
      COMPONENT python_bindings)
  else(TFEL_APPEND_SUFFIX)
    install(TARGETS py_${package}_${name}
       DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/${package}
       COMPONENT python_bindings)
  endif(TFEL_APPEND_SUFFIX)
endfunction(python_lib_module)

function(std_python_module name)
  python_lib_module(${name} std ${ARGN})
endfunction(std_python_module)

function(tfel_python_module name)
  python_lib_module(${name} tfel ${ARGN})
endfunction(tfel_python_module)

function(mfront_python_module name)
  python_lib_module(${name} mfront ${ARGN})
  set(fullname "mfront_${name}")
  target_include_directories(py_${fullname}
    PRIVATE "${PROJECT_SOURCE_DIR}/mfront/include")
endfunction(mfront_python_module)

function(mtest_python_module name)
  python_lib_module(${name} mtest ${ARGN})
  set(fullname "mtest_${name}")
  target_include_directories(py_${fullname}
    PRIVATE "${PROJECT_SOURCE_DIR}/mtest/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/mfront/include")
endfunction(mtest_python_module)

function(mfm_test_generator_python_module name)
  python_lib_module(${name} mfm_test_generator ${ARGN})
  set(fullname "mfm_test_generator_${name}")
  target_include_directories(py_${fullname}
    PRIVATE "${PROJECT_SOURCE_DIR}/mfm-test-generator/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/mtest/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/mfront/include")
endfunction(mfm_test_generator_python_module)

function(tfel_python_script_base dir)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "tfel_python_script_base : no script specified")
  endif(${ARGC} LESS 1)
  foreach(pyscript ${ARGN})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
      if(TFEL_APPEND_SUFFIX)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in")
	  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in"
	    "${CMAKE_CURRENT_BINARY_DIR}/${pyscript}" @ONLY)
	else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in")
	  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in"
	    "${CMAKE_CURRENT_BINARY_DIR}/${pyscript}" @ONLY)
	endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in")
      else(TFEL_APPEND_SUFFIX)
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in"
	  "${CMAKE_CURRENT_BINARY_DIR}/${pyscript}" @ONLY)
      endif(TFEL_APPEND_SUFFIX)
      set(python_script "${CMAKE_CURRENT_BINARY_DIR}/${pyscript}")
    else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
      set(python_script "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}")
    endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
    install(PROGRAMS ${python_script}
       DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/${dir}/
       COMPONENT python_bindings)
  endforeach(pyscript ${ARGN})
endfunction(tfel_python_script_base)

function(tfel_python_script dir)
  if(TFEL_APPEND_SUFFIX)
    tfel_python_script_base(${dir}_${TFEL_SUFFIX_FOR_PYTHON_MODULES} ${ARGN})
  else(TFEL_APPEND_SUFFIX)
    tfel_python_script_base(${dir} ${ARGN})
  endif(TFEL_APPEND_SUFFIX)
endfunction(tfel_python_script)
