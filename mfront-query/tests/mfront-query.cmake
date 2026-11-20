# arguments checking
if(NOT TEST_PROGRAM)
  message(FATAL_ERROR "Require TEST_PROGRAM to be defined")
endif(NOT TEST_PROGRAM)
if(NOT TEST_ARGS)
  message(FATAL_ERROR "Require TEST_ARGS to be defined")
endif(NOT TEST_ARGS)
if(NOT MFRONT_FILE)
  message(FATAL_ERROR "Require MFRONT_FILE to be defined")
endif(NOT MFRONT_FILE)
if(NOT TEST_OUTPUT)
  message(FATAL_ERROR "Require TEST_OUTPUT to be defined")
endif(NOT TEST_OUTPUT)
if(NOT REFERENCE_FILE)
  message(FATAL_ERROR "Require REFERENCE_FILE to be defined")
endif(NOT REFERENCE_FILE)

# run the test program, capture the stdout/stderr and the result var
# `CROSSCOMPILING_EMULATOR` is empty except when cross-compiling, in which case
# `execute_process` cannot run the test program directly: `cmake` itself is run
# on the host, so the emulator (`wine`) has to be used explicitly
execute_process(
  COMMAND ${CROSSCOMPILING_EMULATOR} ${TEST_PROGRAM}
  --unicode-output=false
  --report-warnings=false
  ${TEST_ARGS} ${MFRONT_FILE}
  OUTPUT_FILE ${TEST_OUTPUT}
  ERROR_VARIABLE TEST_ERROR
  RESULT_VARIABLE TEST_RESULT)

# if the return value is !=0 bail out
if(TEST_RESULT)
  message(FATAL_ERROR "Failed: Test program ${TEST_PROGRAM} exited != 0.\n${TEST_ERROR}")
endif(TEST_RESULT)

# the line endings of the output are normalized: the output of the test program
# is written to a file, which uses the `\r\n` line endings under `Windows`,
# whereas the reference files always use the Unix ones, as stated by the
# `.gitattributes` file
file(READ ${TEST_OUTPUT} test_output)
string(REPLACE "\r\n" "\n" test_output "${test_output}")
file(WRITE ${TEST_OUTPUT} "${test_output}")

# now compare the output with the reference
execute_process(
  COMMAND ${CMAKE_COMMAND} -E compare_files ${TEST_OUTPUT} ${REFERENCE_FILE}
  RESULT_VARIABLE TEST_RESULT)

# again, if return value is !=0 scream and shout
if(TEST_RESULT)
  message(FATAL_ERROR "Failed: The output of ${TEST_PROGRAM} did not match ${REFERENCE_FILE}")
endif(TEST_RESULT)

# everything went fine...
message(STATUS "Passed: The output of ${TEST_PROGRAM} matches ${REFERENCE_FILE}")
