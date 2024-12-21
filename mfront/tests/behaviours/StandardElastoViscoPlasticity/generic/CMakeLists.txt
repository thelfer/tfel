mfront_behaviour_brick_check_library(MFrontStandardElastoViscoPlasticityBrickBehaviours
  StandardElastoViscoPlasticity generic
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
  PlasticityTest18
  PlasticityTest18_nj
  PlasticityTest19
  PlasticityTest19_nj
  PlasticityTest20
  PlasticityTest20_nj
  PlasticityTest21
  PlasticityTest21_nj
  PlasticityTest22
  PlasticityTest22_nj
  PlasticityTest23
  PlasticityTest23_nj
  PlasticityTest24
  PlasticityTest24_nj
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

mfront_behaviour_brick_check_library(MFrontStandardElastoViscoPlasticityBrickBehaviours2
  StandardElastoViscoPlasticity generic
  UserDefinedViscoplasticityTest
  UserDefinedViscoplasticityTest_nj
  UserDefinedViscoplasticityTest2
  UserDefinedViscoplasticityTest2_nj
  UserDefinedViscoplasticityTest3
  UserDefinedViscoplasticityTest3_nj)

if(enable-mfront-quantity-tests)
  mfront_behaviour_brick_check_library(MFrontStandardElastoViscoPlasticityBrickBehaviours3
    StandardElastoViscoPlasticity generic
    PlasticityTest18_qt
    PlasticityTest18_qt_nj
    PlasticityTest19_qt
    PlasticityTest19_qt_nj
    PlasticityTest20_qt
    PlasticityTest20_qt_nj
    PlasticityTest21_qt
    PlasticityTest21_qt_nj
    PlasticityTest22_qt
    PlasticityTest22_qt_nj
    PlasticityTest23_qt
    PlasticityTest23_qt_nj
    PlasticityTest24_qt
    PlasticityTest24_qt_nj)
endif(enable-mfront-quantity-tests)

function(generictest_standardelastovicoplasticitybrick test_name test_file library_name behaviour_name)
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
  set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_file}.mtest")
  foreach(mh ${_MODELLING_HYPOTHESES})
    foreach(rm ${IEEE754_ROUNDING_MODES})
      if(NOT _REFERENCE_FILE)
        add_test(NAME brick${test_name}_${behaviour_name}_${mh}_${rm}_generic_mtest
          	     COMMAND mtest --rounding-direction-mode=${rm}
                 --verbose=level0 --xml-output=true --result-file-output=false
                 --@ModellingHypothesis="${mh}"
                 --@library@="$<TARGET_FILE:${library_name}>"
                 --@behaviour@="${behaviour_name}"
                 --@xml_output@="${test_name}-${behaviour_name}-${rm}.xml" ${file})
      else(NOT _REFERENCE_FILE)
        set(rfile "${top_srcdir}/mfront/tests/behaviours/StandardElastoViscoPlasticity/references/${_REFERENCE_FILE}")
        add_test(NAME brick${test_name}_${behaviour_name}_${mh}_${rm}_generic_mtest
          	     COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
                 --@ModellingHypothesis="${mh}"
          	     --@library@="$<TARGET_FILE:${library_name}>"
                 --@behaviour@="${behaviour_name}"
          	     --@xml_output@="${test_name}-${behaviour_name}-${rm}.xml"
          	     --@reference_file@="${rfile}" ${file})
      endif(NOT _REFERENCE_FILE)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
          	set_property(TEST brick${test_name}_${behaviour_name}_${mh}_${rm}_generic_mtest
          	  PROPERTY DEPENDS "MFrontStandardElastoViscoPlasticityBrickBehaviours mtest"
          	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST brick${test_name}_${behaviour_name}_${mh}_${rm}_generic_mtest
          	  PROPERTY DEPENDS "MFrontStandardElastoViscoPlasticityBrickBehaviours mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    endforeach(rm ${IEEE754_ROUNDING_MODES})
  endforeach(mh ${_MODELLING_HYPOTHESES})
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/generic"
      COMPONENT mtest)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront/tests/behaviours/generic"
      COMPONENT mtest)
  endif(TFEL_APPEND_SUFFIX)
endfunction(generictest_standardelastovicoplasticitybrick)

generictest_standardelastovicoplasticitybrick(GursonTvergaardNeedleman1982-test1
    GursonTvergaardNeedleman1982-test1
    MFrontStandardElastoViscoPlasticityBrickBehaviours
    GursonTvergaardNeedleman1982 GursonTvergaardNeedleman1982-test1.ref)
