include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${PROJECT_SOURCE_DIR}/mfront/tests/models/interfaces/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src")

if(NOT WIN32)
  add_library(LicosMFrontModel MODULE LicosModelInterface)
  target_link_libraries(LicosMFrontModel
    TFELMFront
    MFrontLogStream
    TFELSystem)
  add_library(GerminalMFrontModel SHARED
    MFrontPleiadesModelInterfaceBase
    MFrontGerminalModelInterface)
  target_link_libraries(GerminalMFrontModel
    TFELMFront
    MFrontLogStream
    TFELSystem)
  add_library(PleiadesModelInterface-2.0 SHARED
    PleiadesModelInterface-2.0.cxx)
  target_link_libraries(PleiadesModelInterface-2.0
    TFELMFront
    MFrontLogStream
    TFELSystem)
endif(NOT WIN32)
