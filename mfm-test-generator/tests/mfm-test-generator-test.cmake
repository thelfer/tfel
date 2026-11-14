# Generates the MTest or PTest file described by `input` with the mtest
# target of mfm-test-generator, appends the tests of the file `tests` to
# the generated file `generated_file` and runs the result, written in
# `output_file`, with mtest.
cmake_minimum_required(VERSION 3.20)
execute_process(
  COMMAND "${mfm_test_generator}" --target=mtest
          "--@library@=\"${library}\"" "${input}"
  RESULT_VARIABLE result)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "mfm-test-generator failed on '${input}'")
endif()
file(READ "${generated_file}" generated)
file(READ "${tests}" checks)
file(WRITE "${output_file}" "${generated}\n${checks}")
execute_process(
  COMMAND "${mtest}" --verbose=level0 --xml-output=false
          --result-file-output=false "${output_file}"
  RESULT_VARIABLE result)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "mtest failed on '${output_file}'")
endif()
