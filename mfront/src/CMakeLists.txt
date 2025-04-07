configure_file(${PROJECT_SOURCE_DIR}/mfront/src/MFrontHeader.cxx.in
               ${PROJECT_BINARY_DIR}/mfront/src/MFrontHeader.cxx)

# MFrontLogStream

mfront_library(MFrontLogStream
  MFrontLogStream.cxx)
target_include_directories(MFrontLogStream
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)

# MFrontLogProfiling

mfront_library(MFrontProfiling
  BehaviourProfiler.cxx)
target_include_directories(MFrontProfiling
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
if(UNIX AND (NOT APPLE) AND (NOT HAIKU) AND (NOT (CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")))
   target_link_libraries(MFrontProfiling PRIVATE rt)
endif(UNIX AND (NOT APPLE) AND (NOT HAIKU) AND (NOT (CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")))

# TFELMFront

set(TFELMFront_SOURCES
    MFront.cxx
    MFrontBase.cxx
    MFrontUtilities.cxx
    GeneratorOptions.cxx
    MakefileGenerator.cxx
    CodeBlock.cxx
    FileDescription.cxx
    TargetsDescription.cxx
    LibraryDescription.cxx
    MFrontDebugMode.cxx
    PedanticMode.cxx
    MFrontLock.cxx
    SearchFile.cxx
    InitInterfaces.cxx
    InitDSLs.cxx
    DSLFactory.cxx
    DrivingVariable.cxx
    ThermodynamicForce.cxx
    LawFunction.cxx
    MaterialPropertyDescription.cxx
    MaterialPropertyParametersHandler.cxx
    BehaviourData.cxx
    BehaviourDescription.cxx
    BoundsDescription.cxx
    VariableDescription.cxx
    StaticVariableDescription.cxx
    VariableBoundsDescription.cxx
    BehaviourDSLCommon.cxx
    ModelDescription.cxx
    ModelDSLCommon.cxx
    ModelDSL.cxx
    AbstractDSL.cxx
    AbstractBehaviourDSL.cxx
    AbstractBehaviourInterface.cxx
    AbstractMaterialPropertyInterface.cxx
    AbstractModelInterface.cxx
    SupportedTypes.cxx
    DSLBase.cxx
    InterfaceBase.cxx
    DSLUtilities.cxx
    FiniteStrainBehaviourTangentOperatorConversion.cxx
    FiniteStrainBehaviourTangentOperatorConversionPath.cxx
    MaterialPropertyInterfaceFactory.cxx
    BehaviourInterfaceFactory.cxx
    ModelInterfaceFactory.cxx
    DefaultDSLBase.cxx
    DefaultDSL.cxx
    DefaultCZMDSL.cxx
    DefaultFiniteStrainDSL.cxx
    MaterialPropertyDSL.cxx
    IsotropicMisesCreepDSL.cxx
    IsotropicStrainHardeningMisesCreepDSL.cxx
    IsotropicMisesPlasticFlowDSL.cxx
    RungeKuttaDSLBase.cxx
    RungeKuttaDSL.cxx
    RungeKuttaFiniteStrainDSL.cxx
    ImplicitDSLBase.cxx
    ImplicitDSL.cxx
    ImplicitDSLII.cxx
    ImplicitFiniteStrainDSL.cxx
    NonLinearSystemSolver.cxx
    NonLinearSystemSolverBase.cxx
    NonLinearSystemSolverFactory.cxx
    PowellDogLegAlgorithmBase.cxx
    NewtonRaphsonSolvers.cxx
    BroydenSolvers.cxx
    SecondBroydenSolver.cxx
    LevenbergMarquardtSolvers.cxx
    AbstractBehaviourBrick.cxx
    AbstractBehaviourBrickFactory.cxx
    BehaviourBrickProviderIdentifier.cxx
    BehaviourBrickProvider.cxx
    BehaviourBrickRequirement.cxx
    BehaviourBrickRequirementManager.cxx
    BehaviourBrickBase.cxx
    StandardElasticityBrick.cxx
    DDIF2Brick.cxx
    FiniteStrainSingleCrystalBrick.cxx
    LocalDataStructure.cxx
    PerformanceProfiling.cxx
    MultipleIsotropicMisesFlowsDSL.cxx
    IsotropicBehaviourDSLBase.cxx
    MFrontMaterialPropertyInterface.cxx
    CMaterialPropertyInterfaceBase.cxx
    CMaterialPropertyInterface.cxx
    CppMaterialPropertyInterface.cxx
    CppTestMaterialPropertyInterface.cxx
    ExcelMaterialPropertyInterface.cxx
    OctaveMaterialPropertyInterface.cxx
    ExcelMaterialPropertyInternalInterface.cxx
    MFrontModelInterface.cxx
    UMATInterfaceBase.cxx
    ${PROJECT_BINARY_DIR}/mfront/src/MFrontHeader.cxx)

if(HAVE_GNUPLOT)
    set(TFELMFront_SOURCES
	GnuplotMaterialPropertyInterface.cxx
	${TFELMFront_SOURCES})
endif(HAVE_GNUPLOT)

if(HAVE_PYTHON)
    set(TFELMFront_SOURCES
	PythonMaterialPropertyInterface.cxx
	${TFELMFront_SOURCES})
endif(HAVE_PYTHON)

if(HAVE_JAVA)
    set(TFELMFront_SOURCES
	JavaMaterialPropertyInterface.cxx
	${TFELMFront_SOURCES})
endif(HAVE_JAVA)

if(HAVE_FORTRAN)
    set(TFELMFront_SOURCES
      FortranMaterialPropertyInterface.cxx
      Fortran03MaterialPropertyInterface.cxx
      ${TFELMFront_SOURCES})
endif(HAVE_FORTRAN)

if(HAVE_CASTEM)
    set(TFELMFront_SOURCES
	CastemInterface.cxx
	CastemMaterialPropertyInterface.cxx
	${TFELMFront_SOURCES})
    set(CastemInterface_SOURCES
	CastemInterfaceExceptions.cxx
	CastemFiniteStrain.cxx
	CastemComputeStiffnessTensor.cxx
	CastemComputeThermalExpansionCoefficientTensor.cxx
	CastemRotationMatrix.cxx
	CastemTangentOperator.cxx
	CastemOutOfBoundsPolicy.cxx
	CastemGetModellingHypothesis.cxx
	CastemStressFreeExpansionHandler.cxx
	CastemException.cxx)
    mfront_library(CastemInterface ${CastemInterface_SOURCES})
    target_link_libraries(CastemInterface
      MTestFileGenerator
      TFELUtilities
      TFELException)
endif(HAVE_CASTEM)

if(HAVE_ASTER)
  set(TFELMFront_SOURCES
    AsterInterface.cxx
    ${TFELMFront_SOURCES})
  set(AsterInterface_SOURCES
    AsterInterfaceExceptions.cxx
    AsterException.cxx
    AsterRotationMatrix.cxx
    AsterFiniteStrainRotationMatrix.cxx
    AsterComputeStiffnessTensor.cxx
    AsterStressFreeExpansionHandler.cxx
    AsterComputeThermalExpansionCoefficientTensor.cxx
    AsterTangentOperator.cxx)
  mfront_library(AsterInterface ${AsterInterface_SOURCES})
  target_link_libraries(AsterInterface
    MTestFileGenerator
    TFELUtilities
    TFELException)
endif(HAVE_ASTER)

if(HAVE_ABAQUS)
  set(TFELMFront_SOURCES
    AbaqusInterfaceBase.cxx
    AbaqusInterface.cxx
    AbaqusExplicitInterface.cxx
    ${TFELMFront_SOURCES})
  set(AbaqusInterface_SOURCES
    AbaqusInterfaceExceptions.cxx
    AbaqusException.cxx
    AbaqusComputeStiffnessTensor.cxx
    AbaqusStressFreeExpansionHandler.cxx
    AbaqusComputeThermalExpansionCoefficientTensor.cxx
    AbaqusFiniteStrain.cxx)
  mfront_library(AbaqusInterface ${AbaqusInterface_SOURCES})
  target_link_libraries(AbaqusInterface
    MTestFileGenerator
    TFELUtilities
    TFELException)
endif(HAVE_ABAQUS)

if(HAVE_EUROPLEXUS)
  set(TFELMFront_SOURCES
    EuroplexusInterface.cxx
    ${TFELMFront_SOURCES})
  set(EuroplexusInterface_SOURCES
    EuroplexusInterfaceExceptions.cxx
    EuroplexusException.cxx
    EuroplexusComputeStiffnessTensor.cxx
    EuroplexusStressFreeExpansionHandler.cxx
    EuroplexusComputeThermalExpansionCoefficientTensor.cxx
    EuroplexusFiniteStrain.cxx)
  mfront_library(EuroplexusInterface ${EuroplexusInterface_SOURCES})
  target_link_libraries(EuroplexusInterface
    MTestFileGenerator
    TFELUtilities
    TFELException)
endif(HAVE_EUROPLEXUS)

if(HAVE_ZMAT)
  set(TFELMFront_SOURCES
    ZMATInterface.cxx
    ${TFELMFront_SOURCES})
endif(HAVE_ZMAT)

if(HAVE_CYRANO)
  set(TFELMFront_SOURCES
    CyranoInterface.cxx
    ${TFELMFront_SOURCES})
  set(CyranoInterface_SOURCES
    CyranoException.cxx
    CyranoInterfaceExceptions.cxx
    CyranoComputeStiffnessTensor.cxx
    CyranoComputeThermalExpansionCoefficientTensor.cxx
    CyranoOutOfBoundsPolicy.cxx
    CyranoGetModellingHypothesis.cxx)
  mfront_library(CyranoInterface ${CyranoInterface_SOURCES})
  target_link_libraries(CyranoInterface
    MTestFileGenerator
    TFELUtilities
    TFELException)
endif(HAVE_CYRANO)

mfront_library(TFELMFront ${TFELMFront_SOURCES})
target_include_directories(TFELMFront
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
   $<INSTALL_INTERFACE:include>)
if(HAVE_CASTEM)
  if(CASTEHOME)
    target_include_directories(TFELMFront 
      PRIVATE "${CASTEMHOME}/include"
      PRIVATE "${CASTEMHOME}/include/c")
  endif(CASTEHOME)
endif(HAVE_CASTEM)

target_link_libraries(TFELMFront
  PUBLIC MFrontLogStream
  PUBLIC TFELMaterial       PUBLIC TFELMathParser
  PUBLIC TFELSystem         PUBLIC TFELUtilities
  PUBLIC TFELGlossary       PUBLIC TFELException
  PUBLIC TFELConfig)

if(UNIX)
  if(NOT HAIKU)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
      target_link_libraries(TFELMFront
	    PRIVATE ${TFEL_DL_LIBRARY})
    else(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
      target_link_libraries(TFELMFront
      	PRIVATE -pthread PRIVATE ${TFEL_DL_LIBRARY})
    endif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
  else(NOT HAIKU)
    target_link_libraries(TFELMFront
      PRIVATE ${TFEL_DL_LIBRARY})
  endif(NOT HAIKU)
endif(UNIX)

# MTestFileGenerator

mfront_library(MTestFileGenerator
  MTestFileGeneratorBase.cxx
  UmatSmallStrainMTestFileGenerator.cxx
  UmatFiniteStrainMTestFileGenerator.cxx)
target_include_directories(MTestFileGenerator
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(MTestFileGenerator
  PUBLIC TFELMaterial
  PUBLIC TFELMath
  PUBLIC TFELUtilities
  PUBLIC TFELException)

# MFront

add_executable(mfront main.cxx)
set_property(TARGET mfront PROPERTY POSITION_INDEPENDENT_CODE TRUE)
if(TFEL_APPEND_VERSION)
  set_target_properties(mfront
 PROPERTIES OUTPUT_NAME "mfront-${TFEL_VERSION}")
endif(TFEL_APPEND_VERSION)
target_link_libraries(mfront
  TFELMFront 
  MFrontLogStream
  TFELSystem TFELUtilities)
install(TARGETS mfront DESTINATION bin
  COMPONENT mfront)

if(enable-cppcheck)
  add_cppcheck(mfront)
endif(enable-cppcheck)

# mfront-query
add_executable(mfront-query mfront-query.cxx
  MaterialPropertyQuery.cxx
  BehaviourQuery.cxx
  ModelQuery.cxx)
set_property(TARGET mfront-query PROPERTY POSITION_INDEPENDENT_CODE TRUE)
if(TFEL_APPEND_VERSION)
  set_target_properties(mfront-query
    PROPERTIES OUTPUT_NAME "mfront-query-${TFEL_VERSION}")
endif(TFEL_APPEND_VERSION)
target_link_libraries(mfront-query
  PUBLIC TFELMFront 
  PUBLIC MFrontLogStream
  PUBLIC TFELSystem
  PUBLIC TFELUtilities)
install(TARGETS mfront-query DESTINATION bin
  COMPONENT mfront)

# mfront-doc
add_executable(mfront-doc mfront-doc.cxx
  BehaviourDocumentationGenerator.cxx)
set_property(TARGET mfront-doc PROPERTY POSITION_INDEPENDENT_CODE TRUE)
if(TFEL_APPEND_VERSION)
  set_target_properties(mfront-doc
    PROPERTIES OUTPUT_NAME "mfront-doc-${TFEL_VERSION}")
endif(TFEL_APPEND_VERSION)
target_link_libraries(mfront-doc
  PUBLIC TFELMFront 
  PUBLIC MFrontLogStream
  PUBLIC TFELSystem
  PUBLIC TFELUtilities)
install(TARGETS mfront-doc DESTINATION bin
  COMPONENT mfront)
