add_subdirectory(interfaces)

macro(mfront_model)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "mfront_model: no source specified")
  endif(${ARGC} LESS 1)
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
  foreach(source ${ARGN})
    set(mfront_source "${PROJECT_SOURCE_DIR}/mfront/tests/models/${source}.mfront")
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      add_custom_command(
	OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/include/MFront/${source}.hxx"
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
	ARGS "PATH=$<TARGET_FILE_DIR:TFELPhysicalConstants>;%PATH%"
	COMMAND "set"
	ARGS "PATH=$<TARGET_FILE_DIR:TFELUnicodeSupport>;%PATH%"
	COMMAND "${mfront_executable}"
	ARGS    "${mfront_flags}" "--interface=mfront" "${mfront_source}"
	DEPENDS "${PROJECT_BINARY_DIR}/mfront/src/mfront"
	DEPENDS "${mfront_source}"
	COMMENT "treating mfront source ${source}.mfront")
    else((CMAKE_HOST_WIN32) AND (NOT MSYS))
      add_custom_command(
	OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/include/MFront/${source}.hxx"
	COMMAND "${mfront_executable}"
	ARGS    "${mfront_flags}" "--interface=mfront" "${mfront_source}"
	DEPENDS "${PROJECT_BINARY_DIR}/mfront/src/mfront"
	DEPENDS "${mfront_source}"
	COMMENT "treating mfront source ${source}.mfront")
      file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generation-test)
      if(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
	add_test(NAME mfront-${source}-${interface}
	  COMMAND ${mfront_executable}
	  ${mfront_flags}
	  --interface=mfront ${mfront_source}
	  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generation-test")
      endif(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/include/MFront/${source}.hxx"
      PROPERTIES HEADER_FILE_ONLY TRUE)
  endforeach(source)
endmacro(mfront_model)

mfront_model(B4C_ConcentrationModel
  SiC_IrradiationSwellingModel_GoFaster
  UO2_Shrinkage_RAPHAEL2008
  Zircaloy4_AxialGrowth_BISON2014
  NickelAlloyPK33_ThermalExpansion_MechAlloy33
  FastNeutronFluence
  NeutronFluence)

# a simple macro
macro(tests_model test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
      PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMath>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_include_directories(${test_arg}
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
  target_link_libraries(${test_arg}
    TFELMaterial TFELMath TFELTests TFELPhysicalConstants)
endmacro(tests_model)

tests_model(test_model)
set_source_files_properties(test_model.cxx
  PROPERTIES OBJECT_DEPENDS
  "${CMAKE_CURRENT_BINARY_DIR}/include/MFront/B4C_ConcentrationModel.hxx;${CMAKE_CURRENT_BINARY_DIR}/include/MFront/SiC_IrradiationSwellingModel_GoFaster.hxx;${CMAKE_CURRENT_BINARY_DIR}/include/MFront/UO2_Shrinkage_RAPHAEL2008.hxx;${CMAKE_CURRENT_BINARY_DIR}/include/MFront/FastNeutronFluence.hxx;${CMAKE_CURRENT_BINARY_DIR}/include/MFront/NeutronFluence.hxx")
