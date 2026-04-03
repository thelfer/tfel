include(CMakePackageConfigHelpers)

# function add the given definition to the C and C++ preprocessor
function(tfel_add_c_cxx_definitions define)
  add_compile_definitions("$<$<COMPILE_LANGUAGE:C,CXX>:${define}>")
endfunction(tfel_add_c_cxx_definitions)

macro(tfel_project tfel_version_major tfel_version_minor tfel_version_patch)
  set(VERSION "${tfel_version_major}.${tfel_version_minor}.${tfel_version_patch}")
  project("tfel"
           HOMEPAGE_URL "https://thelfer.github.io/tfel/web/index.html"
           LANGUAGES C CXX
           VERSION "${VERSION}")
  set(PACKAGE_NAME "tfel")

  if(TFEL_APPEND_VERSION OR TFEL_VERSION_FLAVOUR)
    set(TFEL_APPEND_SUFFIX ON)
    tfel_add_c_cxx_definitions("TFEL_APPEND_SUFFIX")
  endif(TFEL_APPEND_VERSION OR TFEL_VERSION_FLAVOUR)

  set(TFEL_WEBSITE "http://thelfer.github.io/tfel/web/index.html")
  # the version number.
  set(TFEL_VERSION_MAJOR "${tfel_version_major}")
  set(TFEL_VERSION_MINOR "${tfel_version_minor}")
  set(TFEL_VERSION_PATCH "${tfel_version_patch}")

  set(TFEL_VERSION "${VERSION}")
  if(TFEL_DEVELOPMENT_VERSION)
    set(TFEL_VERSION "${VERSION}-dev")
  endif(TFEL_DEVELOPMENT_VERSION)

  if(TFEL_VERSION_FLAVOUR)
    set(TFEL_VERSION "${TFEL_VERSION}-${TFEL_VERSION_FLAVOUR}")
  else(TFEL_VERSION_FLAVOUR)
  endif(TFEL_VERSION_FLAVOUR)
  tfel_add_c_cxx_definitions("VERSION=\"${TFEL_VERSION}\"")
  
  if(TFEL_APPEND_VERSION)
    set(TFEL_SUFFIX "${TFEL_VERSION}")
    tfel_add_c_cxx_definitions("TFEL_SUFFIX=\"${TFEL_SUFFIX}\"")
  else(TFEL_APPEND_VERSION)
    if(TFEL_VERSION_FLAVOUR)
      set(TFEL_SUFFIX "${TFEL_VERSION_FLAVOUR}")
      tfel_add_c_cxx_definitions("TFEL_SUFFIX=\"${TFEL_SUFFIX}\"")
    endif(TFEL_VERSION_FLAVOUR)
  endif(TFEL_APPEND_VERSION)
  
  if(LIB_SUFFIX)
    tfel_add_c_cxx_definitions("LIB_SUFFIX=\"${LIB_SUFFIX}\"")
  endif(LIB_SUFFIX)

  write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/tfel-config-version.cmake"
    VERSION "${VERSION}"
    COMPATIBILITY SameMinorVersion)
  if(TFEL_APPEND_SUFFIX)
    set(export_install_path "share/tfel-${TFEL_SUFFIX}/cmake")
  else(TFEL_APPEND_SUFFIX)
    set(export_install_path "share/tfel/cmake")
  endif(TFEL_APPEND_SUFFIX)
  install(FILES "${PROJECT_BINARY_DIR}/tfel-config-version.cmake"
    DESTINATION "${export_install_path}")
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
  add_library(tfel::${name} ALIAS ${name})
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
          EXPORT_LINK_INTERFACE_LIBRARIES
          NAMESPACE tfel:: FILE ${name}Targets.cmake)
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${name}Config.cmake.in)
    set(_package_config_file ${CMAKE_CURRENT_SOURCE_DIR}/${name}Config.cmake.in)
  else()
    set(_package_config_file ${CMAKE_CURRENT_BINARY_DIR}/${name}Config.cmake.in)
    file(WRITE ${_package_config_file}
         "@PACKAGE_INIT@\n"
         "\n"
         "include(\"\${CMAKE_CURRENT_LIST_DIR}/${name}Targets.cmake\")")
  endif()
  # generate the config file that includes the exports
  configure_package_config_file(${_package_config_file}
                                "${CMAKE_CURRENT_BINARY_DIR}/${name}Config.cmake"
                                INSTALL_DESTINATION ${export_install_path}
                                NO_SET_AND_CHECK_MACRO
                                NO_CHECK_REQUIRED_COMPONENTS_MACRO)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${name}Config.cmake"
          DESTINATION ${export_install_path})
  if(enable-static)
    add_library(${name}-static STATIC ${ARGN})
    add_library(tfel::${name}-static ALIAS ${name}-static)
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
            NAMESPACE tfel:: FILE ${name}StaticTargets.cmake
            EXPORT_LINK_INTERFACE_LIBRARIES)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${name}StaticConfig.cmake.in)
      set(_package_config_file ${CMAKE_CURRENT_SOURCE_DIR}/${name}StaticConfig.cmake.in)
    else()
      set(_package_config_file ${CMAKE_CURRENT_BINARY_DIR}/${name}Config.cmake.in)
      file(WRITE ${_package_config_file}
           "@PACKAGE_INIT@\n"
           "\n"
          "include(\"\${CMAKE_CURRENT_LIST_DIR}/${name}StaticTargets.cmake\")")
    endif()
    # generate the config file that includes the exports
    configure_package_config_file(${_package_config_file}
                                  "${CMAKE_CURRENT_BINARY_DIR}/${name}StaticConfig.cmake"
                                  INSTALL_DESTINATION ${export_install_path}
                                  NO_SET_AND_CHECK_MACRO
                                  NO_CHECK_REQUIRED_COMPONENTS_MACRO)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${name}StaticConfig.cmake"
            DESTINATION ${export_install_path})
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
  set(mfront_executable "$<TARGET_FILE:mfront>")
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
      ARGS "PATH=\
$<TARGET_FILE_DIR:TFELMFront>;\
$<TARGET_FILE_DIR:MFrontLogStream>;\
$<TARGET_FILE_DIR:TFELMaterial>;\
$<TARGET_FILE_DIR:TFELNUMODIS>;\
$<TARGET_FILE_DIR:TFELMathParser>;\
$<TARGET_FILE_DIR:TFELGlossary>;\
$<TARGET_FILE_DIR:TFELSystem>;\
$<TARGET_FILE_DIR:TFELUtilities>;\
$<TARGET_FILE_DIR:TFELException>;\
$<TARGET_FILE_DIR:TFELConfig>;\
$<TARGET_FILE_DIR:TFELUnicodeSupport>;\
%PATH%"
      COMMAND "${mfront_executable}"
      ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/${dir}"
      ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/models"
      ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/behaviours"
      ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties"
      ARGS    "${mfront_flags}" "--interface=${interface}" "${mfront_file}"
      DEPENDS mfront
      DEPENDS "${mfront_file}"
      COMMENT "treating mfront source ${file}.mfront")
    else((CMAKE_HOST_WIN32) AND (NOT MSYS))
      add_custom_command(
    	OUTPUT  ${output_files}
	COMMAND "${mfront_executable}"
        ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/${dir}"
	ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/models"
	ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/behaviours"
	ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties"
	ARGS    "${mfront_flags}" "--interface=${interface}" "${mfront_file}"
	DEPENDS mfront
	DEPENDS "${mfront_file}"
	COMMENT "treating mfront source ${file}.mfront")
      file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generation-test)
      if(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
	add_test(NAME mfront-${file}-${interface}
	  COMMAND ${mfront_executable}
	  --search-path=${PROJECT_SOURCE_DIR}/mfront/tests/${dir}
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
    set(mfront_executable "$<TARGET_FILE:mfront>")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(mfront_flags "--debug")
    else(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(mfront_flags "")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      add_custom_command(
	OUTPUT  "src/${source}-mfront.cxx"
	COMMAND "set"
	ARGS "PATH=\
$<TARGET_FILE_DIR:TFELMFront>;\
$<TARGET_FILE_DIR:MFrontLogStream>;\
$<TARGET_FILE_DIR:TFELMaterial>;\
$<TARGET_FILE_DIR:TFELNUMODIS>;\
$<TARGET_FILE_DIR:TFELMathParser>;\
$<TARGET_FILE_DIR:TFELGlossary>;\
$<TARGET_FILE_DIR:TFELSystem>;\
$<TARGET_FILE_DIR:TFELUtilities>;\
$<TARGET_FILE_DIR:TFELException>;\
$<TARGET_FILE_DIR:TFELConfig>;\
$<TARGET_FILE_DIR:TFELUnicodeSupport>;\
%PATH%"
	COMMAND "${mfront_executable}"
	ARGS    "${mfront_flags}" "--interface=mfront" "${PROJECT_SOURCE_DIR}/mfront/tests/properties/${source}.mfront"
	DEPENDS mfront
	DEPENDS "${mfront_file}"
	COMMENT "treating mfront source ${source}.mfront")
    else((CMAKE_HOST_WIN32) AND (NOT MSYS))
      add_custom_command(
	OUTPUT  "src/${source}-mfront.cxx"
	COMMAND "${mfront_executable}"
	ARGS    "${mfront_flags}" "--interface=mfront" "${PROJECT_SOURCE_DIR}/mfront/tests/properties/${source}.mfront"
	DEPENDS mfront
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
  mfront_check_library_base(${lib} ${interface} "behaviours/${dir}" ON ${ARGN})
endfunction(mfront_behaviour_brick_check_library)

function(mfront_model_check_library lib interface)
  mfront_check_library_base(${lib} ${interface} "models" OFF ${ARGN})
endfunction(mfront_model_check_library)

