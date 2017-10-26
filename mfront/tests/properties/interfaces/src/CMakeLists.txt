include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${PROJECT_SOURCE_DIR}/mfront/tests/properties/interfaces/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src")

if(NOT WIN32)
  add_library(PleiadesMaterialProperties-2.0 MODULE
    PleiadesMaterialPropertyInterface-2.0.cxx)
  target_link_libraries(PleiadesMaterialProperties-2.0
    TFELMFront
    MFrontLogStream
    TFELSystem)
endif(NOT WIN32)
