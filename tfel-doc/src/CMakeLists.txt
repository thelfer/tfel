include_directories("${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mtest/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${PROJECT_SOURCE_DIR}/tfel-doc/include")

link_directories("${PROJECT_BINARY_DIR}/mfront/mtest"
                 "${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
                 "${PROJECT_BINARY_DIR}/src/Tests")

set(tfel-doc_SOURCES tfel-doc.cxx
		     TestDocMain.cxx
		     Global.cxx
		     ParserBase.cxx
		     ConfigParser.cxx
		     TestDocumentation.cxx
		     TestDocParser.cxx
		     MTestDocParser.cxx
		     LaTeXGenerator.cxx
		     MarkdownGenerator.cxx)

add_executable(tfel-doc ${tfel-doc_SOURCES})
set_property(TARGET tfel-doc PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(tfel-doc
                      TFELMTest
                      TFELSystem
		      TFELUtilities
		      TFELException)
if(TFEL_APPEND_VERSION)
  set_target_properties(tfel-doc
    PROPERTIES OUTPUT_NAME "tfel-doc-${TFEL_VERSION}")
endif(TFEL_APPEND_VERSION)
install(TARGETS tfel-doc DESTINATION bin
  COMPONENT core)
