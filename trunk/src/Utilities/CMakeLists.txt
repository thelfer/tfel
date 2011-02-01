include_directories("${PROJECT_SOURCE_DIR}/include")

set(TFELUtilities_SOURCES
    TextData.cxx
    TerminalColors.cxx
    GenTypeCastError.cxx
    Token.cxx
    CxxKeywords.cxx
    Name.cxx
    CxxTokenizer.cxx)

tfel_library(TFELUtilities ${TFELUtilities_SOURCES})
