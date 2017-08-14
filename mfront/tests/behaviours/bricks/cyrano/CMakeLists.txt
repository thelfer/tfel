include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

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
  mfront_behaviour_check_library(MFrontCyranoBehaviourBricks cyrano
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
    IsotropicPlasticity)
  target_link_libraries(MFrontCyranoBehaviourBricks
    CyranoInterface MFrontProfiling TFELMaterial
    TFELMath TFELUtilities TFELException)
endif(HAVE_CYRANO)

macro(cyranotest_brick test_arg behaviour)
  if(HAVE_CYRANO)
    set(_REFERENCE_FILE )
    if(NOT (${ARGC} EQUAL 0))
      set(_REFERENCE_FILE "${ARGN}")
    endif()
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
    if(NOT _REFERENCE_FILE)
      add_test(NAME brick${test_arg}${behaviour}_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
	--@library@="$<TARGET_FILE:MFrontCyranoBehaviourBricks>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}.xml" ${file})
    else(NOT _REFERENCE_FILE)
      add_test(NAME brick${test_arg}${behaviour}_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
	--@library@="$<TARGET_FILE:MFrontCyranoBehaviourBricks>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}.xml"
	--@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}"
	${file})
    endif(NOT _REFERENCE_FILE)
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST brick${test_arg}${behaviour}_mtest
	PROPERTY DEPENDS "MFrontCyranoBehaviours MFrontMaterialProperties-cyrano mtest"
	PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
    elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST brick${test_arg}${behaviour}_mtest
	PROPERTY DEPENDS "MFrontCyranoBehaviours MFrontMaterialProperties-cyrano mtest")
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
  endif(HAVE_CYRANO)
endmacro(cyranotest_brick)

cyranotest_brick(isotropicstandardelasticity cyranoisotropicstandardelasticity )
cyranotest_brick(isotropicstandardelasticity cyranoisotropicstandardelasticity3)
cyranotest_brick(isotropicstandardelasticity cyranoisotropicstandardelasticity4)
cyranotest_brick(isotropicstandardelasticity cyranoisotropicstandardelasticity5)
cyranotest_brick(isotropicstandardelasticity cyranoisotropicstandardelasticity6)
cyranotest_brick(isotropicstandardelasticity cyranoisotropicstandardelasticity8)
cyranotest_brick(isotropicstandardelasticity cyranoisotropicstandardelasticity9)
cyranotest_brick(isotropicstandardelasticity cyranoisotropicstandardelasticity10)

cyranotest_brick(testcomputeelasticprediction  cyranotestcomputeelasticprediction)
cyranotest_brick(testcomputeelasticprediction2 cyranotestcomputeelasticprediction)
cyranotest_brick(testcomputeelasticprediction3 cyranotestcomputeelasticprediction2)
cyranotest_brick(testcomputeelasticprediction4 cyranotestcomputeelasticprediction2)
