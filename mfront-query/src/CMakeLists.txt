include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/mfront/include"
                    "${PROJECT_SOURCE_DIR}/mfront-query/include")
		  
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

# mfront-query
add_executable(mfront-query mfront-query.cxx
  MaterialPropertyQuery.cxx
  BehaviourQuery.cxx
  ModelQuery.cxx)
target_link_libraries(mfront-query
  TFELMFront 
  MFrontLogStream
  TFELSystem TFELUtilities)
install(TARGETS mfront-query DESTINATION bin
  COMPONENT mfront)


