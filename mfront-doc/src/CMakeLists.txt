include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/mfront/include"
                    "${PROJECT_SOURCE_DIR}/mfront-doc/include")
if(HAVE_CASTEM)
  if(CASTEHOME)
    include_directories("${CASTEMHOME}/include")
    include_directories("${CASTEMHOME}/include/c")
  endif(CASTEHOME)
endif(HAVE_CASTEM)

link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Glossary"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
		 "${PROJECT_BINARY_DIR}/src/Config")

# mfront-doc
add_executable(mfront-doc mfront-doc.cxx
  BehaviourDocumentationGenerator.cxx)
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
