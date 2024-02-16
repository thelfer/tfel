tfel_library(TFELCheck
  Configuration.cxx
  ConfigurationManager.cxx
  PCILogDriver.cxx
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
target_include_directories(TFELUtilities
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/tfel-check/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(TFELCheck
  TFELMathCubicSpline TFELMathParser
  TFELMath TFELUtilities TFELSystem
  TFELConfig)

add_executable(tfel-check tfel-check.cxx)
set_property(TARGET tfel-check PROPERTY POSITION_INDEPENDENT_CODE TRUE)
if(enable-python OR enable-python-bindings)
  target_compile_definitions(tfel-check
    PRIVATE TFEL_PYTHON_EXECUTABLE="${PYTHON_EXECUTABLE}")
endif(enable-python OR enable-python-bindings)

target_link_libraries(tfel-check
  TFELMFront TFELCheck TFELUtilities TFELSystem)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(tfel-check
    PROPERTIES OUTPUT_NAME "tfel-check-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)

install(TARGETS tfel-check DESTINATION bin
  COMPONENT core)

if(TFEL_HAVE_MADNEX)
  target_compile_definitions(tfel-check
    PUBLIC TFEL_HAVE_MADNEX)
endif(TFEL_HAVE_MADNEX)
