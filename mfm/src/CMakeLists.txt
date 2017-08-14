include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
		 "${PROJECT_BINARY_DIR}/src/Config")

add_executable(mfm mfm.cxx)
target_link_libraries(mfm
  TFELSystem TFELUtilities)
if(TFEL_APPEND_VERSION)
  set_target_properties(mfm
    PROPERTIES OUTPUT_NAME "mfm-${TFEL_VERSION}")
endif(TFEL_APPEND_VERSION)
install(TARGETS mfm DESTINATION bin
    COMPONENT mfront)
