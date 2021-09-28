include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/tfel-check/include")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities"
  "${PROJECT_BINARY_DIR}/src/System"
  "${PROJECT_BINARY_DIR}/src/Math")

tfel_library(TFELCheck PCILogDriver.cxx
  PCJUnitDriver.cxx
  PCLogger.cxx
  PCTextDriver.cxx
  TestLauncher.cxx
  TestLauncherV1.cxx
  SplineInterpolation.cxx
  SplineLocalInterpolation.cxx
  Interpolation.cxx
  LinearInterpolation.cxx
  Linearization.cxx
  NoInterpolation.cxx
  Column.cxx
  Comparison.cxx
  AbsoluteComparison.cxx
  RelativeComparison.cxx
  RelativeAndAbsoluteComparison.cxx
  MixedComparison.cxx
  AreaComparison.cxx
  Test.cxx)

target_link_libraries(TFELCheck
  TFELMathCubicSpline TFELMathParser
  TFELMath TFELUtilities TFELSystem)

add_executable(tfel-check tfel-check.cxx)
target_link_libraries(tfel-check
  TFELCheck TFELUtilities TFELSystem)
if(TFEL_APPEND_VERSION)
  set_target_properties(tfel-check
    PROPERTIES OUTPUT_NAME "tfel-check-${TFEL_VERSION}")
endif(TFEL_APPEND_VERSION)
install(TARGETS tfel-check DESTINATION bin
  COMPONENT core)
