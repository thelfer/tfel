macro(mfmtg_library name)
  tfel_library_internal(${name} mfm ${ARGN})
endmacro(mfmtg_library)

mfmtg_library(MFMTestGenerator
  Log.cxx
  BehaviourData.cxx
  AbstractTestCase.cxx
  TestCaseBase.cxx
  AbstractTestCaseFactory.cxx
  InputFileGeneratorFactory.cxx
  TestCaseParameters.cxx
  UniaxialTensileTest.cxx
  ClosedPipeTest.cxx
  MTestInputFileGenerators.cxx
  LicosInputFileGenerators.cxx
  MFMTestGeneratorFileExport.cxx)
target_include_directories(MFMTestGenerator
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfm-test-generator/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(MFMTestGenerator
  PUBLIC
  MFrontLogStream
  TFELMTest TFELMaterial
  TFELSystem TFELUtilities)

if(TFEL_HAVE_MADNEX)
  target_compile_definitions(MFMTestGenerator
    PUBLIC MFM_TEST_GENERATOR_HAVE_MADNEX)
  target_link_libraries(MFMTestGenerator
    PUBLIC madnex::Madnex)
endif(TFEL_HAVE_MADNEX)

add_executable(mfm-test-generator mfm-test-generator.cxx)
set_property(TARGET mfm-test-generator PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mfm-test-generator
 PRIVATE
 MFMTestGenerator)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(mfm-test-generator
    PROPERTIES OUTPUT_NAME "mfm-test-generator-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)
install(TARGETS mfm-test-generator DESTINATION bin COMPONENT mfm)
