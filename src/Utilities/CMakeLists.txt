include_directories("${PROJECT_SOURCE_DIR}/include")

set(TFELUtilities_SOURCES
    StringAlgorithms.cxx
    TextData.cxx
    GenTypeCastError.cxx
    Token.cxx
    CxxKeywords.cxx
    TerminalColors.cxx
    CxxTokenizer.cxx)

tfel_library(TFELUtilities ${TFELUtilities_SOURCES})
