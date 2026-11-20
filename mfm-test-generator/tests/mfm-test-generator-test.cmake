# Generates the MTest or PTest file described by `input` with the mtest
# target of mfm-test-generator, appends the tests of the file `tests` to
# the generated file `generated_file` and runs the result, written in
# `output_file`, with mtest.
cmake_minimum_required(VERSION 3.20)
# `CROSSCOMPILING_EMULATOR` is empty except when cross-compiling, in which case
# `execute_process` cannot run the programs of the build tree directly: `cmake`
# itself is run on the host, so the emulator (`wine`) has to be used explicitly
execute_process(
  COMMAND ${CROSSCOMPILING_EMULATOR} "${mfm_test_generator}" --target=mtest
          "--@library@=\"${library}\"" "${input}"
  RESULT_VARIABLE result)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "mfm-test-generator failed on '${input}'")
endif()
file(READ "${generated_file}" generated)
file(READ "${tests}" checks)
file(WRITE "${output_file}" "${generated}\n${checks}")
execute_process(
  COMMAND ${CROSSCOMPILING_EMULATOR} "${mtest}" --verbose=level0 --xml-output=false
          --result-file-output=false "${output_file}"
  RESULT_VARIABLE result)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "mtest failed on '${output_file}'")
endif()
