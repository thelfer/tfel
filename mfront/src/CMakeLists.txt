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
if(enable-static)
  target_include_directories(MFrontLogStream-static
    PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
endif(enable-static)

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
if(enable-static)
  target_include_directories(MFrontProfiling-static
    PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
  if(UNIX AND (NOT APPLE) AND (NOT HAIKU) AND (NOT (CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")))
    target_link_libraries(MFrontProfiling-static PRIVATE rt)
  endif(UNIX AND (NOT APPLE) AND (NOT HAIKU) AND (NOT (CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")))
endif(enable-static)
# MTestFileGenerator
mfront_library(MTestFileGenerator
  MTestFileGeneratorBase.cxx
  UmatSmallStrainMTestFileGenerator.cxx
  UmatFiniteStrainMTestFileGenerator.cxx
  GenericBehaviourMTestFileGenerator.cxx
  GenericBehaviourSmallStrainMTestFileGenerator.cxx
  GenericBehaviourFiniteStrainMTestFileGenerator.cxx)
target_include_directories(MTestFileGenerator
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(MTestFileGenerator
  PUBLIC tfel::TFELMaterial
  PUBLIC tfel::TFELMath
  PUBLIC tfel::TFELUtilities
  PUBLIC tfel::TFELException)
if(enable-static)
    target_include_directories(MTestFileGenerator-static
    PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
    $<INSTALL_INTERFACE:include>)
  target_link_libraries(MTestFileGenerator-static
    PUBLIC tfel::TFELMaterial
    PUBLIC tfel::TFELMath
    PUBLIC tfel::TFELUtilities
    PUBLIC tfel::TFELException)
endif(enable-static)

# TFELMFront

set(TFELMFront_SOURCES
    MFront.cxx
    MFrontBase.cxx
    GlobalDomainSpecificLanguageOptionsManager.cxx
    MaterialKnowledgeDescription.cxx
    PathSpecifier.cxx
    MFrontUtilities.cxx
    GeneratorOptions.cxx
    CMakeGenerator.cxx
    MakefileGenerator.cxx
    CodeBlock.cxx
    FileDescription.cxx
    TargetsDescription.cxx
    OverridableImplementation.cxx
    CompiledTargetDescriptionBase.cxx
    LibraryDescription.cxx
    ExecutableDescription.cxx
    SpecificTargetDescription.cxx
    MFrontWarningMode.cxx
    MFrontDebugMode.cxx
    PedanticMode.cxx
    MFrontLock.cxx
    InstallPath.cxx
    SearchPathsHandler.cxx
    InitInterfaces.cxx
    InitDSLs.cxx
    DSLFactory.cxx
    DataInterpolationUtilities.cxx
    Gradient.cxx
    ThermodynamicForce.cxx
    LawFunction.cxx
    MaterialPropertyDescription.cxx
    MaterialPropertyParametersHandler.cxx
    BehaviourData.cxx
    BehaviourDescription.cxx
    VariableDescriptionBase.cxx
    VariableDescription.cxx
    BehaviourVariableDescription.cxx
    StaticVariableDescription.cxx
    VariableBoundsDescription.cxx
    BehaviourDSLCommon.cxx
    ModelDescription.cxx
    ModelDSLCommon.cxx
    ModelDSL.cxx
    BehaviourDSLDescription.cxx
    AbstractDSL.cxx
    AbstractBehaviourDSL.cxx
    AbstractBehaviourInterface.cxx
    AbstractMaterialPropertyInterface.cxx
    AbstractModelInterface.cxx
    SupportedTypes.cxx
    DSLBase.cxx
    InterfaceBase.cxx
    DSLUtilities.cxx
    BehaviourDSLUtilities.cxx
    CodeGeneratorUtilities.cxx
    FiniteStrainBehaviourTangentOperatorConversion.cxx
    FiniteStrainBehaviourTangentOperatorConversionPath.cxx
    MaterialPropertyInterfaceFactory.cxx
    BehaviourInterfaceFactory.cxx
    ModelInterfaceFactory.cxx
    DefaultDSLBase.cxx
    DefaultDSL.cxx
    DefaultCZMDSL.cxx
    DefaultFiniteStrainDSL.cxx
    DefaultGenericBehaviourDSL.cxx
    DefaultModelDSL.cxx
    MaterialPropertyDSL.cxx
    IsotropicMisesCreepDSL.cxx
    IsotropicStrainHardeningMisesCreepDSL.cxx
    IsotropicMisesPlasticFlowDSL.cxx
    RungeKuttaDSLBase.cxx
    RungeKuttaDSL.cxx
    RungeKuttaFiniteStrainDSL.cxx
    RungeKuttaGenericBehaviourDSL.cxx
    RungeKuttaModelDSL.cxx
    ImplicitDSLBase.cxx
    ImplicitDSL.cxx
    ImplicitDSLII.cxx
    ImplicitFiniteStrainDSL.cxx
    ImplicitGenericBehaviourDSL.cxx
    ImplicitModelDSL.cxx
    ImplicitCZMDSL.cxx
    NonLinearSystemSolver.cxx
    NonLinearSystemSolverBase.cxx
    NonLinearSystemSolverFactory.cxx
    UserDefinedNonLinearSystemSolver.cxx
    PowellDogLegAlgorithmBase.cxx
    NewtonRaphsonSolvers.cxx
    BroydenSolvers.cxx
    SecondBroydenSolver.cxx
    LevenbergMarquardtSolvers.cxx
    BehaviourBrickDescription.cxx
    AbstractBehaviourBrick.cxx
    BehaviourBrickFactory.cxx
    BehaviourBrickBase.cxx
    StandardElasticityBrick.cxx
    StandardElastoViscoPlasticityBrick.cxx
    DDIF2Brick.cxx
    FiniteStrainSingleCrystalBrick.cxx
    LocalDataStructure.cxx
    PerformanceProfiling.cxx
    MultipleIsotropicMisesFlowsDSL.cxx
    IsotropicBehaviourDSLBase.cxx
    AbstractBehaviourCodeGenerator.cxx
    BehaviourCodeGeneratorBase.cxx
    DefaultCodeGeneratorBase.cxx
    DefaultCZMCodeGenerator.cxx
    DefaultFiniteStrainCodeGenerator.cxx
    RungeKuttaCodeGeneratorBase.cxx
    ImplicitCodeGeneratorBase.cxx
    IsotropicBehaviourCodeGeneratorBase.cxx
    IsotropicStrainHardeningMisesCreepCodeGenerator.cxx
    IsotropicMisesPlasticFlowCodeGenerator.cxx
    IsotropicMisesCreepCodeGenerator.cxx
    MultipleIsotropicMisesFlowsCodeGenerator.cxx
    MFrontMaterialPropertyInterface.cxx
    MFrontBehaviourInterface.cxx
    CMaterialPropertyInterfaceBase.cxx
    CMaterialPropertyInterface.cxx
    CppMaterialPropertyInterface.cxx
    CppTestMaterialPropertyInterface.cxx
    ExcelMaterialPropertyInterface.cxx
    OctaveMaterialPropertyInterface.cxx
    ExcelMaterialPropertyInternalInterface.cxx
    MFrontModelInterface.cxx
    BehaviourMaterialProperty.cxx
    BehaviourInterfaceBase.cxx
    SymbolsGenerator.cxx
    UMATInterfaceBase.cxx
    GenericMaterialPropertyInterfaceBase.cxx
    GenericMaterialPropertyInterface.cxx
    GenericBehaviourInterface.cxx
    GenericBehaviourSymbolsGenerator.cxx
    GenericModelInterface.cxx
    OptionDescription.cxx
    BrickUtilities.cxx
    StressPotential.cxx
    HookeStressPotentialBase.cxx
    HookeStressPotential.cxx
    ScalarDamageHookeStressPotentialBase.cxx
    ScalarDamageHookeStressPotential.cxx
    DDIF2StressPotential.cxx
    StressPotentialFactory.cxx
    InelasticFlow.cxx
    InelasticFlowFactory.cxx
    InelasticFlowBase.cxx
    ViscoplasticFlowBase.cxx
    NortonHoffViscoplasticFlow.cxx
    HyperbolicSineViscoplasticFlow.cxx
    HarmonicSumOfNortonHoffViscoplasticFlows.cxx
    UserDefinedViscoplasticFlow.cxx
    PlasticInelasticFlow.cxx
    PorosityNucleationModel.cxx
    PorosityNucleationModelFactory.cxx
    PorosityNucleationModelBase.cxx
    StrainBasedPorosityNucleationModelBase.cxx
    StressBasedPorosityNucleationModelBase.cxx
    ChuNeedleman1980StrainBasedPorosityNucleationModel.cxx
    ChuNeedleman1980StressBasedPorosityNucleationModel.cxx
    PowerLawStrainBasedPorosityNucleationModel.cxx
    PowerLawStressBasedPorosityNucleationModel.cxx
    StressCriterion.cxx
    StressCriterionBase.cxx
    StandardStressCriterionBase.cxx
    StandardPorousStressCriterionBase.cxx
    MisesStressCriterion.cxx
    Hill1948StressCriterion.cxx
    Barlat2004StressCriterion.cxx
    Hosford1972StressCriterion.cxx
    Drucker1949StressCriterion.cxx
    Cazacu2001StressCriterion.cxx
    Cazacu2004IsotropicStressCriterion.cxx
    Cazacu2004OrthotropicStressCriterion.cxx
    MohrCoulombStressCriterion.cxx
    MichelAndSuquet1992HollowSphereStressCriterion.cxx
    GursonTvergaardNeedleman1982StressCriterion.cxx
    RousselierTanguyBesson2002StressCriterion.cxx
    StressCriterionFactory.cxx
    IsotropicHardeningRule.cxx
    LinearIsotropicHardeningRule.cxx
    PowerIsotropicHardeningRule.cxx
    SwiftIsotropicHardeningRule.cxx
    VoceIsotropicHardeningRule.cxx
    DataIsotropicHardeningRule.cxx
    StrainRateSensitiveIsotropicHardeningRule.cxx
    UserDefinedIsotropicHardeningRule.cxx
    IsotropicHardeningRuleFactory.cxx
    StrainRateSensitivityFactor.cxx
    StrainRateSensitivityFactorFactory.cxx
    CowperSymondsStrainRateSensitivityFactor.cxx
    JohnsonCookStrainRateSensitivityFactor.cxx
    KinematicHardeningRule.cxx
    KinematicHardeningRuleBase.cxx
    PragerKinematicHardeningRule.cxx
    Chaboche2012KinematicHardeningRule.cxx
    BurletCailletaudKinematicHardeningRule.cxx
    ArmstrongFrederickKinematicHardeningRule.cxx
    DelobelleRobinetSchafflerKinematicHardeningRule.cxx
    KinematicHardeningRuleFactory.cxx
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
	Castem21Interface.cxx
	CastemSymbolsGenerator.cxx
	CastemMaterialPropertyInterface.cxx
	CastemModelInterface.cxx
	AmitexInterface.cxx
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
    AsterSymbolsGenerator.cxx
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
    AbaqusSymbolsGenerator.cxx
    AbaqusExplicitSymbolsGenerator.cxx
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

if(HAVE_LSDYNA)
  set(TFELMFront_SOURCES
    LSDYNAInterface.cxx
    LSDYNASymbolsGenerator.cxx
    ${TFELMFront_SOURCES})
  set(LSDYNAInterface_SOURCES
    LSDYNAInterfaceExceptions.cxx
    LSDYNAException.cxx
    LSDYNAComputeStiffnessTensor.cxx
    LSDYNAStressFreeExpansionHandler.cxx
    LSDYNAComputeThermalExpansionCoefficientTensor.cxx
    LSDYNAFiniteStrain.cxx)
  mfront_library(LSDYNAInterface ${LSDYNAInterface_SOURCES})
  target_link_libraries(LSDYNAInterface
    MTestFileGenerator
    TFELUtilities
    TFELException)
endif(HAVE_LSDYNA)

if(HAVE_DIANAFEA)
  set(TFELMFront_SOURCES
    DianaFEAInterface.cxx
    DianaFEASymbolsGenerator.cxx
    ${TFELMFront_SOURCES})
  set(DianaFEAInterface_SOURCES
    DianaFEAInterfaceExceptions.cxx
    DianaFEAException.cxx
    DianaFEAOutOfBoundsPolicy.cxx
    DianaFEAComputeStiffnessTensor.cxx
    DianaFEAStressFreeExpansionHandler.cxx
    DianaFEAComputeThermalExpansionCoefficientTensor.cxx)
  mfront_library(DianaFEAInterface ${DianaFEAInterface_SOURCES})
  target_link_libraries(DianaFEAInterface
    MTestFileGenerator
    TFELUtilities
    TFELException)
endif(HAVE_DIANAFEA)

if(HAVE_CALCULIX)
  set(TFELMFront_SOURCES
    CalculiXInterface.cxx
    CalculiXSymbolsGenerator.cxx
    ${TFELMFront_SOURCES})
  set(CalculiXInterface_SOURCES
    CalculiXInterfaceExceptions.cxx
    CalculiXException.cxx
    CalculiXRotationMatrix.cxx
    CalculiXComputeStiffnessTensor.cxx
    CalculiXStressFreeExpansionHandler.cxx
    CalculiXComputeThermalExpansionCoefficientTensor.cxx)
  mfront_library(CalculiXInterface ${CalculiXInterface_SOURCES})
  target_link_libraries(CalculiXInterface
    MTestFileGenerator
    TFELUtilities
    TFELException)
endif(HAVE_CALCULIX)

if(HAVE_COMSOL)
  set(TFELMFront_SOURCES
    ComsolInterface.cxx
#    ComsolSymbolsGenerator.cxx
    ${TFELMFront_SOURCES})
#  set(ComsolInterface_SOURCES
#    ComsolInterfaceExceptions.cxx
#    ComsolException.cxx
#    ComsolRotationMatrix.cxx
#    ComsolComputeStiffnessTensor.cxx
#    ComsolStressFreeExpansionHandler.cxx
#    ComsolComputeThermalExpansionCoefficientTensor.cxx)
#  mfront_library(ComsolInterface ${ComsolInterface_SOURCES})
#  target_link_libraries(ComsolInterface
#    MTestFileGenerator
#    TFELUtilities
#    TFELException)
endif(HAVE_COMSOL)

if(HAVE_ANSYS)
  set(TFELMFront_SOURCES
    AnsysInterface.cxx
    AnsysSymbolsGenerator.cxx
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

if(HAVE_ZMAT)
  set(TFELMFront_SOURCES
    ZMATInterface.cxx
    ${TFELMFront_SOURCES})
endif(HAVE_ZMAT)

if(HAVE_CYRANO)
  set(TFELMFront_SOURCES
    CyranoMaterialPropertyInterface.cxx
    CyranoInterface.cxx
    CyranoSymbolsGenerator.cxx
    ${TFELMFront_SOURCES})
  set(CyranoInterface_SOURCES
    CyranoException.cxx
    CyranoInterfaceExceptions.cxx
    CyranoComputeStiffnessTensor.cxx
    CyranoStressFreeExpansionHandler.cxx
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
if(enable-static)
  target_include_directories(TFELMFront-static
    PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
    $<INSTALL_INTERFACE:include>)
endif(enable-static)
if(HAVE_CASTEM)
  if(CASTEHOME)
    target_include_directories(TFELMFront 
      PRIVATE "${CASTEMHOME}/include"
      PRIVATE "${CASTEMHOME}/include/c")
    if(enable-static)
      target_include_directories(TFELMFront-static 
        PRIVATE "${CASTEMHOME}/include"
        PRIVATE "${CASTEMHOME}/include/c")
    endif(enable-static)
  endif(CASTEHOME)
endif(HAVE_CASTEM)

target_link_libraries(TFELMFront
  PUBLIC
  tfel::MFrontLogStream
  tfel::TFELMaterial
  tfel::TFELMathParser
  tfel::TFELMathCubicSpline
  tfel::TFELMath
  tfel::TFELSystem
  tfel::TFELUtilities
  tfel::TFELGlossary 
  tfel::TFELUnicodeSupport
  tfel::TFELException
  tfel::TFELConfig)
if(enable-static)
  target_link_libraries(TFELMFront-static
    PUBLIC
    tfel::MFrontLogStream-static
    tfel::TFELMaterial-static
    tfel::TFELMathParser-static
    tfel::TFELMathCubicSpline-static
    tfel::TFELMath-static
    tfel::TFELSystem-static
    tfel::TFELUtilities-static
    tfel::TFELUnicodeSupport-static
    tfel::TFELGlossary-static
    tfel::TFELException-static
    tfel::TFELConfig-static)
endif(enable-static)

if(UNIX)
  if(NOT HAIKU)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
      target_link_libraries(TFELMFront
	    PRIVATE ${TFEL_DL_LIBRARY})
    else(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
      target_link_libraries(TFELMFront
      	PRIVATE -pthread PRIVATE ${TFEL_DL_LIBRARY})
    endif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
    if(enable-static)
      if(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
        target_link_libraries(TFELMFront-static
        PRIVATE ${TFEL_DL_LIBRARY})
      else(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
        target_link_libraries(TFELMFront-static
          PRIVATE -pthread PRIVATE ${TFEL_DL_LIBRARY})
      endif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
    endif(enable-static)
  else(NOT HAIKU)
    target_link_libraries(TFELMFront
      PRIVATE ${TFEL_DL_LIBRARY})
    if(enable-static)
      target_link_libraries(TFELMFront-static
        PRIVATE ${TFEL_DL_LIBRARY})
    endif(enable-static)
  endif(NOT HAIKU)
else(UNIX)
endif(UNIX)

if(TFEL_HAVE_MADNEX)
  target_compile_definitions(TFELMFront
    PUBLIC MFRONT_HAVE_MADNEX)
  target_link_libraries(TFELMFront
    PRIVATE madnex::Madnex)
  if(enable-static)
    target_compile_definitions(TFELMFront-static
      PUBLIC MFRONT_HAVE_MADNEX)
    target_link_libraries(TFELMFront-static
      PRIVATE madnex::Madnex)  
  endif(enable-static)
endif(TFEL_HAVE_MADNEX)

# MFront
add_executable(mfront main.cxx)
set_property(TARGET mfront PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mfront
  PUBLIC
  tfel::TFELMFront 
  tfel::MFrontLogStream
  tfel::TFELSystem
  tfel::TFELUtilities)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(mfront
    PROPERTIES OUTPUT_NAME "mfront-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)
install(TARGETS mfront DESTINATION bin
  COMPONENT mfront)

if(enable-cppcheck)
  add_cppcheck(mfront)
endif(enable-cppcheck)

# MFrontDataBase

tfel_library(TFELMFrontDatabase MFrontDatabase.cxx)
target_include_directories(TFELMFrontDatabase
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
   $<INSTALL_INTERFACE:include>
   PRIVATE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(TFELMFrontDatabase 
   PUBLIC tfel::TFELSystem)
if(enable-static)
  target_include_directories(TFELMFrontDatabase-static
    PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
  # type of architecture
  target_link_libraries(TFELMFrontDatabase-static 
    PUBLIC tfel::TFELSystem-static)
endif(enable-static)
