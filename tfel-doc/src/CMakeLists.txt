include_directories("${PROJECT_SOURCE_DIR}/include"
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
target_link_libraries(tfel-doc
                      TFELMTest
                      TFELSystem
		      TFELUtilities
		      TFELException)
install(TARGETS tfel-doc DESTINATION bin
  COMPONENT core)
