set(TFELMTest_SOURCES
  RoundingMode.cxx
  Study.cxx
  StudyCurrentState.cxx
  StructureCurrentState.cxx
  CurrentState.cxx
  Solver.cxx
  SolverOptions.cxx
  GenericSolver.cxx
  Scheme.cxx
  SchemeBase.cxx
  SingleStructureScheme.cxx
  AnalyticalTest.cxx
  ReferenceFileComparisonTest.cxx
  Evolution.cxx
  CastemEvolution.cxx
  FunctionEvolution.cxx
  Behaviour.cxx
  BehaviourWorkSpace.cxx
  LogarithmicStrain1DBehaviourWrapper.cxx
  UmatBehaviourBase.cxx
  UmatNormaliseTangentOperator.cxx
  Constraint.cxx
  NonLinearConstraint.cxx
  ImposedDrivingVariable.cxx
  ImposedThermodynamicForce.cxx
  AccelerationAlgorithmFactory.cxx
  AccelerationAlgorithm.cxx
  CastemAccelerationAlgorithm.cxx
  AlternateSecantAccelerationAlgorithm.cxx
  AlternateDelta2AccelerationAlgorithm.cxx
  Alternate2DeltaAccelerationAlgorithm.cxx
  CrossedSecantAccelerationAlgorithm.cxx
  CrossedDelta2AccelerationAlgorithm.cxx
  Crossed2DeltaAccelerationAlgorithm.cxx
  Crossed2DeltabisAccelerationAlgorithm.cxx
  SecantAccelerationAlgorithm.cxx
  SteffensenAccelerationAlgorithm.cxx
  IronsTuckAccelerationAlgorithm.cxx
  UAndersonAccelerationAlgorithm.cxx
  FAndersonAccelerationAlgorithm.cxx
  MTest.cxx
  PipeTest.cxx
  PipeLinearElement.cxx
  PipeQuadraticElement.cxx
  PipeCubicElement.cxx
  PipeProfile.cxx
  PipeProfileHandler.cxx
  TextDataUtilities.cxx
  SchemeParserBase.cxx
  SingleStructureSchemeParser.cxx
  MTestParser.cxx
  PipeTestParser.cxx
  GasEquationOfState.cxx)

set(TFELMTest_LDADD
  MFrontLogStream
  TFELMaterial
  TFELMathParser
  TFELMath
  TFELSystem
  TFELUtilities
  TFELException
  TFELTests
  TFELConfig)

if(HAVE_CASTEM)
  set(TFELMTest_SOURCES
    CastemStandardBehaviour.cxx
    CastemSmallStrainBehaviour.cxx
    CastemFiniteStrainBehaviour.cxx
    CastemFiniteStrainBehaviour2.cxx
    CastemCohesiveZoneModel.cxx
    CastemUmatStandardBehaviour.cxx
    CastemUmatSmallStrainBehaviour.cxx
    CastemUmatFiniteStrainBehaviour.cxx
    MistralBehaviour.cxx
    ${TFELMTest_SOURCES})
  set(TFELMTest_LDADD
    CastemInterface
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

if(HAVE_EUROPLEXUS)
set(TFELMTest_SOURCES
    EuroplexusStandardBehaviour.cxx
    EuroplexusFiniteStrainBehaviour.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    EuroplexusInterface
    ${TFELMTest_LDADD})
endif(HAVE_EUROPLEXUS)

if(HAVE_ABAQUS)
set(TFELMTest_SOURCES
    AbaqusStandardBehaviour.cxx
    AbaqusSmallStrainBehaviour.cxx
    AbaqusFiniteStrainBehaviour.cxx
    AbaqusExplicitBehaviour.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    AbaqusInterface
    ${TFELMTest_LDADD})
endif(HAVE_ABAQUS)

if(HAVE_CALCULIX)
set(TFELMTest_SOURCES
    CalculiXStandardBehaviour.cxx
    CalculiXSmallStrainBehaviour.cxx
    CalculiXFiniteStrainBehaviour.cxx
    ${TFELMTest_SOURCES})
endif(HAVE_CALCULIX)

if(HAVE_ANSYS)
set(TFELMTest_SOURCES
    AnsysStandardBehaviour.cxx
    AnsysSmallStrainBehaviour.cxx
    AnsysFiniteStrainBehaviour.cxx
    AnsysNormaliseTangentOperator.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    AnsysInterface
    ${TFELMTest_LDADD})
endif(HAVE_ANSYS)

if(HAVE_CYRANO)
set(TFELMTest_SOURCES
    CyranoBehaviour.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
    CyranoInterface
    ${TFELMTest_LDADD})
endif(HAVE_CYRANO)

mtest_library(TFELMTest ${TFELMTest_SOURCES})
target_include_directories(TFELMTest
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mtest/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)

target_link_libraries(TFELMTest PUBLIC ${TFELMTest_LDADD})

add_executable(mtest MTestMain.cxx)
set_property(TARGET mtest PROPERTY POSITION_INDEPENDENT_CODE TRUE)
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
  if(HAVE_ABAQUS)
    list(APPEND mtest_static_link_libraries AbaqusInterface-static)
  endif(HAVE_ABAQUS)
  if(HAVE_CYRANO)
    list(APPEND mtest_static_link_libraries CyranoInterface-static)
  endif(HAVE_CYRANO)
  target_link_libraries(mtest ${mtest_static_link_libraries})
  set_target_properties(mtest PROPERTIES COMPILE_FLAGS "-DTFEL_STATIC_BUILD")
  set_target_properties(mtest PROPERTIES LINK_FLAGS "-static-libgcc -static") 
  set_target_properties(mtest PROPERTIES LINK_SEARCH_END_STATIC ON) 
else(WIN32 AND enable-static)
  target_link_libraries(mtest TFELMTest TFELSystem TFELUtilities TFELTests)
endif(WIN32 AND enable-static)

if(TFEL_APPEND_SUFFIX)
  set_target_properties(mtest
    PROPERTIES OUTPUT_NAME "mtest-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)

install(TARGETS mtest DESTINATION bin
  COMPONENT mtest)
