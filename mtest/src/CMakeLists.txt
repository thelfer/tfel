set(TFELMTest_SOURCES
  MTestFileExport.cxx
  MaterialProperty.cxx
  MaterialPropertyBase.cxx
  CastemMaterialProperty.cxx
  CyranoMaterialProperty.cxx
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
  CyranoEvolution.cxx
  FunctionEvolution.cxx
  Behaviour.cxx
  BehaviourWrapperBase.cxx
  SmallStrainTridimensionalBehaviourWrapper.cxx
  LogarithmicStrain1DBehaviourWrapper.cxx
  BehaviourWorkSpace.cxx
  StandardBehaviourBase.cxx
  GenericBehaviour.cxx
  UmatNormaliseTangentOperator.cxx
  Constraint.cxx
  ConstraintBase.cxx
  NonLinearConstraint.cxx
  ImposedGradient.cxx
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
  UserDefinedPostProcessing.cxx
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
    AsterLogarithmicStrainBehaviourWrapper.cxx
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

if(HAVE_DIANAFEA)
  set(TFELMTest_SOURCES
    DianaFEASmallStrainBehaviour.cxx
    ${TFELMTest_SOURCES})
set(TFELMTest_LDADD
  DianaFEAInterface
  ${TFELMTest_LDADD})
endif(HAVE_DIANAFEA)

mtest_library(TFELMTest ${TFELMTest_SOURCES})
target_include_directories(TFELMTest
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/mtest/include>
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/mfront/include>
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(TFELMTest PUBLIC ${TFELMTest_LDADD})

if(TFEL_HAVE_MADNEX)
  target_compile_definitions(TFELMTest
    PUBLIC MTEST_HAVE_MADNEX)
  target_link_libraries(TFELMTest
    PRIVATE madnex::Madnex)
endif(TFEL_HAVE_MADNEX)

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
