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
    AnalyticalTest.cxx
    ReferenceFileComparisonTest.cxx
    Evolution.cxx
    CastemEvolution.cxx
    FunctionEvolution.cxx
    Behaviour.cxx
    UmatBehaviourBase.cxx
    UmatNormaliseTangentOperator.cxx
    Constraint.cxx
    ImposedDrivingVariable.cxx
    ImposedThermodynamicForce.cxx
    AccelerationAlgorithmFactory.cxx
    AccelerationAlgorithm.cxx
    CastemAccelerationAlgorithm.cxx
    SecantAccelerationAlgorithm.cxx
    SteffensenAccelerationAlgorithm.cxx
    IronsTuckAccelerationAlgorithm.cxx)

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
    UmatStandardBehaviour.cxx
    UmatSmallStrainBehaviour.cxx
    UmatFiniteStrainBehaviour.cxx
    UmatCohesiveZoneModel.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    UMATInterface
    ${TFELMTest_LDADD})
endif(HAVE_CASTEM)

if(HAVE_ASTER)
set(TFELMTest_SOURCES
    AsterStandardBehaviour.cxx
    AsterSmallStrainBehaviour.cxx
    AsterFiniteStrainBehaviour.cxx
    AsterCohesiveZoneModel.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    AsterInterface
    ${TFELMTest_LDADD})
endif(HAVE_ASTER)

if(HAVE_CYRANO)
set(TFELMTest_SOURCES
    CyranoBehaviour.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    CyranoInterface
    ${TFELMTest_LDADD})
endif(HAVE_CYRANO)

mtest_library(TFELMTest ${TFELMTest_SOURCES})
target_link_libraries(TFELMTest ${TFELMTest_LDADD})

add_executable(mtest MTestMain.cxx)
if(WIN32 AND enable-static)
  set(mtest_static_link_libraries
    TFELMTest-static
    MFrontLogStream-static    
    TFELMaterial-static
    TFELMathParser-static
    TFELMath-static
    TFELSystem-static
    TFELUtilities-static
    TFELException-static
    TFELTests-static)
  if(HAVE_CASTEM)
    list(APPEND mtest_static_link_libraries UMATInterface-static)
  endif(HAVE_CASTEM)
  if(HAVE_ASTER)
    list(APPEND mtest_static_link_libraries AsterInterface-static)
  endif(HAVE_ASTER)
  if(HAVE_CYRANO)
    list(APPEND mtest_static_link_libraries CyranoInterface-static)
  endif(HAVE_CYRANO)
  target_link_libraries(mtest ${mtest_static_link_libraries})
  set_target_properties(mtest PROPERTIES COMPILE_FLAGS "-DTFEL_STATIC_BUILD")
  set_target_properties(mtest PROPERTIES LINK_FLAGS "-static-libgcc -static") 
  set_target_properties(mtest PROPERTIES LINK_SEARCH_END_STATIC ON) 
else(WIN32 AND enable-static)
  target_link_libraries(mtest TFELMTest TFELSystem)
endif(WIN32 AND enable-static)

install(TARGETS mtest DESTINATION bin
  COMPONENT mtest)
