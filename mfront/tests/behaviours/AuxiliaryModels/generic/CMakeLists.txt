mfront_behaviour_brick_check_library(MFrontAuxiliaryModels
  AuxiliaryModels generic
  AuxiliaryModelTest1
  AuxiliaryModelTest2
  AuxiliaryModelTest3
  AuxiliaryModelTest4)

macro(generictest_auxiliary_model test_arg behaviour)
    set(_REFERENCE_FILE )
    if(NOT (${ARGC} EQUAL 0))
      set(_REFERENCE_FILE "${ARGN}")
    endif()
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
    foreach(rm ${IEEE754_ROUNDING_MODES})
      if(NOT _REFERENCE_FILE)
	add_test(NAME brick${test_arg}${behaviour}_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@library@="$<TARGET_FILE:MFrontAuxiliaryModels>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml" ${file})
      else(NOT _REFERENCE_FILE)
	add_test(NAME brick${test_arg}${behaviour}_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@library@="$<TARGET_FILE:MFrontAuxiliaryModels>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml"
	  --@reference_file@="${top_srcdir}/mfront/tests/behaviours/AuxiliaryModels/references/${_REFERENCE_FILE}" ${file})
      endif(NOT _REFERENCE_FILE)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST brick${test_arg}${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontAuxiliaryModels mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST brick${test_arg}${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontAuxiliaryModels mtest")
	set_property(TEST brick${test_arg}${behaviour}_consistenttangentoperator_${rm}_mtest
	  PROPERTY DEPENDS "MFrontAuxiliaryModel mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    endforeach(rm ${IEEE754_ROUNDING_MODES})
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/BehaviourVariable/generic"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront/tests/behaviours/BehaviourVariable/generic"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
endmacro(generictest_auxiliary_model)

generictest_auxiliary_model(auxiliary_model_test1 MFrontAuxiliaryModels auxiliary_model_test1.ref)
generictest_auxiliary_model(auxiliary_model_test2 MFrontAuxiliaryModels)
generictest_auxiliary_model(auxiliary_model_test3 MFrontAuxiliaryModels)
generictest_auxiliary_model(auxiliary_model_test4 MFrontAuxiliaryModels)
