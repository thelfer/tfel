macro(mfm_test args mfront_library file)
  if(NOT MFM_TEST_ID)
    set(MFM_TEST_ID 0)
  endif(NOT MFM_TEST_ID)
  set(test "mfm-test-${MFM_TEST_ID}")
  add_test(NAME ${test}
      COMMAND ${CMAKE_COMMAND}
      -D TEST_PROGRAM=$<TARGET_FILE:mfm>
      -D TEST_ARGS=${args}
      -D LIBRARY=${mfront_library}
      -D TEST_OUTPUT=mfm-result-${MFM_TEST_ID}.txt
      -D REFERENCE_FILE=${CMAKE_CURRENT_SOURCE_DIR}/references/${file}
      -P ${CMAKE_CURRENT_SOURCE_DIR}/mfm.cmake )
  set_property(TEST ${test} PROPERTY DEPENDS "mfm")
  # The previous definition with generators, which does not work !!
  #
  #   add_test(mfm-test-${MFM_TEST_ID}
  #     ${CMAKE_COMMAND}
  #     -D CONFIG=$<CONFIG>
  #     -D TEST_PROGRAM=${mfm}
  # # ${PROJECT_BINARY_DIR}/mfm/src/mfm${CMAKE_EXECUTABLE_SUFFIX}
  #     -D TEST_ARGS=${args}
  #     -D MFRONT_FILE=${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/${mfront_file}
  #     -D TEST_OUTPUT=mfm-result-${MFM_TEST_ID}.txt
  #     -D REFERENCE_FILE=${CMAKE_CURRENT_SOURCE_DIR}/references/${file}
  #     -P ${CMAKE_CURRENT_SOURCE_DIR}/mfm.cmake )
  MATH(EXPR MFM_TEST_ID "${MFM_TEST_ID}+1")
endmacro(mfm_test)

if(HAVE_CASTEM)
mfm_test("--filter-by-name=.+t91.+" $<TARGET_FILE:MFrontCastemBehaviours>
  "mfm-t91.ref")
endif(HAVE_CASTEM)
# mfm_test("--state-variables" "Norton.mfront"
#   "mfm-norton-material-properites.ref")
