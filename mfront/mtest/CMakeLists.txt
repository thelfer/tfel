include_directories("${PROJECT_BINARY_DIR}/src/TFELConfig"
                    "${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/mfront/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
                 "${PROJECT_BINARY_DIR}/src/Tests"
		 "${PROJECT_BINARY_DIR}/mfront/src"
		 "${PROJECT_BINARY_DIR}/mfront/mtest")

set(TFELMTest_SOURCES
    MTest.cxx
    MTestAnalyticalTest.cxx
    MTestReferenceFileComparisonTest.cxx
    MTestLogStream.cxx
    MTestEvolution.cxx
    MTestCastemEvolution.cxx
    MTestFunctionEvolution.cxx
    MTestBehaviour.cxx
    MTestUmatBehaviourBase.cxx
    MTestUmatNormaliseTangentOperator.cxx
    MTestConstraint.cxx
    MTestImposedStrain.cxx
    MTestImposedStress.cxx)

set(mtest_SOURCES mtest.cxx)
set(TFELMTest_LDADD
    TFELMathParser
    TFELMath
    TFELSystem
    TFELUtilities
    TFELException
    TFELTests)

if(HAVE_CASTEM)
set(TFELMTest_SOURCES
    MTestUmatBehaviour.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    UMATInterface
    ${TFELMTest_LDADD})
endif(HAVE_CASTEM)

if(HAVE_ASTER)
set(TFELMTest_SOURCES
    MTestAsterBehaviour.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    AsterInterface
    ${TFELMTest_LDADD})
endif(HAVE_ASTER)

tfel_library(TFELMTest ${TFELMTest_SOURCES})
target_link_libraries(TFELMTest ${TFELMTest_LDADD})

add_executable(mtest mtest.cxx)
target_link_libraries(mtest TFELMTest)
install(TARGETS mtest DESTINATION bin)
