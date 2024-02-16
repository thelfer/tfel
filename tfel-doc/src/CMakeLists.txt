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
target_include_directories(tfel-doc 
  PRIVATE "${PROJECT_SOURCE_DIR}/tfel-doc/include")
target_link_libraries(tfel-doc
  TFELMTest
  TFELSystem
  TFELUtilities
  TFELException)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(tfel-doc
    PROPERTIES OUTPUT_NAME "tfel-doc-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)
install(TARGETS tfel-doc DESTINATION bin COMPONENT core)
