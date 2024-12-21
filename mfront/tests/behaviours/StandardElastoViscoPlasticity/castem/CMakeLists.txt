if(HAVE_CASTEM)
  mfront_behaviour_brick_check_library(MFrontCastemStandardElastoViscoPlasticityBrickBehaviours
  StandardElastoViscoPlasticity umat
  Test1
  Test2
  Test3
  Test4
  Test5
  Test6
  NortonTest
  NortonTest_nj
  NortonTest2
  NortonTest2_nj
  NortonTest3
  NortonTest3_nj
  NortonTest4
  NortonTest4_nj
  NortonTest5
  NortonTest5_nj
  NortonTest6
  NortonTest6_nj
  NortonTest7
  NortonTest7_nj
  NortonTest8
  NortonTest8_nj
  NortonTest9
  NortonTest9_nj
  OrthotropicNortonTest
  OrthotropicNortonTest_nj
  OrthotropicNortonTest_na
  OrthotropicNortonTest_nanj
  PlasticityTest
  PlasticityTest_nj
  PlasticityTest_na
  PlasticityTest_nanj
  PlasticityTest2
  PlasticityTest2_nj
  PlasticityTest3
  PlasticityTest3_nj
  PlasticityTest4
  PlasticityTest4_nj
  PlasticityTest5
  PlasticityTest5_nj
  PlasticityTest6
  PlasticityTest6_nj
  PlasticityTest7
  PlasticityTest7_nj
  PlasticityTest8
  PlasticityTest8_nj
  PlasticityTest9
  PlasticityTest9_nj
  PlasticityTest9_na
  PlasticityTest9_nanj
  PlasticityTest10
  PlasticityTest10_nj
  PlasticityTest10_na
  PlasticityTest10_nanj
  PlasticityTest11
  PlasticityTest11_nj
  PlasticityTest11_na
  PlasticityTest11_nanj
  PlasticityTest12
  PlasticityTest12_nj
  PlasticityTest12_na
  PlasticityTest12_nanj
  PlasticityTest13
  PlasticityTest13_nj
  PlasticityTest13_na
  PlasticityTest13_nanj
  PlasticityTest14
  PlasticityTest14_nj
  PlasticityTest14_na
  PlasticityTest14_nanj
  PlasticityTest15
  PlasticityTest15_nj
  PlasticityTest15_na
  PlasticityTest15_nanj
  PlasticityTest16
  PlasticityTest16_nj
  PlasticityTest17
  PlasticityTest17_nj
  IsotropicDamageHookeLaw
  IsotropicDamageHookeLaw_nj
  IsotropicDamageHookeLaw2
  IsotropicDamageHookeLaw2_nj
  HyperbolicSineTest
  HyperbolicSineTest_nj
  HyperbolicSineTest2
  HyperbolicSineTest2_nj
  HarmonicSumOfNortonHoffViscoplasticFlowsTest
  HarmonicSumOfNortonHoffViscoplasticFlowsTest_nj
  HarmonicSumOfNortonHoffViscoplasticFlowsTest2
  HarmonicSumOfNortonHoffViscoplasticFlowsTest2_nj
  ChuNeedleman1980StrainBasedNucleationModelTest
  ChuNeedleman1980StressBasedNucleationModelTest
  PowerLawStrainBasedNucleationModelTest
  PowerLawStressBasedNucleationModelTest
  ElasticContributionToPorosityGrowth
  GursonTvergaardNeedleman1982
  RousselierTanguyBesson2002)

  target_link_libraries(MFrontCastemStandardElastoViscoPlasticityBrickBehaviours
    PRIVATE CastemInterface)
endif(HAVE_CASTEM)

