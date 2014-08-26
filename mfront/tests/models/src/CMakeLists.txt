include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${PROJECT_SOURCE_DIR}/mfront/tests/models/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src")

if(NOT WIN32)
  add_library(LicosMFrontModel SHARED LicosModelInterface)
  target_link_libraries(LicosMFrontModel
    MFrontLogStream)
  add_library(GerminalMFrontModel SHARED
    MFrontPleiadesModelInterfaceBase
    MFrontGerminalModelInterface)
  target_link_libraries(GerminalMFrontModel
    MFrontLogStream)
endif(NOT WIN32)
