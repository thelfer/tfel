if(NOT WIN32)
  add_library(LicosMFrontModel MODULE LicosModelInterface)
  target_include_directories(LicosMFrontModel
    PRIVATE ${CMAKE_SOURCE_DIR}/mfront/tests/models/interfaces/include)
  target_link_libraries(LicosMFrontModel
    PRIVATE TFELMFront
    PRIVATE MFrontLogStream
    PRIVATE TFELSystem)
  add_library(GerminalMFrontModel MODULE
    MFrontPleiadesModelInterfaceBase
    MFrontGerminalModelInterface)
  target_include_directories(GerminalMFrontModel
    PRIVATE ${CMAKE_SOURCE_DIR}/mfront/tests/models/interfaces/include)
  target_link_libraries(GerminalMFrontModel
    PRIVATE TFELMFront
    PRIVATE MFrontLogStream
    PRIVATE TFELSystem)
  add_library(PleiadesModelInterface-2.0
    PleiadesModelInterface-2.0.cxx)
  target_include_directories(PleiadesModelInterface-2.0
    PRIVATE ${CMAKE_SOURCE_DIR}/mfront/tests/models/interfaces/include)
  target_link_libraries(PleiadesModelInterface-2.0
    PRIVATE TFELMFront
    PRIVATE MFrontLogStream
    PRIVATE TFELSystem)
endif(NOT WIN32)