function(castemtest_standardelastovicoplasticitybrick test_arg behaviour_name)
  if(HAVE_CASTEM)
    set(_REFERENCE_FILE )
    set(_MODELLING_HYPOTHESES)
	foreach(_ARG ${ARGN})
	  if ( ${_ARG} MATCHES PLANESTRESS)
	    list(APPEND _MODELLING_HYPOTHESES "PlaneStress")
	  elseif ( ${_ARG} MATCHES TRIDIMENSIONAL)
	    list(APPEND _MODELLING_HYPOTHESES "Tridimensional")
	  elseif ( ${_ARG} MATCHES WILL_FAIL)
	    set ( _WILL_FAIL ON)
	  else()
        set(_REFERENCE_FILE "${_ARG}")
	  endif()
	endforeach(_ARG ${ARGN})
    if(NOT _MODELLING_HYPOTHESES)
      set(_MODELLING_HYPOTHESES "Tridimensional")
    endif(NOT _MODELLING_HYPOTHESES)
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
    foreach(mh ${_MODELLING_HYPOTHESES})
      foreach(rm ${IEEE754_ROUNDING_MODES})
        if(NOT _REFERENCE_FILE)
          add_test(NAME brick${test_arg}_${behaviour_name}_${mh}_${rm}_castem_mtest
  	              COMMAND mtest --rounding-direction-mode=${rm}
                  --verbose=level0 --xml-output=true
                  --result-file-output=false
                  --@AccelerationAlgorithm='Cast3M'
                  --@ModellingHypothesis="${mh}"
                  --@library@="$<TARGET_FILE:MFrontCastemStandardElastoViscoPlasticityBrickBehaviours>"
                  --@behaviour@="${behaviour_name}"
                  --@xml_output@="${test_arg}-${behaviour_name}-${rm}.xml" ${file})
          add_test(NAME brick${test_arg}_${behaviour_name}_consistenttangentoperator_${mh}_${rm}_castem_mtest
            	     COMMAND mtest --rounding-direction-mode=${rm}
                  --verbose=level0 --xml-output=true --result-file-output=false
                  --@StiffnessMatrixType='ConsistentTangentOperator'
                  --@ModellingHypothesis="${mh}"
                  --@library@="$<TARGET_FILE:MFrontCastemStandardElastoViscoPlasticityBrickBehaviours>"
                  --@behaviour@="${behaviour_name}"
                  --@xml_output@="${test_arg}-${behaviour_name}-consistenttangentoperator-${rm}.xml" ${file})
        else(NOT _REFERENCE_FILE)
          set(rfile "${top_srcdir}/mfront/tests/behaviours/StandardElastoViscoPlasticity/references/${_REFERENCE_FILE}")
          add_test(NAME brick${test_arg}_${behaviour_name}_${mh}_${rm}_castem_mtest
            	     COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
            	     --@AccelerationAlgorithm='Cast3M'
                  --@ModellingHypothesis="${mh}"
            	     --@library@="$<TARGET_FILE:MFrontCastemStandardElastoViscoPlasticityBrickBehaviours>"
                  --@behaviour@="${behaviour_name}"
            	     --@xml_output@="${test_arg}-${behaviour_name}-${rm}.xml"
            	     --@reference_file@="${rfile}" ${file})
          add_test(NAME brick${test_arg}_${behaviour_name}_consistenttangentoperator_${mh}_${rm}_castem_mtest
            	     COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
            	     --@StiffnessMatrixType='ConsistentTangentOperator'
                  --@ModellingHypothesis="${mh}"
            	     --@library@="$<TARGET_FILE:MFrontCastemStandardElastoViscoPlasticityBrickBehaviours>"
                  --@behaviour@="${behaviour_name}"
            	     --@xml_output@="${test_arg}-${behaviour_name}-consistenttangentoperator-${rm}.xml"
            	     --@reference_file@="${rfile}" ${file})
        endif(NOT _REFERENCE_FILE)
        if((CMAKE_HOST_WIN32) AND (NOT MSYS))
            	set_property(TEST brick${test_arg}_${behaviour_name}_${mh}_${rm}_castem_mtest
            	  PROPERTY DEPENDS "MFrontCastemStandardElastoViscoPlasticityBrickBehaviours MFrontMaterialProperties-castem mtest"
            	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
            	set_property(TEST brick${test_arg}_${behaviour_name}_consistenttangentoperator_${mh}_${rm}_castem_mtest
            	  PROPERTY DEPENDS "MFrontCastemStandardElastoViscoPlasticityBrickBehaviours MFrontMaterialProperties-castem mtest"
            	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
        elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST brick${test_arg}_${behaviour_name}_${mh}_${rm}_castem_mtest
            	  PROPERTY DEPENDS "MFrontCastemStandardElastoViscoPlasticityBrickBehaviours MFrontMaterialProperties-castem mtest")
          set_property(TEST brick${test_arg}_${behaviour_name}_consistenttangentoperator_${mh}_${rm}_castem_mtest
            	  PROPERTY DEPENDS "MFrontCastemStandardElastoViscoPlasticityBrickBehaviours MFrontMaterialProperties-castem mtest")
        endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      endforeach(rm ${IEEE754_ROUNDING_MODES})
    endforeach(mh ${_MODELLING_HYPOTHESES})
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/StandardElastoViscoPlasticity/castem"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront/tests/behaviours/StandardElastoViscoPlasticity/castem"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endif(HAVE_CASTEM)
endfunction(castemtest_standardelastovicoplasticitybrick)

function(castemtest_standardelastovicoplasticitybrick_python test)
  configure_file(castemtest_standardelastovicoplasticitybrick_python_${test}.py.in
    castemtest_standardelastovicoplasticitybrick_python_${test}.py @ONLY)
  add_test(NAME castemtest_standardelastovicoplasticitybrick_python_${test}_py
    COMMAND ${PYTHON_EXECUTABLE} castemtest_standardelastovicoplasticitybrick_python_${test}.py)
  set_tests_properties(castemtest_standardelastovicoplasticitybrick_python_${test}_py
    PROPERTIES DEPENDS MFrontCastemStandardElastoViscoPlasticityBrickBehaviours)
  if(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(castemtest_standardelastovicoplasticitybrick_python_${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python;$<TARGET_FILE_DIR:py_std__stl_vector>;$<TARGET_FILE_DIR:py_tfel_tests>")
  else(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(castemtest_standardelastovicoplasticitybrick_python_${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python")
  endif(CMAKE_CONFIGURATION_TYPES)
endfunction(castemtest_standardelastovicoplasticitybrick_python)

castemtest_standardelastovicoplasticitybrick(nortontest
  umatnortontest)
castemtest_standardelastovicoplasticitybrick(nortontest
  umatnortontest_nj)
castemtest_standardelastovicoplasticitybrick(nortontest2
  umatnortontest2)
castemtest_standardelastovicoplasticitybrick(nortontest2
  umatnortontest2_nj)
castemtest_standardelastovicoplasticitybrick(nortontest3
  umatnortontest3)
castemtest_standardelastovicoplasticitybrick(nortontest3
  umatnortontest3_nj)
castemtest_standardelastovicoplasticitybrick(nortontest4
  umatnortontest4)
castemtest_standardelastovicoplasticitybrick(nortontest4
  umatnortontest4_nj)
castemtest_standardelastovicoplasticitybrick(nortontest5
  umatnortontest5)
castemtest_standardelastovicoplasticitybrick(nortontest5
  umatnortontest5_nj)
castemtest_standardelastovicoplasticitybrick(nortontest6-1
  umatnortontest6)
castemtest_standardelastovicoplasticitybrick(nortontest6-1
  umatnortontest6_nj)
castemtest_standardelastovicoplasticitybrick(nortontest6-2
  umatnortontest6)
castemtest_standardelastovicoplasticitybrick(nortontest6-2
  umatnortontest6_nj)
castemtest_standardelastovicoplasticitybrick(nortontest7
  umatnortontest7 standardelastoviscoplasticitynortontest7.ref)
castemtest_standardelastovicoplasticitybrick(nortontest7
  umatnortontest7_nj standardelastoviscoplasticitynortontest7.ref)
castemtest_standardelastovicoplasticitybrick(nortontest8_E
  umatnortontest8
  standardelastoviscoplasticitynortontest8_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest8_S
  umatnortontest8
  standardelastoviscoplasticitynortontest8_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest8-cyclic_E
  umatnortontest8
  standardelastoviscoplasticitynortontest8-cyclic_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest8-cyclic_S
  umatnortontest8
  standardelastoviscoplasticitynortontest8-cyclic_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest8_E
  umatnortontest8_nj
  standardelastoviscoplasticitynortontest8_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest8_S
  umatnortontest8_nj
  standardelastoviscoplasticitynortontest8_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest8-cyclic_E
  umatnortontest8_nj
  standardelastoviscoplasticitynortontest8-cyclic_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest8-cyclic_S
  umatnortontest8_nj
  standardelastoviscoplasticitynortontest8-cyclic_S.ref
  PLANESTRESS TRIDIMENSIONAL)

castemtest_standardelastovicoplasticitybrick(nortontest9_E
  umatnortontest9
  standardelastoviscoplasticitynortontest9_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest9_S
  umatnortontest9
  standardelastoviscoplasticitynortontest9_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest9-cyclic_E
  umatnortontest9
  standardelastoviscoplasticitynortontest9-cyclic_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest9-cyclic_S
  umatnortontest9
  standardelastoviscoplasticitynortontest9-cyclic_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest9_E
  umatnortontest9_nj
  standardelastoviscoplasticitynortontest9_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest9_S
  umatnortontest9_nj
  standardelastoviscoplasticitynortontest9_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest9-cyclic_E
  umatnortontest9_nj
  standardelastoviscoplasticitynortontest9-cyclic_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(nortontest9-cyclic_S
  umatnortontest9_nj
  standardelastoviscoplasticitynortontest9-cyclic_S.ref
  PLANESTRESS TRIDIMENSIONAL)


castemtest_standardelastovicoplasticitybrick(orthotropicnortontest
  umatorthotropicnortontest)
castemtest_standardelastovicoplasticitybrick(orthotropicnortontest
  umatorthotropicnortontest_nj)
castemtest_standardelastovicoplasticitybrick(orthotropicnortontest
  umatorthotropicnortontest_na)
castemtest_standardelastovicoplasticitybrick(orthotropicnortontest
  umatorthotropicnortontest_nanj)

castemtest_standardelastovicoplasticitybrick(hyperbolicsinetest
  umathyperbolicsinetest)
castemtest_standardelastovicoplasticitybrick(hyperbolicsinetest
  umathyperbolicsinetest_nj)
castemtest_standardelastovicoplasticitybrick(hyperbolicsinetest2
  umathyperbolicsinetest2)
castemtest_standardelastovicoplasticitybrick(hyperbolicsinetest2
  umathyperbolicsinetest2_nj)

castemtest_standardelastovicoplasticitybrick(plasticitytest
  umatplasticitytest
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest_nj
  umatplasticitytest_nj)
castemtest_standardelastovicoplasticitybrick(plasticitytest
  umatplasticitytest_na
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest_nj
  umatplasticitytest_nanj)
castemtest_standardelastovicoplasticitybrick(plasticitytest2
  umatplasticitytest2
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest2_nj
  umatplasticitytest2_nj
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest3
  umatplasticitytest3
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest3_nj
  umatplasticitytest3_nj
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest4
  umatplasticitytest4 standardelastoviscoplasticityplasticitytest4.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest5
  umatplasticitytest5
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest5_nj
  umatplasticitytest5_nj
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest6
  umatplasticitytest6 standardelastoviscoplasticityplasticitytest6.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest6
  umatplasticitytest6_nj standardelastoviscoplasticityplasticitytest6.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest7_E
  umatplasticitytest7
  standardelastoviscoplasticityplasticitytest7_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest7_S
  umatplasticitytest7
  standardelastoviscoplasticityplasticitytest7_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest7-cyclic_E
  umatplasticitytest7
  standardelastoviscoplasticityplasticitytest7-cyclic_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest7-cyclic_S
  umatplasticitytest7
  standardelastoviscoplasticityplasticitytest7-cyclic_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest7_E
  umatplasticitytest7_nj
  standardelastoviscoplasticityplasticitytest7_E.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest7_S
  umatplasticitytest7_nj
  standardelastoviscoplasticityplasticitytest7_S.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest7-cyclic_E
  umatplasticitytest7_nj
  standardelastoviscoplasticityplasticitytest7-cyclic_E.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest7-cyclic_S
  umatplasticitytest7_nj
  standardelastoviscoplasticityplasticitytest7-cyclic_S.ref)

castemtest_standardelastovicoplasticitybrick(plasticitytest8
  umatplasticitytest8 standardelastoviscoplasticityplasticitytest8.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest8_nj
  umatplasticitytest8_nj standardelastoviscoplasticityplasticitytest8_nj.ref)

castemtest_standardelastovicoplasticitybrick(plasticitytest11_E
  umatplasticitytest11
  standardelastoviscoplasticityplasticitytest11_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest11_E
  umatplasticitytest11_na
  standardelastoviscoplasticityplasticitytest11_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest11_E
  umatplasticitytest11_nj
  standardelastoviscoplasticityplasticitytest11_E.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest11_E
  umatplasticitytest11_nanj
  standardelastoviscoplasticityplasticitytest11_E.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest11_S
  umatplasticitytest11
  standardelastoviscoplasticityplasticitytest11_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest11_S
  umatplasticitytest11_na
  standardelastoviscoplasticityplasticitytest11_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest11_S
  umatplasticitytest11_nj
  standardelastoviscoplasticityplasticitytest11_S.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest11_S
  umatplasticitytest11_nanj
  standardelastoviscoplasticityplasticitytest11_S.ref)

castemtest_standardelastovicoplasticitybrick(plasticitytest11-cyclic_E
  umatplasticitytest11
  standardelastoviscoplasticityplasticitytest11-cyclic_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest11-cyclic_E
  umatplasticitytest11_na
  standardelastoviscoplasticityplasticitytest11-cyclic_E.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest11-cyclic_E
  umatplasticitytest11_nj
  standardelastoviscoplasticityplasticitytest11-cyclic_E.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest11-cyclic_E
  umatplasticitytest11_nanj
  standardelastoviscoplasticityplasticitytest11-cyclic_E.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest11-cyclic_S
  umatplasticitytest11
  standardelastoviscoplasticityplasticitytest11-cyclic_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest11-cyclic_S
  umatplasticitytest11_na
  standardelastoviscoplasticityplasticitytest11-cyclic_S.ref
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest11-cyclic_S
  umatplasticitytest11_nj
  standardelastoviscoplasticityplasticitytest11-cyclic_S.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest11-cyclic_S
  umatplasticitytest11_nanj
  standardelastoviscoplasticityplasticitytest11-cyclic_S.ref)

castemtest_standardelastovicoplasticitybrick(plasticitytest12
  umatplasticitytest12 PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest12
  umatplasticitytest12_nj PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest12
  umatplasticitytest12_na PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest12
  umatplasticitytest12_nanj PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest13
  umatplasticitytest13 PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest13
  umatplasticitytest13_nj PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest13
  umatplasticitytest13_na PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest13
  umatplasticitytest13_nanj PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(plasticitytest14
  umatplasticitytest14)
castemtest_standardelastovicoplasticitybrick(plasticitytest14
  umatplasticitytest14_nj)
castemtest_standardelastovicoplasticitybrick(plasticitytest14
  umatplasticitytest14_na)
castemtest_standardelastovicoplasticitybrick(plasticitytest14
  umatplasticitytest14_nanj)
castemtest_standardelastovicoplasticitybrick(plasticitytest16
  umatplasticitytest16 standardelastoviscoplasticityplasticitytest16.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest16
  umatplasticitytest16_nj standardelastoviscoplasticityplasticitytest16.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest17
  umatplasticitytest17 standardelastoviscoplasticityplasticitytest17.ref)
castemtest_standardelastovicoplasticitybrick(plasticitytest17
  umatplasticitytest17_nj standardelastoviscoplasticityplasticitytest17.ref)


castemtest_standardelastovicoplasticitybrick(isotropicdamagehookelaw
  umatisotropicdamagehookelaw
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(isotropicdamagehookelaw
  umatisotropicdamagehookelaw_nj
  PLANESTRESS TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(isotropicdamagehookelaw2
  umatisotropicdamagehookelaw2 TRIDIMENSIONAL)
castemtest_standardelastovicoplasticitybrick(isotropicdamagehookelaw2
  umatisotropicdamagehookelaw2_nj)

castemtest_standardelastovicoplasticitybrick(nortontest
  umatharmonicsumofnortonhoffviscoplasticflowstest)
castemtest_standardelastovicoplasticitybrick(nortontest
  umatharmonicsumofnortonhoffviscoplasticflowstest_nj)
castemtest_standardelastovicoplasticitybrick(nortontest
  umatharmonicsumofnortonhoffviscoplasticflowstest2)
castemtest_standardelastovicoplasticitybrick(nortontest
  umatharmonicsumofnortonhoffviscoplasticflowstest2_nj)

if(enable-python-bindings)
#  castemtest_standardelastovicoplasticitybrick_python(Cazacu_2001)
endif(enable-python-bindings)

