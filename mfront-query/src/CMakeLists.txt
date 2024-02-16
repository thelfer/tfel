# mfront-query
add_executable(mfront-query mfront-query.cxx
  QueryUtilities.cxx
  QueryHandlerBase.cxx
  MaterialPropertyQuery.cxx
  BehaviourQuery.cxx
  ModelQuery.cxx)
set_property(TARGET mfront-query PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_include_directories(mfront-query 
  PRIVATE "${PROJECT_SOURCE_DIR}/mfront-query/include")
target_link_libraries(mfront-query
  PUBLIC TFELMFront MFrontLogStream TFELSystem TFELUtilities)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(mfront-query
    PROPERTIES OUTPUT_NAME "mfront-query-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)

if(TFEL_HAVE_MADNEX)
  target_compile_definitions(mfront-query
    PUBLIC MFRONT_QUERY_HAVE_MADNEX)
  target_link_libraries(mfront-query
    PRIVATE madnex::Madnex)
endif(TFEL_HAVE_MADNEX)

install(TARGETS mfront-query DESTINATION bin
  COMPONENT mfront)


