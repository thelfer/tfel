include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/mfront/include")
if(HAVE_CASTEM)
  if(CASTEHOME)
    include_directories("${CASTEMHOME}/include")
    include_directories("${CASTEMHOME}/include/c")
  endif(CASTEHOME)
endif(HAVE_CASTEM)

link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Glossary"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
		 "${PROJECT_BINARY_DIR}/src/Config")

configure_file(${PROJECT_SOURCE_DIR}/mfront/src/MFrontHeader.cxx.in
               ${PROJECT_BINARY_DIR}/mfront/src/MFrontHeader.cxx)

# MFrontLogStream

mfront_library(MFrontLogStream
  MFrontLogStream.cxx)

# MFrontLogProfiling

mfront_library(MFrontProfiling
  BehaviourProfiler.cxx)
if(UNIX AND (NOT APPLE))
  target_link_libraries(MFrontProfiling rt)
endif(UNIX AND (NOT APPLE))

# TFELMFront

set(TFELMFront_SOURCES
    MFront.cxx
    MFrontBase.cxx
    MFrontUtilities.cxx
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

if(HAVE_ANSYS)
  set(TFELMFront_SOURCES
    AnsysInterface.cxx
    ${TFELMFront_SOURCES})
  set(AnsysInterface_SOURCES
    AnsysInterfaceExceptions.cxx
    AnsysException.cxx
    AnsysComputeStiffnessTensor.cxx
    AnsysStressFreeExpansionHandler.cxx
    AnsysComputeThermalExpansionCoefficientTensor.cxx
    AnsysFiniteStrain.cxx)
  mfront_library(AnsysInterface ${AnsysInterface_SOURCES})
  target_link_libraries(AnsysInterface
    MTestFileGenerator
    TFELUtilities
    TFELException)
endif(HAVE_ANSYS)

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

if(UNIX)
  target_link_libraries(TFELMFront
    MFrontLogStream
    TFELMaterial TFELMathParser
    TFELSystem   TFELUtilities
    TFELGlossary TFELException
    TFELConfig
    -pthread ${TFEL_DL_LIBRARY})
else(UNIX)
  target_link_libraries(TFELMFront
    MFrontLogStream
    TFELMaterial TFELMathParser
    TFELSystem   TFELUtilities
    TFELGlossary TFELException
    TFELConfig)
endif(UNIX)

# MTestFileGenerator

mfront_library(MTestFileGenerator
  MTestFileGeneratorBase.cxx
  UmatSmallStrainMTestFileGenerator.cxx
  UmatFiniteStrainMTestFileGenerator.cxx)
target_link_libraries(MTestFileGenerator
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

# MFront

add_executable(mfront main.cxx)
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
target_link_libraries(mfront-query
  TFELMFront 
  MFrontLogStream
  TFELSystem TFELUtilities)
install(TARGETS mfront-query DESTINATION bin
  COMPONENT mfront)

# mfront-doc
add_executable(mfront-doc mfront-doc.cxx
  BehaviourDocumentationGenerator.cxx)
target_link_libraries(mfront-doc
  TFELMFront 
  MFrontLogStream
  TFELSystem TFELUtilities)
install(TARGETS mfront-doc DESTINATION bin
  COMPONENT mfront)