generictest_standardelastovicoplasticitybrick(GursonTvergaardNeedleman1982-test2
    GursonTvergaardNeedleman1982-test2
    MFrontStandardElastoViscoPlasticityBrickBehaviours
    GursonTvergaardNeedleman1982 GursonTvergaardNeedleman1982-test2.ref)

foreach(id RANGE 18 21)
  generictest_standardelastovicoplasticitybrick("DataIsotropicHardeningRuleTest-${id}"
    DataIsotropicHardeningRuleTest
    MFrontStandardElastoViscoPlasticityBrickBehaviours
    "PlasticityTest${id}"
    "DataIsotropicHardeningRuleTest-${id}.ref")
  generictest_standardelastovicoplasticitybrick("DataIsotropicHardeningRuleTest-nj-${id}"
    DataIsotropicHardeningRuleTest
    MFrontStandardElastoViscoPlasticityBrickBehaviours
    "PlasticityTest${id}_nj"
    "DataIsotropicHardeningRuleTest-${id}.ref")
  if(enable-mfront-quantity-tests)
    generictest_standardelastovicoplasticitybrick("DataIsotropicHardeningRuleTest-qt-${id}"
      DataIsotropicHardeningRuleTest
      MFrontStandardElastoViscoPlasticityBrickBehaviours3
      "PlasticityTest${id}_qt"
      "DataIsotropicHardeningRuleTest-${id}.ref")
    generictest_standardelastovicoplasticitybrick("DataIsotropicHardeningRuleTest-qt-nj-${id}"
      DataIsotropicHardeningRuleTest
      MFrontStandardElastoViscoPlasticityBrickBehaviours3
      "PlasticityTest${id}_qt_nj"
      "DataIsotropicHardeningRuleTest-${id}.ref")
  endif(enable-mfront-quantity-tests)
endforeach()

foreach(final_time 0.05 0.5 5 50 500 5000)
  generictest_standardelastovicoplasticitybrick("CowperSymondsRateSensitivityFactor-${final_time}"
     "RateSensitivityFactorTest-${final_time}"
     MFrontStandardElastoViscoPlasticityBrickBehaviours
     "PlasticityTest22"
     "CowperSymondsRateSensitivityFactorTest-${final_time}.ref")
  generictest_standardelastovicoplasticitybrick("CowperSymondsRateSensitivityFactor_nj-${final_time}"
     "RateSensitivityFactorTest-${final_time}"
     MFrontStandardElastoViscoPlasticityBrickBehaviours
     "PlasticityTest22_nj"
     "CowperSymondsRateSensitivityFactorTest-${final_time}.ref")
  generictest_standardelastovicoplasticitybrick("CowperSymondsRateSensitivityFactor_qt-${final_time}"
     "RateSensitivityFactorTest-${final_time}"
     MFrontStandardElastoViscoPlasticityBrickBehaviours3
     "PlasticityTest22_qt"
     "CowperSymondsRateSensitivityFactorTest-${final_time}.ref")
  generictest_standardelastovicoplasticitybrick("CowperSymondsRateSensitivityFactor_qt_nj-${final_time}"
     "RateSensitivityFactorTest-${final_time}"
     MFrontStandardElastoViscoPlasticityBrickBehaviours3
     "PlasticityTest22_qt_nj"
     "CowperSymondsRateSensitivityFactorTest-${final_time}.ref")
  generictest_standardelastovicoplasticitybrick("JohnsonCookRateSensitivityFactor-${final_time}"
     "RateSensitivityFactorTest-${final_time}"
     MFrontStandardElastoViscoPlasticityBrickBehaviours
     "PlasticityTest24"
     "JohnsonCookRateSensitivityFactorTest-${final_time}.ref")
  generictest_standardelastovicoplasticitybrick("JohnsonCookRateSensitivityFactor_nj-${final_time}"
     "RateSensitivityFactorTest-${final_time}"
     MFrontStandardElastoViscoPlasticityBrickBehaviours
     "PlasticityTest24_nj"
     "JohnsonCookRateSensitivityFactorTest-${final_time}.ref")
  generictest_standardelastovicoplasticitybrick("JohnsonCookRateSensitivityFactor_qt-${final_time}"
     "RateSensitivityFactorTest-${final_time}"
     MFrontStandardElastoViscoPlasticityBrickBehaviours3
     "PlasticityTest24_qt"
     "JohnsonCookRateSensitivityFactorTest-${final_time}.ref")
  generictest_standardelastovicoplasticitybrick("JohnsonCookRateSensitivityFactor_qt_nj-${final_time}"
     "RateSensitivityFactorTest-${final_time}"
     MFrontStandardElastoViscoPlasticityBrickBehaviours3
     "PlasticityTest24_qt_nj"
     "JohnsonCookRateSensitivityFactorTest-${final_time}.ref")
endforeach()

