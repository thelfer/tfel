include_directories("${PROJECT_BINARY_DIR}/src/TFELConfig"
                    "${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/mfront/include")
if(HAVE_CASTEM)
include_directories("${CASTEMHOME}/include")
endif(HAVE_CASTEM)

link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Glossary"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

configure_file(${PROJECT_SOURCE_DIR}/mfront/src/MFrontHeader.cxx.in
               ${PROJECT_BINARY_DIR}/mfront/src/MFrontHeader.cxx)

set(mfront_SOURCES
    main.cxx
    MFront.cxx
    MFrontDebugMode.cxx
    PedanticMode.cxx
    MFrontLock.cxx
    SearchFile.cxx
    InitInterfaces.cxx
    InitDSLs.cxx
    InitAnalysers.cxx
    DSLFactory.cxx
    DrivingVariable.cxx
    MaterialPropertyDescription.cxx
    BehaviourData.cxx
    BehaviourDescription.cxx
    BoundsDescription.cxx
    VariableDescription.cxx
    StaticVariableDescription.cxx
    BehaviourDSLCommon.cxx
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
    BehaviourAnalyser.cxx
    BehaviourAnalyserFactory.cxx
    MaterialPropertyInterfaceFactory.cxx
    BehaviourInterfaceFactory.cxx
    ModelInterfaceFactory.cxx
    BehaviourAnalyser.cxx
    BehaviourAnalyserFactory.cxx
    DefaultDSLBase.cxx
    DefaultDSL.cxx
    DefaultCZMDSL.cxx
    DefaultFiniteStrainDSL.cxx
    MaterialPropertyDSL.cxx
    IsotropicMisesCreepDSL.cxx
    IsotropicStrainHardeningMisesCreepDSL.cxx
    IsotropicMisesPlasticFlowDSL.cxx
    RungeKuttaDSL.cxx
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
    BehaviourBrick.cxx
    BehaviourBrickFactory.cxx
    BehaviourBrickBase.cxx
    ElasticityBehaviourBrick.cxx
    PerformanceProfiling.cxx
    MultipleIsotropicMisesFlowsDSL.cxx
    IsotropicBehaviourDSLBase.cxx
    MFrontMaterialPropertyInterface.cxx
    CMaterialPropertyInterfaceBase.cxx
    CMaterialPropertyInterface.cxx
    CppMaterialPropertyInterface.cxx
    CppTestMaterialPropertyInterface.cxx
    ExcelMaterialPropertyInterface.cxx
    ExcelMaterialPropertyInternalInterface.cxx
    LaTeXMaterialPropertyInterface.cxx
    LaTeXBehaviourAnalyser.cxx
    MarkdownBehaviourAnalyser.cxx
    UMATInterfaceBase.cxx
    ${PROJECT_BINARY_DIR}/mfront/src/MFrontHeader.cxx)

if(HAVE_GNUPLOT)
    set(mfront_SOURCES
	GnuplotMaterialPropertyInterface.cxx
	${mfront_SOURCES})
endif(HAVE_GNUPLOT)

if(HAVE_PYTHON)
    set(mfront_SOURCES
	PythonMaterialPropertyInterface.cxx
	${mfront_SOURCES})
endif(HAVE_PYTHON)

if(HAVE_FORTRAN)
    set(mfront_SOURCES
	FortranMaterialPropertyInterface.cxx
	${mfront_SOURCES})
endif(HAVE_FORTRAN)

mfront_library(MFrontLogStream
  MFrontLogStream.cxx)

if(enable-cxx-11)
  mfront_library(MFrontProfiling
    BehaviourProfiler.cxx)
  if(UNIX)
    target_link_libraries(MFrontProfiling rt)
  endif(UNIX)
endif(enable-cxx-11)

mfront_library(MTestFileGenerator
  MTestFileGeneratorBase.cxx
  UMATSmallStrainMTestFileGenerator.cxx
  UMATFiniteStrainMTestFileGenerator.cxx)
target_link_libraries(MTestFileGenerator
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

if(HAVE_CASTEM)
    set(mfront_SOURCES
	UMATInterface.cxx
	CastemMaterialPropertyInterface.cxx
	${mfront_SOURCES})
    set(UMATInterface_SOURCES
	UMATInterfaceExceptions.cxx
	UMATFiniteStrain.cxx
	UMATComputeStiffnessTensor.cxx
	UMATComputeThermalExpansionCoefficientTensor.cxx
	UMATRotationMatrix.cxx
	UMATFiniteStrainRotationMatrix.cxx
	UMATTangentOperator.cxx
	UMATOutOfBoundsPolicy.cxx
	UMATGetModellingHypothesis.cxx
	UMATStressFreeExpansionHandler.cxx
	UMATException.cxx)
    mfront_library(UMATInterface ${UMATInterface_SOURCES})
    target_link_libraries(UMATInterface
      MTestFileGenerator
      TFELUtilities
      TFELException)
endif(HAVE_CASTEM)

if(HAVE_ASTER)
  set(mfront_SOURCES
    AsterInterface.cxx
    ${mfront_SOURCES})
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

if(HAVE_ZMAT)
  set(mfront_SOURCES
    ZMATInterface.cxx
    ${mfront_SOURCES})
endif(HAVE_ZMAT)

if(HAVE_CYRANO)
  set(mfront_SOURCES
    CyranoInterface.cxx
    ${mfront_SOURCES})
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

add_executable(mfront ${mfront_SOURCES})

if(UNIX)
  target_link_libraries(mfront
    MFrontLogStream
    TFELMaterial TFELMathParser
    TFELSystem   TFELUtilities
    TFELGlossary TFELException
    -pthread @TFEL_DL_LIBRARY@)
else(UNIX)
target_link_libraries(mfront
                      MFrontLogStream
                      TFELMaterial
                      TFELMathParser
                      TFELSystem
		      TFELUtilities
                      TFELGlossary
		      TFELException)
endif(UNIX)

install(TARGETS mfront DESTINATION bin
  COMPONENT mfront)
