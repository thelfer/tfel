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
    IsotropicPlasticity)
  target_link_libraries(MFrontCastemBehaviourBricks
    CastemInterface MFrontProfiling TFELMaterial
    TFELMath TFELUtilities TFELException)
endif(HAVE_CASTEM)

macro(castemtest_brick test_arg behaviour)
  if(HAVE_CASTEM)
    set(_REFERENCE_FILE )
    if(NOT (${ARGC} EQUAL 0))
      set(_REFERENCE_FILE "${ARGN}")
    endif()
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
    if(NOT _REFERENCE_FILE)
      add_test(NAME brick${test_arg}${behaviour}_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
	--@library@="$<TARGET_FILE:MFrontCastemBehaviourBricks>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}.xml" ${file})
      add_test(NAME brick${test_arg}${behaviour}_consistenttangentoperator_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
	--@StiffnessMatrixType='ConsistentTangentOperator'
	--@library@="$<TARGET_FILE:MFrontCastemBehaviourBricks>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}.xml" ${file})
    else(NOT _REFERENCE_FILE)
      add_test(NAME brick${test_arg}${behaviour}_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
	--@library@="$<TARGET_FILE:MFrontCastemBehaviourBricks>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}-consistenttangentoperator.xml"
	--@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}" ${file})
      add_test(NAME brick${test_arg}${behaviour}_consistenttangentoperator_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
	--@StiffnessMatrixType='ConsistentTangentOperator'
	--@library@="$<TARGET_FILE:MFrontCastemBehaviourBricks>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}-consistenttangentoperator.xml"
	--@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}" ${file})
    endif(NOT _REFERENCE_FILE)
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST brick${test_arg}${behaviour}_mtest
	PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
      set_property(TEST brick${test_arg}${behaviour}_consistenttangentoperator_mtest
	PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
    elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST brick${test_arg}${behaviour}_mtest
	PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
      set_property(TEST brick${test_arg}${behaviour}_consistenttangentoperator_mtest
	PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    if(TFEL_APPEND_VERSION)
      install(FILES ${file}
	DESTINATION "share/doc/mfront-${TFEL_VERSION}/tests/behaviours/bricks"
	COMPONENT mtest)
    else(TFEL_APPEND_VERSION)
      install(FILES ${file}
	DESTINATION "share/doc/mfront/tests/behaviours/bricks"
	COMPONENT mtest)
    endif(TFEL_APPEND_VERSION)
  endif(HAVE_CASTEM)
endmacro(castemtest_brick)

castemtest_brick(isotropicstandardelasticity umatisotropicstandardelasticity )
castemtest_brick(isotropicstandardelasticity umatisotropicstandardelasticity2)
castemtest_brick(isotropicstandardelasticity umatisotropicstandardelasticity3)
castemtest_brick(isotropicstandardelasticity umatisotropicstandardelasticity4)
castemtest_brick(isotropicstandardelasticity umatisotropicstandardelasticity5)
castemtest_brick(isotropicstandardelasticity umatisotropicstandardelasticity6)
castemtest_brick(isotropicstandardelasticity umatisotropicstandardelasticity7)
castemtest_brick(isotropicstandardelasticity umatisotropicstandardelasticity8)
castemtest_brick(isotropicstandardelasticity umatisotropicstandardelasticity9)
castemtest_brick(isotropicstandardelasticity umatisotropicstandardelasticity10)

castemtest_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity )
castemtest_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity2)
castemtest_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity3)
castemtest_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity4)
castemtest_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity5)
castemtest_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity6)
castemtest_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity7)
castemtest_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity8)
castemtest_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity9)
castemtest_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity10)

castemtest_brick(isotropicstandardelasticity3 umatisotropicstandardelasticity11)

castemtest_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity )
castemtest_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity2)
castemtest_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity3)
castemtest_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity4)
castemtest_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity5)
castemtest_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity6)
castemtest_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity7)
castemtest_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity8)
castemtest_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity9)
castemtest_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity10)

castemtest_brick(isotropicstandardelasticity5 umatisotropicstandardelasticity12)
castemtest_brick(isotropicstandardelasticity5 umatisotropicstandardelasticity13)
castemtest_brick(isotropicstandardelasticity6 umatisotropicstandardelasticity12)
castemtest_brick(isotropicstandardelasticity6 umatisotropicstandardelasticity13)
castemtest_brick(isotropicstandardelasticity7 umatisotropicstandardelasticity12)
castemtest_brick(isotropicstandardelasticity7 umatisotropicstandardelasticity13)

castemtest_brick(orthotropicstandardelasticity umatorthotropicstandardelasticity )
castemtest_brick(orthotropicstandardelasticity umatorthotropicstandardelasticity2)
castemtest_brick(orthotropicstandardelasticity umatorthotropicstandardelasticity3)

castemtest_brick(testcomputeelasticprediction  umattestcomputeelasticprediction)
castemtest_brick(testcomputeelasticprediction2 umattestcomputeelasticprediction)
castemtest_brick(testcomputeelasticprediction3 umattestcomputeelasticprediction2)
castemtest_brick(testcomputeelasticprediction4 umattestcomputeelasticprediction2)

castemtest_brick(isotropicplasticity umatisotropicplasticity plasticity.ref)
