if(NOT WIN32)
  add_library(LicosMFrontModel MODULE LicosModelInterface.cxx)
  target_include_directories(LicosMFrontModel
    PRIVATE ${PROJECT_SOURCE_DIR}/mfront/tests/models/interfaces/include)
  target_link_libraries(LicosMFrontModel
    PRIVATE TFELMFront
    PRIVATE MFrontLogStream
    PRIVATE TFELSystem)
  add_library(GerminalMFrontModel MODULE
    MFrontPleiadesModelInterfaceBase.cxx
    MFrontGerminalModelInterface.cxx)
  target_include_directories(GerminalMFrontModel
    PRIVATE ${PROJECT_SOURCE_DIR}/mfront/tests/models/interfaces/include)
  target_link_libraries(GerminalMFrontModel
    PRIVATE TFELMFront
    PRIVATE MFrontLogStream
    PRIVATE TFELSystem)
endif(NOT WIN32)
