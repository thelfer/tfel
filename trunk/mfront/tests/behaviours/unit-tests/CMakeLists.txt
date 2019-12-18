macro(mfront_unit_test file)
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "test_castem: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
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
    --no-gui --search-path=${PROJECT_SOURCE_DIR}/mfront/tests/behaviours
    --search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties
    ${mfront_file})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST mfront-unit-tests-${file}
 	         APPEND PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMath>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELPhysicalConstants>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  if(_WILL_FAIL)
    set_tests_properties(mfront-unit-tests-${file}
      PROPERTIES WILL_FAIL true)
  endif(_WILL_FAIL)
endmacro(mfront_unit_test file)

mfront_unit_test(ComputeThermalExpansion  WILL_FAIL)
mfront_unit_test(ComputeThermalExpansion2 WILL_FAIL)
mfront_unit_test(ComputeThermalExpansion3 WILL_FAIL)
mfront_unit_test(ComputeThermalExpansion4 WILL_FAIL)
mfront_unit_test(ComputeThermalExpansion5)
mfront_unit_test(ComputeThermalExpansion6 WILL_FAIL)
mfront_unit_test(ComputeStiffnessTensor   WILL_FAIL)
mfront_unit_test(ComputeStiffnessTensor2  WILL_FAIL)
mfront_unit_test(ComputeStiffnessTensor3  WILL_FAIL)
