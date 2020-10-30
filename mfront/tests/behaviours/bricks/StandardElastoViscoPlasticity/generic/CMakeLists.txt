mfront_behaviour_brick_check_library(MFrontStandardElastoViscoPlasticityBrickBehaviours
  StandardElastoViscoPlasticity generic
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
  StandardElastoViscoPlasticityNortonTest8
  StandardElastoViscoPlasticityNortonTest8_nj
  StandardElastoViscoPlasticityNortonTest9
  StandardElastoViscoPlasticityNortonTest9_nj
  StandardElastoViscoPlasticityOrthotropicNortonTest
  StandardElastoViscoPlasticityOrthotropicNortonTest_nj
  StandardElastoViscoPlasticityOrthotropicNortonTest_na
  StandardElastoViscoPlasticityOrthotropicNortonTest_nanj
  StandardElastoViscoPlasticityPlasticityTest
  StandardElastoViscoPlasticityPlasticityTest_nj
  StandardElastoViscoPlasticityPlasticityTest_na
  StandardElastoViscoPlasticityPlasticityTest_nanj
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
  StandardElastoViscoPlasticityPlasticityTest8_nj
  StandardElastoViscoPlasticityPlasticityTest9
  StandardElastoViscoPlasticityPlasticityTest9_nj
  StandardElastoViscoPlasticityPlasticityTest9_na
  StandardElastoViscoPlasticityPlasticityTest9_nanj
  StandardElastoViscoPlasticityPlasticityTest10
  StandardElastoViscoPlasticityPlasticityTest10_nj
  StandardElastoViscoPlasticityPlasticityTest10_na
  StandardElastoViscoPlasticityPlasticityTest10_nanj
  StandardElastoViscoPlasticityPlasticityTest11
  StandardElastoViscoPlasticityPlasticityTest11_nj
  StandardElastoViscoPlasticityPlasticityTest11_na
  StandardElastoViscoPlasticityPlasticityTest11_nanj
  StandardElastoViscoPlasticityPlasticityTest12
  StandardElastoViscoPlasticityPlasticityTest12_nj
  StandardElastoViscoPlasticityPlasticityTest12_na
  StandardElastoViscoPlasticityPlasticityTest12_nanj
  StandardElastoViscoPlasticityPlasticityTest13
  StandardElastoViscoPlasticityPlasticityTest13_nj
  StandardElastoViscoPlasticityPlasticityTest13_na
  StandardElastoViscoPlasticityPlasticityTest13_nanj
  StandardElastoViscoPlasticityPlasticityTest14
  StandardElastoViscoPlasticityPlasticityTest14_nj
  StandardElastoViscoPlasticityPlasticityTest14_na
  StandardElastoViscoPlasticityPlasticityTest14_nanj
  StandardElastoViscoPlasticityPlasticityTest15
  StandardElastoViscoPlasticityPlasticityTest15_nj
  StandardElastoViscoPlasticityPlasticityTest15_na
  StandardElastoViscoPlasticityPlasticityTest15_nanj
  StandardElastoViscoPlasticityPlasticityTest16
  StandardElastoViscoPlasticityPlasticityTest16_nj
  StandardElastoViscoPlasticityPlasticityTest17
  StandardElastoViscoPlasticityPlasticityTest17_nj
  StandardElastoViscoPlasticityIsotropicDamageHookeLaw
  StandardElastoViscoPlasticityIsotropicDamageHookeLaw_nj
  StandardElastoViscoPlasticityIsotropicDamageHookeLaw2
  StandardElastoViscoPlasticityIsotropicDamageHookeLaw2_nj
  StandardElastoViscoPlasticityHyperbolicSineTest
  StandardElastoViscoPlasticityHyperbolicSineTest_nj
  StandardElastoViscoPlasticityHyperbolicSineTest2
  StandardElastoViscoPlasticityHyperbolicSineTest2_nj
  ChuNeedleman1980StrainBasedNucleationModelTest
  ChuNeedleman1980StressBasedNucleationModelTest
  PowerLawStrainBasedNucleationModelTest
  PowerLawStressBasedNucleationModelTest
  ElasticContributionToPorosityGrowth
  GursonTvergaardNeedleman1982
  RousselierTanguyBesson2002)

function(generictest_standardelastovicoplasticitybrick test_arg behaviour_name)
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
        add_test(NAME brick${test_arg}_${behaviour_name}_${mh}_${rm}_generic_mtest
          	     COMMAND mtest --rounding-direction-mode=${rm}
                --verbose=level0 --xml-output=true --result-file-output=false
                --@ModellingHypothesis="${mh}"
                --@library@="$<TARGET_FILE:MFrontStandardElastoViscoPlasticityBrickBehaviours>"
                --@behaviour@="${behaviour_name}"
                --@xml_output@="${test_arg}-${behaviour_name}-${rm}.xml" ${file})
      else(NOT _REFERENCE_FILE)
        set(rfile "${top_srcdir}/mfront/tests/behaviours/bricks/StandardElastoViscoPlasticity/references/${_REFERENCE_FILE}")
        add_test(NAME brick${test_arg}_${behaviour_name}_${mh}_${rm}_generic_mtest
          	     COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
                 --@ModellingHypothesis="${mh}"
          	     --@library@="$<TARGET_FILE:MFrontStandardElastoViscoPlasticityBrickBehaviours>"
                 --@behaviour@="${behaviour_name}"
          	     --@xml_output@="${test_arg}-${behaviour_name}-${rm}.xml"
          	     --@reference_file@="${rfile}" ${file})
      endif(NOT _REFERENCE_FILE)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
          	set_property(TEST brick${test_arg}_${behaviour_name}_${mh}_${rm}_generic_mtest
          	  PROPERTY DEPENDS "MFrontStandardElastoViscoPlasticityBrickBehaviours mtest"
          	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELPhysicalConstants>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
          	set_property(TEST brick${test_arg}_${behaviour_name}_consistenttangentoperator_${mh}_${rm}_generic_mtest
          	  PROPERTY DEPENDS "MFrontStandardElastoViscoPlasticityBrickBehaviours mtest"
          	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELPhysicalConstants>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST brick${test_arg}_${behaviour_name}_${mh}_${rm}_generic_mtest
          	  PROPERTY DEPENDS "MFrontStandardElastoViscoPlasticityBrickBehaviours mtest")
        set_property(TEST brick${test_arg}_${behaviour_name}_consistenttangentoperator_${mh}_${rm}_generic_mtest
          	  PROPERTY DEPENDS "MFrontStandardElastoViscoPlasticityBrickBehaviours mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    endforeach(rm ${IEEE754_ROUNDING_MODES})
  endforeach(mh ${_MODELLING_HYPOTHESES})
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/bricks/StandardElastoViscoPlasticity/generic"
      COMPONENT mtest)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront/tests/behaviours/bricks/StandardElastoViscoPlasticity/generic"
      COMPONENT mtest)
  endif(TFEL_APPEND_SUFFIX)
endfunction(generictest_standardelastovicoplasticitybrick)

# generictest_standardelastovicoplasticitybrick(GursonTvergaardNeedleman1982-test1 GursonTvergaardNeedleman1982)
# generictest_standardelastovicoplasticitybrick(GursonTvergaardNeedleman1982-test2 GursonTvergaardNeedleman1982)
