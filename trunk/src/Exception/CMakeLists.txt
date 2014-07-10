include_directories("${PROJECT_SOURCE_DIR}/include")

set(TFELException_SRC TFELException.cxx)
tfel_library(TFELException ${TFELException_SRC})