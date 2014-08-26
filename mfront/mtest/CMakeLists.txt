include_directories("${PROJECT_BINARY_DIR}/src/TFELConfig"
                    "${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/mfront/include")
link_directories("${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
                 "${PROJECT_BINARY_DIR}/src/Tests"
		 "${PROJECT_BINARY_DIR}/mfront/src"
		 "${PROJECT_BINARY_DIR}/mfront/mtest")

set(TFELMTest_SOURCES
    MTest.cxx
    MTestParser.cxx
    MTestAnalyticalTest.cxx
    MTestReferenceFileComparisonTest.cxx
    MTestEvolution.cxx
    MTestCastemEvolution.cxx
    MTestFunctionEvolution.cxx
    MTestBehaviour.cxx
    MTestUmatBehaviourBase.cxx
    MTestUmatNormaliseTangentOperator.cxx
    MTestConstraint.cxx
    MTestImposedDrivingVariable.cxx
    MTestImposedThermodynamicForce.cxx
    MTestAccelerationAlgorithmFactory.cxx
    MTestAccelerationAlgorithm.cxx
    MTestCastemAccelerationAlgorithm.cxx
    MTestSecantAccelerationAlgorithm.cxx
    MTestSteffensenAccelerationAlgorithm.cxx
    MTestIronsTuckAccelerationAlgorithm.cxx)

set(TFELMTest_LDADD
    MFrontLogStream
    TFELMaterial
    TFELMathParser
    TFELMath
    TFELSystem
    TFELUtilities
    TFELException
    TFELTests)

if(HAVE_CASTEM)
set(TFELMTest_SOURCES
    MTestUmatStandardBehaviour.cxx
    MTestUmatSmallStrainBehaviour.cxx
    MTestUmatFiniteStrainBehaviour.cxx
    MTestUmatCohesiveZoneModelBehaviour.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    UMATInterface
    ${TFELMTest_LDADD})
endif(HAVE_CASTEM)

if(HAVE_ASTER)
set(TFELMTest_SOURCES
    MTestAsterSmallStrainBehaviour.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    AsterInterface
    ${TFELMTest_LDADD})
endif(HAVE_ASTER)

if(HAVE_CYRANO)
set(TFELMTest_SOURCES
    MTestCyranoBehaviour.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    CyranoInterface
    ${TFELMTest_LDADD})
endif(HAVE_CYRANO)

tfel_library(TFELMTest ${TFELMTest_SOURCES})
target_link_libraries(TFELMTest ${TFELMTest_LDADD})

add_executable(mtest MTestMain.cxx)
if(WIN32 AND enable-static)
  target_link_libraries(mtest
    TFELMTest-static
    MFrontLogStream-static    
    TFELMathParser-static
    TFELMath-static
    TFELSystem-static
    TFELUtilities-static
    TFELException-static
    AsterInterface-static
    UMATInterface-static
    TFELTests-static)
  set_target_properties(mtest PROPERTIES COMPILE_FLAGS "-DTFEL_STATIC_BUILD")
  set_target_properties(mtest PROPERTIES LINK_FLAGS "-static-libgcc -static") 
  set_target_properties(mtest PROPERTIES LINK_SEARCH_END_STATIC ON) 
else(WIN32 AND enable-static)
  target_link_libraries(mtest TFELMTest TFELSystem)
endif(WIN32 AND enable-static)

install(TARGETS mtest DESTINATION bin)
