configure_file(uniaxialtensiletest-times.txt
  uniaxialtensiletest-times.txt COPYONLY)

# Generates the MTest or PTest file of the given test case with the mtest
# target of mfm-test-generator, appends the tests of the file
# <test_case>-tests.<extension> and runs it with mtest.
function(mfm_test_generator_test test_case extension)
  # the input file is given by a path relative to the current binary
  # directory, since mfm-test-generator treats arguments starting with a
  # slash as options
  configure_file(${test_case}.mfmtg ${test_case}.mfmtg COPYONLY)
  add_test(NAME mfm-test-generator-${test_case}
    COMMAND ${CMAKE_COMMAND}
    "-Dmfm_test_generator=$<TARGET_FILE:mfm-test-generator>"
    "-Dmtest=$<TARGET_FILE:mtest>"
    "-Dlibrary=$<TARGET_FILE:MFrontGenericBehaviours>"
    "-Dinput=${test_case}.mfmtg"
    "-Dgenerated_file=${test_case}.${extension}"
    "-Dtests=${CMAKE_CURRENT_SOURCE_DIR}/${test_case}-tests.${extension}"
    "-Doutput_file=${test_case}-with-tests.${extension}"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/mfm-test-generator-test.cmake")
  # on native Windows, the executables of the build tree find the TFEL
  # libraries through PATH: mfm-test-generator and mtest, run by
  # execute_process, inherit the environment of the test
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST mfm-test-generator-${test_case}
      PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:MFMTestGenerator>\;$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
endfunction(mfm_test_generator_test)

mfm_test_generator_test(uniaxialtensiletest mtest)
mfm_test_generator_test(closedpipetest ptest)
