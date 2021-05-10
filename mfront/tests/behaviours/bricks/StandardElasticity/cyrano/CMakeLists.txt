if(HAVE_CYRANO)
  mfront_dependencies(MFrontCyranoBehaviourBricks
    YoungModulusTest
    YoungModulusTest_1
    YoungModulusTest_2
    YoungModulusTest_3
    PoissonRatioTest_12
    PoissonRatioTest_23
    PoissonRatioTest_13
    ShearModulusTest_12
    ShearModulusTest_23
    ShearModulusTest_13
    Inconel600_YoungModulus)
  mfront_behaviour_brick_check_library(MFrontCyranoBehaviourBricks
    StandardElasticity cyrano
    IsotropicStandardElasticity
    IsotropicStandardElasticity2
    IsotropicStandardElasticity3
    IsotropicStandardElasticity4
    IsotropicStandardElasticity5 
    IsotropicStandardElasticity6
    IsotropicStandardElasticity7
    IsotropicStandardElasticity8
    IsotropicStandardElasticity9
    IsotropicStandardElasticity10
    IsotropicStandardElasticity11
    OrthotropicStandardElasticity
    OrthotropicStandardElasticity2
    OrthotropicStandardElasticity3
    TestComputeElasticPrediction
    TestComputeElasticPrediction2
    IsotropicJ2Plasticity)
  target_link_libraries(MFrontCyranoBehaviourBricks
    PRIVATE CyranoInterface)
endif(HAVE_CYRANO)

macro(cyranotest_standardelasticitybrick test_arg behaviour)
  if(HAVE_CYRANO)
    set(_REFERENCE_FILE )
    if(NOT (${ARGC} EQUAL 0))
      set(_REFERENCE_FILE "${ARGN}")
    endif()
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
    foreach(rm ${IEEE754_ROUNDING_MODES})
      if(NOT _REFERENCE_FILE)
	add_test(NAME brick${test_arg}${behaviour}_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@library@="$<TARGET_FILE:MFrontCyranoBehaviourBricks>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml" ${file})
      else(NOT _REFERENCE_FILE)
	add_test(NAME brick${test_arg}${behaviour}_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@library@="$<TARGET_FILE:MFrontCyranoBehaviourBricks>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml"
	  --@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}"
	  ${file})
      endif(NOT _REFERENCE_FILE)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST brick${test_arg}${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCyranoBehaviours MFrontMaterialProperties-cyrano mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST brick${test_arg}${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCyranoBehaviours MFrontMaterialProperties-cyrano mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    endforeach(rm ${IEEE754_ROUNDING_MODES})
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/bricks/StandardElasticity/cyrano/"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront/tests/behaviours/bricks/StandardElasticity/cyrano/"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endif(HAVE_CYRANO)
endmacro(cyranotest_standardelasticitybrick)

cyranotest_standardelasticitybrick(isotropicstandardelasticity cyranoisotropicstandardelasticity )
cyranotest_standardelasticitybrick(isotropicstandardelasticity cyranoisotropicstandardelasticity3)
cyranotest_standardelasticitybrick(isotropicstandardelasticity cyranoisotropicstandardelasticity4)
cyranotest_standardelasticitybrick(isotropicstandardelasticity cyranoisotropicstandardelasticity5)
cyranotest_standardelasticitybrick(isotropicstandardelasticity cyranoisotropicstandardelasticity6)
cyranotest_standardelasticitybrick(isotropicstandardelasticity cyranoisotropicstandardelasticity8)
cyranotest_standardelasticitybrick(isotropicstandardelasticity cyranoisotropicstandardelasticity9)
cyranotest_standardelasticitybrick(isotropicstandardelasticity cyranoisotropicstandardelasticity10)

cyranotest_standardelasticitybrick(testcomputeelasticprediction  cyranotestcomputeelasticprediction)
cyranotest_standardelasticitybrick(testcomputeelasticprediction2 cyranotestcomputeelasticprediction)
cyranotest_standardelasticitybrick(testcomputeelasticprediction3 cyranotestcomputeelasticprediction2)
cyranotest_standardelasticitybrick(testcomputeelasticprediction4 cyranotestcomputeelasticprediction2)
