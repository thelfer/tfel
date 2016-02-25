macro(mfront_unit_test file)
  set(mfront_file   "${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/unit-tests/${file}.mfront")
  if(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
    set(mfront_executable "$<TARGET_FILE:mfront>")
  else(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
    # retrieve the old behaviour for debian squeeze's version of cmake
    # does not work with configurations
    if(WIN32)
      set(mfront_executable "${PROJECT_BINARY_DIR}/mfront/src/mfront.exe")
    else(WIN32)
      set(mfront_executable "${PROJECT_BINARY_DIR}/mfront/src/mfront")
    endif(WIN32)
  endif(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
  add_test(NAME mfront-unit-tests-${file}
    COMMAND ${mfront_executable}
    --terminate-handler --no-gui
    --search-path=${PROJECT_SOURCE_DIR}/mfront/tests/behaviours
    --search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties
    ${mfront_file})
  set_tests_properties(mfront-unit-tests-${file}
    PROPERTIES WILL_FAIL true)
endmacro(mfront_unit_test file)

mfront_unit_test(ComputeThermalExpansion)
mfront_unit_test(ComputeThermalExpansion2)
mfront_unit_test(ComputeThermalExpansion3)