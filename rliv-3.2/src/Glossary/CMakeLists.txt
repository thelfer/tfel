include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")

tfel_library(TFELGlossary
  GlossaryEntry.cxx
  Glossary.cxx)
target_link_libraries(TFELGlossary TFELUtilities)
