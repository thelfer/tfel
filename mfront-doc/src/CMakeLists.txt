# mfront-doc
add_executable(mfront-doc mfront-doc.cxx DocumentationGeneratorBase.cxx
  BehaviourDocumentationGenerator.cxx MaterialPropertyDocumentationGenerator.cxx)
set_property(TARGET mfront-doc PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_include_directories(mfront-doc
  PRIVATE ${PROJECT_SOURCE_DIR}/mfront-doc/include)
target_link_libraries(mfront-doc
  TFELMFront 
  MFrontLogStream
  TFELSystem TFELUtilities)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(mfront-doc
    PROPERTIES OUTPUT_NAME "mfront-doc-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)
install(TARGETS mfront-doc DESTINATION bin
  COMPONENT mfront)
