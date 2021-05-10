if(HAVE_CASTEM)
  mfront_behaviour_brick_check_library(MFrontCastemDDIF2BrickBehaviours
    DDIF2 umat
    DDIF2 DDIF2_2 DDIF2_3 DDIF2_4 DDIF2_5 DDIF2_6)
  target_link_libraries(MFrontCastemDDIF2BrickBehaviours
    PRIVATE CastemInterface)
endif(HAVE_CASTEM)

macro(castemtest_ddif2brick test_arg behaviour)
  if(HAVE_CASTEM)
    set(_REFERENCE_FILE )
    if(NOT (${ARGC} EQUAL 0))
      set(_REFERENCE_FILE "${ARGN}")
    endif()
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
    foreach(rm ${IEEE754_ROUNDING_MODES})
      if(NOT _REFERENCE_FILE)
	add_test(NAME brick${test_arg}_${behaviour}_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@library@="$<TARGET_FILE:MFrontCastemDDIF2BrickBehaviours>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml" ${file})
	add_test(NAME brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@StiffnessMatrixType='ConsistentTangentOperator'
	  --@library@="$<TARGET_FILE:MFrontCastemDDIF2BrickBehaviours>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-consistenttangentoperator-${rm}.xml" ${file})
      else(NOT _REFERENCE_FILE)
	add_test(NAME brick${test_arg}_${behaviour}_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@library@="$<TARGET_FILE:MFrontCastemDDIF2BrickBehaviours>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml"
	  --@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}" ${file})
	add_test(NAME brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@StiffnessMatrixType='ConsistentTangentOperator'
	  --@library@="$<TARGET_FILE:MFrontCastemDDIF2BrickBehaviours>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-consistenttangentoperator-${rm}.xml"
	  --@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}" ${file})
      endif(NOT _REFERENCE_FILE)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST brick${test_arg}_${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemDDIF2BrickBehaviours MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
	set_property(TEST brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemDDIF2BrickBehaviours MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST brick${test_arg}_${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemDDIF2BrickBehaviours MFrontMaterialProperties-castem mtest")
	set_property(TEST brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemDDIF2BrickBehaviours MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    endforeach(rm ${IEEE754_ROUNDING_MODES})
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/bricks/DDIF2/castem"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront/tests/behaviours/bricks/DDIF2/castem"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endif(HAVE_CASTEM)
endmacro(castemtest_ddif2brick)

castemtest_ddif2brick(ddif2   umatddif2   ddif2-uniaxialtesting.ref)
castemtest_ddif2brick(ddif2_2 umatddif2_2 ddif2-uniaxialtesting.ref)
castemtest_ddif2brick(ddif2_3 umatddif2_3 ddif2-uniaxialtesting.ref)
castemtest_ddif2brick(ddif2_4 umatddif2_4 ddif2-uniaxialtesting.ref)
castemtest_ddif2brick(ddif2_5 umatddif2_5 ddif2-uniaxialtesting.ref)
castemtest_ddif2brick(ddif2_6 umatddif2_6 ddif2-uniaxialtesting.ref)
