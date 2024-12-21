if(HAVE_CASTEM)
  mfront_dependencies(MFrontCastemBehaviourBricks
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
  mfront_behaviour_brick_check_library(MFrontCastemBehaviourBricks
    StandardElasticity umat
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
    IsotropicStandardElasticity12
    IsotropicStandardElasticity13
    OrthotropicStandardElasticity
    OrthotropicStandardElasticity2
    OrthotropicStandardElasticity3
    TestComputeElasticPrediction
    TestComputeElasticPrediction2
    IsotropicJ2Plasticity
    SingleCrystal_DD_FCC)
  target_link_libraries(MFrontCastemBehaviourBricks
    PRIVATE CastemInterface)
endif(HAVE_CASTEM)

macro(castemtest_standardelasticitybrick test_arg behaviour)
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
	  --@library@="$<TARGET_FILE:MFrontCastemBehaviourBricks>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml" ${file})
	add_test(NAME brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@StiffnessMatrixType='ConsistentTangentOperator'
	  --@library@="$<TARGET_FILE:MFrontCastemBehaviourBricks>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-consistenttangentoperator-${rm}.xml" ${file})
      else(NOT _REFERENCE_FILE)
	add_test(NAME brick${test_arg}_${behaviour}_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@AccelerationAlgorithm='Cast3M'
	  --@library@="$<TARGET_FILE:MFrontCastemBehaviourBricks>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml"
	  --@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}" ${file})
	add_test(NAME brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@StiffnessMatrixType='ConsistentTangentOperator'
	  --@library@="$<TARGET_FILE:MFrontCastemBehaviourBricks>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-consistenttangentoperator-${rm}.xml"
	  --@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}" ${file})
      endif(NOT _REFERENCE_FILE)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST brick${test_arg}_${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemBehaviourBricks MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
	set_property(TEST brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemBehaviourBricks MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST brick${test_arg}_${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemBehaviourBricks MFrontMaterialProperties-castem mtest")
	set_property(TEST brick${test_arg}_${behaviour}_consistenttangentoperator_${rm}_mtest
	  PROPERTY DEPENDS "MFrontCastemBehaviourBricks MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    endforeach(rm ${IEEE754_ROUNDING_MODES})
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/bricks/StandardElasticity/castem"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront/tests/behaviours/bricks/StandardElasticity/castem"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endif(HAVE_CASTEM)
endmacro(castemtest_standardelasticitybrick)

castemtest_standardelasticitybrick(isotropicstandardelasticity umatisotropicstandardelasticity )
castemtest_standardelasticitybrick(isotropicstandardelasticity umatisotropicstandardelasticity2)
castemtest_standardelasticitybrick(isotropicstandardelasticity umatisotropicstandardelasticity3)
castemtest_standardelasticitybrick(isotropicstandardelasticity umatisotropicstandardelasticity4)
castemtest_standardelasticitybrick(isotropicstandardelasticity umatisotropicstandardelasticity5)
castemtest_standardelasticitybrick(isotropicstandardelasticity umatisotropicstandardelasticity6)
castemtest_standardelasticitybrick(isotropicstandardelasticity umatisotropicstandardelasticity7)
castemtest_standardelasticitybrick(isotropicstandardelasticity umatisotropicstandardelasticity8)
castemtest_standardelasticitybrick(isotropicstandardelasticity umatisotropicstandardelasticity9)
castemtest_standardelasticitybrick(isotropicstandardelasticity umatisotropicstandardelasticity10)

castemtest_standardelasticitybrick(isotropicstandardelasticity2 umatisotropicstandardelasticity )
castemtest_standardelasticitybrick(isotropicstandardelasticity2 umatisotropicstandardelasticity2)
castemtest_standardelasticitybrick(isotropicstandardelasticity2 umatisotropicstandardelasticity3)
castemtest_standardelasticitybrick(isotropicstandardelasticity2 umatisotropicstandardelasticity4)
castemtest_standardelasticitybrick(isotropicstandardelasticity2 umatisotropicstandardelasticity5)
castemtest_standardelasticitybrick(isotropicstandardelasticity2 umatisotropicstandardelasticity6)
castemtest_standardelasticitybrick(isotropicstandardelasticity2 umatisotropicstandardelasticity7)
castemtest_standardelasticitybrick(isotropicstandardelasticity2 umatisotropicstandardelasticity8)
castemtest_standardelasticitybrick(isotropicstandardelasticity2 umatisotropicstandardelasticity9)
castemtest_standardelasticitybrick(isotropicstandardelasticity2 umatisotropicstandardelasticity10)

castemtest_standardelasticitybrick(isotropicstandardelasticity3 umatisotropicstandardelasticity11)

castemtest_standardelasticitybrick(isotropicstandardelasticity4 umatisotropicstandardelasticity )
castemtest_standardelasticitybrick(isotropicstandardelasticity4 umatisotropicstandardelasticity2)
castemtest_standardelasticitybrick(isotropicstandardelasticity4 umatisotropicstandardelasticity3)
castemtest_standardelasticitybrick(isotropicstandardelasticity4 umatisotropicstandardelasticity4)
castemtest_standardelasticitybrick(isotropicstandardelasticity4 umatisotropicstandardelasticity5)
castemtest_standardelasticitybrick(isotropicstandardelasticity4 umatisotropicstandardelasticity6)
castemtest_standardelasticitybrick(isotropicstandardelasticity4 umatisotropicstandardelasticity7)
castemtest_standardelasticitybrick(isotropicstandardelasticity4 umatisotropicstandardelasticity8)
castemtest_standardelasticitybrick(isotropicstandardelasticity4 umatisotropicstandardelasticity9)
castemtest_standardelasticitybrick(isotropicstandardelasticity4 umatisotropicstandardelasticity10)

castemtest_standardelasticitybrick(isotropicstandardelasticity5 umatisotropicstandardelasticity12)
castemtest_standardelasticitybrick(isotropicstandardelasticity5 umatisotropicstandardelasticity13)
castemtest_standardelasticitybrick(isotropicstandardelasticity6 umatisotropicstandardelasticity12)
castemtest_standardelasticitybrick(isotropicstandardelasticity6 umatisotropicstandardelasticity13)
castemtest_standardelasticitybrick(isotropicstandardelasticity7 umatisotropicstandardelasticity12)
castemtest_standardelasticitybrick(isotropicstandardelasticity7 umatisotropicstandardelasticity13)

castemtest_standardelasticitybrick(orthotropicstandardelasticity umatorthotropicstandardelasticity )
castemtest_standardelasticitybrick(orthotropicstandardelasticity umatorthotropicstandardelasticity2)
castemtest_standardelasticitybrick(orthotropicstandardelasticity umatorthotropicstandardelasticity3)

castemtest_standardelasticitybrick(testcomputeelasticprediction  umattestcomputeelasticprediction)
castemtest_standardelasticitybrick(testcomputeelasticprediction2 umattestcomputeelasticprediction)
castemtest_standardelasticitybrick(testcomputeelasticprediction3 umattestcomputeelasticprediction2)
castemtest_standardelasticitybrick(testcomputeelasticprediction4 umattestcomputeelasticprediction2)

castemtest_standardelasticitybrick(isotropicj2plasticity
  umatisotropicj2plasticity plasticity.ref)
