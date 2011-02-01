include_directories("${PROJECT_SOURCE_DIR}/include")

tfel_library(TFELTests
             Test.cxx
             TestResult.cxx
             TestSuite.cxx
             TestOutput.cxx
             StdStreamTestOutput.cxx
             XMLTestOutput.cxx
             MultipleTestOutputs.cxx
             TestManager.cxx
             TestCase.cxx)

target_link_libraries(TFELTests rt)
