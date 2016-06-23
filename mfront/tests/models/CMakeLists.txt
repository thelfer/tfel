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
	OUTPUT  "include/MFront/${source}.hxx"
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
	COMMAND "set"
	ARGS "PATH=$<TARGET_FILE_DIR:TFELConfig>;%PATH%"
	COMMAND "${mfront_executable}"
	ARGS    "${mfront_flags}" "--interface=mfront" "${mfront_source}"
	DEPENDS "${PROJECT_BINARY_DIR}/mfront/src/mfront"
	DEPENDS "${mfront_source}"
	COMMENT "treating mfront source ${source}.mfront")
    else((CMAKE_HOST_WIN32) AND (NOT MSYS))
      add_custom_command(
	OUTPUT  "include/MFront/${source}.hxx"
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
  endforeach(source)
endmacro(mfront_behaviour_check_library)

mfront_model(B4C_ConcentrationModel
  SiC_IrradiationSwellingModel_GoFaster
  UO2_Shrinkage_RAPHAEL2008
  FastNeutronFluence
  NeutronFluence)

# set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/MFront/
# PROPERTIES HEADER_FILE_ONLY TRUE)
#       SET_SOURCE_FILES_PROPERTIES(${TMP_FILENAME}.cxx PROPERTIES
# OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${TMP_FILENAME}CLP.h)