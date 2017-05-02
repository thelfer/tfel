macro(mfront_query_test args mfront_file file)
  if(NOT MFRONT_QUERY_TEST_ID)
    set(MFRONT_QUERY_TEST_ID 0)
  endif(NOT MFRONT_QUERY_TEST_ID)
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(test "castem${test_arg}_${rm}_${conf}_mtest")
      set(file "castem${test_arg}-${rm}-${conf}.mtest")
      get_property(MFrontQueryPath TARGET mfront-query PROPERTY LOCATION_${conf})
      add_test(mfront-query-test-${MFRONT_QUERY_TEST_ID}_${conf}
	${CMAKE_COMMAND}
	-D TEST_PROGRAM=${MFrontQueryPath}
	-D TEST_ARGS=${args}
	-D MFRONT_FILE=${CMAKE_SOURCE_DIR}/mfront/tests/behaviours/${mfront_file}
	-D TEST_OUTPUT=mfront-query-result-${MFRONT_QUERY_TEST_ID}.txt
	-D REFERENCE_FILE=${CMAKE_CURRENT_SOURCE_DIR}/references/${file}
	-P ${CMAKE_CURRENT_SOURCE_DIR}/mfront-query.cmake )
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontQueryPath TARGET mfront-query PROPERTY LOCATION)
    add_test(mfront-query-test-${MFRONT_QUERY_TEST_ID}_${conf}
      ${CMAKE_COMMAND}
      -D TEST_PROGRAM=${MFrontQueryPath}
      -D TEST_ARGS=${args}
      -D MFRONT_FILE=${CMAKE_SOURCE_DIR}/mfront/tests/behaviours/${mfront_file}
      -D TEST_OUTPUT=mfront-query-result-${MFRONT_QUERY_TEST_ID}.txt
      -D REFERENCE_FILE=${CMAKE_CURRENT_SOURCE_DIR}/references/${file}
      -P ${CMAKE_CURRENT_SOURCE_DIR}/mfront-query.cmake )
  endif(CMAKE_CONFIGURATION_TYPES)
  # The previous definition with generators, which does not work !!
  #
  #   add_test(mfront-query-test-${MFRONT_QUERY_TEST_ID}
  #     ${CMAKE_COMMAND}
  #     -D CONFIG=$<CONFIG>
  #     -D TEST_PROGRAM=${mfront_query}
  # # ${PROJECT_BINARY_DIR}/mfront-query/src/mfront-query${CMAKE_EXECUTABLE_SUFFIX}
  #     -D TEST_ARGS=${args}
  #     -D MFRONT_FILE=${CMAKE_SOURCE_DIR}/mfront/tests/behaviours/${mfront_file}
  #     -D TEST_OUTPUT=mfront-query-result-${MFRONT_QUERY_TEST_ID}.txt
  #     -D REFERENCE_FILE=${CMAKE_CURRENT_SOURCE_DIR}/references/${file}
  #     -P ${CMAKE_CURRENT_SOURCE_DIR}/mfront-query.cmake )
  MATH(EXPR MFRONT_QUERY_TEST_ID "${MFRONT_QUERY_TEST_ID}+1")
endmacro(mfront_query_test)

mfront_query_test("--material-properties" "Norton.mfront"
  "mfront-query-norton-material-properites.ref")
# mfront_query_test("--state-variables" "Norton.mfront"
#   "mfront-query-norton-material-properites.ref")
