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
  LicosInputFileGenerators.cxx)
target_include_directories(MFMTestGenerator
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/mfm-test-generator/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(MFMTestGenerator
  TFELMTest MFrontLogStream TFELMaterial
  TFELSystem TFELUtilities)

add_executable(mfm-test-generator mfm-test-generator.cxx)
target_link_libraries(mfm-test-generator MFMTestGenerator)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(mfm-test-generator
    PROPERTIES OUTPUT_NAME "mfm-test-generator-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)
install(TARGETS mfm-test-generator DESTINATION bin COMPONENT mfm)
