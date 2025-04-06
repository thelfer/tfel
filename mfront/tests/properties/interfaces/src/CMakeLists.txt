if(NOT WIN32)
  add_library(PleiadesMaterialProperties-2.0 MODULE
    PleiadesMaterialPropertyInterface-2.0.cxx)
  target_include_directories(PleiadesMaterialProperties-2.0
    PRIVATE "${PROJECT_SOURCE_DIR}/mfront/tests/properties/interfaces/include")
  target_link_libraries(PleiadesMaterialProperties-2.0
    TFELMFront
    MFrontLogStream
    TFELSystem)
endif(NOT WIN32)
