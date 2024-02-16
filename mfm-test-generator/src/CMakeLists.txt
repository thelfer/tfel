include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/mfront/include"
                    "${PROJECT_SOURCE_DIR}/mtest/include"
                    "${PROJECT_SOURCE_DIR}/mfm-test-generator/include")
link_directories("${PROJECT_BINARY_DIR}/mtest/src"
                 "${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
		         "${PROJECT_BINARY_DIR}/src/Config")

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
target_link_libraries(MFMTestGenerator
  TFELMTest MFrontLogStream TFELMaterial
  TFELSystem TFELUtilities)

add_executable(mfm-test-generator mfm-test-generator.cxx)
set_property(TARGET mfm-test-generator PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mfm-test-generator MFMTestGenerator)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(mfm-test-generator
    PROPERTIES OUTPUT_NAME "mfm-test-generator-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)
install(TARGETS mfm-test-generator DESTINATION bin COMPONENT mfm)
