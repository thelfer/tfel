include_directories("${PROJECT_SOURCE_DIR}/include"
		    "${PROJECT_SOURCE_DIR}/tfel-doc/include")

link_directories("${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

set(tfel-doc_SOURCES tfel-doc.cxx
		     TestDocMain.cxx
		     Global.cxx
		     ParserBase.cxx
		     ConfigParser.cxx
		     TestDocumentation.cxx
		     TestDocParser.cxx
		     LaTeXConvertion.cxx)

add_executable(tfel-doc ${tfel-doc_SOURCES})

target_link_libraries(tfel-doc
                      TFELSystem
		      TFELUtilities
		      TFELException)
