include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

if(HAVE_CASTEM)
  mfront_behaviour_brick_check_library(MFrontCastemStandardElastoViscoPlasticityBrickBehaviours
  StandardElastoViscoPlasticity umat
  StandardElastoViscoPlasticityTest1
  StandardElastoViscoPlasticityTest2
  StandardElastoViscoPlasticityTest3
  StandardElastoViscoPlasticityTest4
  StandardElastoViscoPlasticityTest5
  StandardElastoViscoPlasticityTest6
  StandardElastoViscoPlasticityNortonTest
  StandardElastoViscoPlasticityNortonTest_nj
  StandardElastoViscoPlasticityNortonTest2
  StandardElastoViscoPlasticityNortonTest2_nj
  StandardElastoViscoPlasticityNortonTest3
  StandardElastoViscoPlasticityNortonTest3_nj
  StandardElastoViscoPlasticityNortonTest4
  StandardElastoViscoPlasticityNortonTest4_nj
  StandardElastoViscoPlasticityNortonTest5
  StandardElastoViscoPlasticityNortonTest5_nj
  StandardElastoViscoPlasticityNortonTest6
  StandardElastoViscoPlasticityNortonTest6_nj
  StandardElastoViscoPlasticityNortonTest7
  StandardElastoViscoPlasticityNortonTest7_nj
  StandardElastoViscoPlasticityOrthotropicNortonTest
  StandardElastoViscoPlasticityOrthotropicNortonTest_nj
  StandardElastoViscoPlasticityPlasticityTest
  StandardElastoViscoPlasticityPlasticityTest_nj
  StandardElastoViscoPlasticityPlasticityTest2
  StandardElastoViscoPlasticityPlasticityTest2_nj
  StandardElastoViscoPlasticityPlasticityTest3
  StandardElastoViscoPlasticityPlasticityTest3_nj
  StandardElastoViscoPlasticityPlasticityTest4
  StandardElastoViscoPlasticityPlasticityTest4_nj
  StandardElastoViscoPlasticityPlasticityTest5
  StandardElastoViscoPlasticityPlasticityTest5_nj
  StandardElastoViscoPlasticityPlasticityTest6
  StandardElastoViscoPlasticityPlasticityTest6_nj
  StandardElastoViscoPlasticityPlasticityTest7
  StandardElastoViscoPlasticityPlasticityTest7_nj
  StandardElastoViscoPlasticityPlasticityTest8
  StandardElastoViscoPlasticityPlasticityTest8_nj)
  target_link_libraries(MFrontCastemStandardElastoViscoPlasticityBrickBehaviours
  CastemInterface MFrontProfiling TFELMaterial TFELMath TFELUtilities
  TFELException)
endif(HAVE_CASTEM)

