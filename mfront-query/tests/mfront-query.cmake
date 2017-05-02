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
execute_process(
  COMMAND ${TEST_PROGRAM} ${TEST_ARGS} ${MFRONT_FILE}
  OUTPUT_FILE ${TEST_OUTPUT}
  ERROR_VARIABLE TEST_ERROR
  RESULT_VARIABLE TEST_RESULT)

# if the return value is !=0 bail out
if(TEST_RESULT)
  message(FATAL_ERROR "Failed: Test program ${TEST_PROGRAM} exited != 0.\n${TEST_ERROR}")
endif(TEST_RESULT)

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
