include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${PROJECT_SOURCE_DIR}/mfront/tests/properties/interfaces/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src")

if(NOT WIN32)
  add_library(libPleiadesMaterialProperties-2.0 SHARED
    MFrontPleiadesLawInterface-2.0.cxx)
  target_link_libraries(libPleiadesMaterialProperties-2.0
    MFrontLogStream)
endif(NOT WIN32)