macro(castemtest_standardelastovicoplasticitybrick test_arg behaviour)
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
      --@AccelerationAlgorithm='Cast3M'
	  --@library@="$<TARGET_FILE:MFrontCastemStandardElastoViscoPlasticityBrickBehaviours>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml" ${file})
	add_test(NAME brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@StiffnessMatrixType='ConsistentTangentOperator'
	  --@library@="$<TARGET_FILE:MFrontCastemStandardElastoViscoPlasticityBrickBehaviours>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-consistenttangentoperator-${rm}.xml" ${file})
      else(NOT _REFERENCE_FILE)
	add_test(NAME brick${test_arg}_${behaviour}_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@AccelerationAlgorithm='Cast3M'
	  --@library@="$<TARGET_FILE:MFrontCastemStandardElastoViscoPlasticityBrickBehaviours>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml"
	  --@reference_file@="${top_srcdir}/mfront/tests/behaviours/bricks/StandardElastoViscoPlasticity/references/${_REFERENCE_FILE}" ${file})
	add_test(NAME brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@StiffnessMatrixType='ConsistentTangentOperator'
	  --@library@="$<TARGET_FILE:MFrontCastemStandardElastoViscoPlasticityBrickBehaviours>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-consistenttangentoperator-${rm}.xml"
	  --@reference_file@="${top_srcdir}/mfront/tests/behaviours/bricks/StandardElastoViscoPlasticity/references/${_REFERENCE_FILE}" ${file})
      endif(NOT _REFERENCE_FILE)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST brick${test_arg}_${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemStandardElastoViscoPlasticityBrickBehaviours MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELPhysicalConstants>\;$ENV{PATH}")
	set_property(TEST brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemStandardElastoViscoPlasticityBrickBehaviours MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELPhysicalConstants>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST brick${test_arg}_${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemStandardElastoViscoPlasticityBrickBehaviours MFrontMaterialProperties-castem mtest")
	set_property(TEST brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemStandardElastoViscoPlasticityBrickBehaviours MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    endforeach(rm ${IEEE754_ROUNDING_MODES})
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/bricks/StandardElastoViscoPlasticity/castem"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront/tests/behaviours/bricks/StandardElastoViscoPlasticity/castem"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endif(HAVE_CASTEM)
endmacro(castemtest_standardelastovicoplasticitybrick)

castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest umatstandardelastoviscoplasticitynortontest)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest umatstandardelastoviscoplasticitynortontest_nj)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest2 umatstandardelastoviscoplasticitynortontest2)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest2 umatstandardelastoviscoplasticitynortontest2_nj)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest3 umatstandardelastoviscoplasticitynortontest3)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest3 umatstandardelastoviscoplasticitynortontest3_nj)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest4 umatstandardelastoviscoplasticitynortontest4)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest4 umatstandardelastoviscoplasticitynortontest4_nj)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest5 umatstandardelastoviscoplasticitynortontest5)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest5 umatstandardelastoviscoplasticitynortontest5_nj)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest6-1 umatstandardelastoviscoplasticitynortontest6)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest6-1 umatstandardelastoviscoplasticitynortontest6_nj)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest6-2 umatstandardelastoviscoplasticitynortontest6)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest6-2 umatstandardelastoviscoplasticitynortontest6_nj)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest7 umatstandardelastoviscoplasticitynortontest7 standardelastoviscoplasticitynortontest7.ref)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticitynortontest7 umatstandardelastoviscoplasticitynortontest7_nj standardelastoviscoplasticitynortontest7.ref)

castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityorthotropicnortontest
  umatstandardelastoviscoplasticityorthotropicnortontest)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityorthotropicnortontest
  umatstandardelastoviscoplasticityorthotropicnortontest_nj)

castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest umatstandardelastoviscoplasticityplasticitytest)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest_nj umatstandardelastoviscoplasticityplasticitytest_nj)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest2 umatstandardelastoviscoplasticityplasticitytest2)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest2_nj umatstandardelastoviscoplasticityplasticitytest2_nj)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest3 umatstandardelastoviscoplasticityplasticitytest3)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest3_nj umatstandardelastoviscoplasticityplasticitytest3_nj)

castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest4 umatstandardelastoviscoplasticityplasticitytest4 standardelastoviscoplasticityplasticitytest4.ref)

castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest5 umatstandardelastoviscoplasticityplasticitytest5)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest5_nj umatstandardelastoviscoplasticityplasticitytest5_nj)

castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest6 umatstandardelastoviscoplasticityplasticitytest6 standardelastoviscoplasticityplasticitytest6.ref)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest6 umatstandardelastoviscoplasticityplasticitytest6_nj standardelastoviscoplasticityplasticitytest6.ref)

castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest8 umatstandardelastoviscoplasticityplasticitytest8 standardelastoviscoplasticityplasticitytest8.ref)
castemtest_standardelastovicoplasticitybrick(castemstandardelastoviscoplasticityplasticitytest8_nj umatstandardelastoviscoplasticityplasticitytest8_nj standardelastoviscoplasticityplasticitytest8_nj.ref)
