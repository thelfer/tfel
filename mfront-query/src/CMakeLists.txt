# mfront-query
add_executable(mfront-query mfront-query.cxx
  MaterialPropertyQuery.cxx
  BehaviourQuery.cxx
  ModelQuery.cxx)
target_include_directories(mfront-query 
  PRIVATE "${PROJECT_SOURCE_DIR}/mfront-query/include")
target_link_libraries(mfront-query
  TFELMFront 
  MFrontLogStream
  TFELSystem TFELUtilities)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(mfront-query
    PROPERTIES OUTPUT_NAME "mfront-query-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)
install(TARGETS mfront-query DESTINATION bin
  COMPONENT mfront)


