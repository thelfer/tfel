add_executable(mfm mfm.cxx)
target_link_libraries(mfm
  tfel::TFELUtilities
  tfel::TFELMFrontDatabase)
set_property(TARGET mfm PROPERTY POSITION_INDEPENDENT_CODE TRUE)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(mfm
    PROPERTIES OUTPUT_NAME "mfm-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)
install(TARGETS mfm DESTINATION bin COMPONENT mfm)
