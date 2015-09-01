include_directories("${PROJECT_BINARY_DIR}/src/TFELConfig"
                    "${PROJECT_SOURCE_DIR}/include"
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
                 "${PROJECT_BINARY_DIR}/src/Exception")

configure_file(${PROJECT_SOURCE_DIR}/mfront/src/MFrontHeader.cxx.in
               ${PROJECT_BINARY_DIR}/mfront/src/MFrontHeader.cxx)

set(mfront_SOURCES
    main.cxx
    MFront.cxx
    MFrontDebugMode.cxx
    MFrontPedanticMode.cxx
    MFrontLock.cxx
    MFrontSearchFile.cxx
    MFrontInitInterfaces.cxx
    MFrontInitParsers.cxx
    MFrontInitAnalysers.cxx
    MFrontParserFactory.cxx
    DrivingVariable.cxx
    MaterialPropertyDescription.cxx
    MechanicalBehaviourData.cxx
    MechanicalBehaviourDescription.cxx
    BoundsDescription.cxx
    VariableDescription.cxx
    StaticVariableDescription.cxx
    MFrontBehaviourParserCommon.cxx
    MFrontModelParserCommon.cxx
    MFrontModelParser.cxx
    MFrontBehaviourVirtualParser.cxx
    MFrontBehaviourVirtualInterface.cxx
    MFrontLawVirtualInterface.cxx
    MFrontModelVirtualInterface.cxx
    SupportedTypes.cxx
    ParserBase.cxx
    InterfaceBase.cxx
    ParserUtilities.cxx
    FiniteStrainBehaviourTangentOperatorConversion.cxx
    FiniteStrainBehaviourTangentOperatorConversionPath.cxx
    MFrontBehaviourAnalyser.cxx
    MFrontBehaviourAnalyserFactory.cxx
    MFrontLawInterfaceFactory.cxx
    MFrontBehaviourInterfaceFactory.cxx
    MFrontModelInterfaceFactory.cxx
    MFrontBehaviourAnalyser.cxx
    MFrontBehaviourAnalyserFactory.cxx
    MFrontDefaultParserBase.cxx
    MFrontDefaultParser.cxx
    MFrontDefaultCZMParser.cxx
    MFrontDefaultFiniteStrainParser.cxx
    MFrontMaterialLawParser.cxx
    MFrontIsotropicMisesCreepParser.cxx
    MFrontIsotropicStrainHardeningMisesCreepParser.cxx
    MFrontIsotropicMisesPlasticFlowParser.cxx
    MFrontRungeKuttaParser.cxx
    MFrontImplicitParserBase.cxx
    MFrontImplicitParser.cxx
    MFrontImplicitParserII.cxx
    MFrontImplicitFiniteStrainParser.cxx
    MFrontNonLinearSystemSolver.cxx
    MFrontNonLinearSystemSolverBase.cxx
    MFrontNonLinearSystemSolverFactory.cxx
    MFrontPowellDogLegAlgorithmBase.cxx
    MFrontNewtonRaphsonSolvers.cxx
    MFrontBroydenSolvers.cxx
    MFrontSecondBroydenSolver.cxx
    MFrontLevenbergMarquardtSolvers.cxx
    MFrontPerformanceProfiling.cxx
    MFrontMultipleIsotropicMisesFlowsParser.cxx
    MFrontIsotropicBehaviourParserBase.cxx
    MFrontMFrontLawInterface.cxx
    MFrontCLawInterfaceBase.cxx
    MFrontCLawInterface.cxx
    MFrontCppLawInterface.cxx
    MFrontCppTestLawInterface.cxx
    MFrontExcelLawInterface.cxx
    MFrontExcelLawInternalInterface.cxx
    MFrontLaTeXLawInterface.cxx
    MFrontLaTeXBehaviourAnalyser.cxx
    MFrontMarkdownBehaviourAnalyser.cxx
    MFrontUMATInterfaceBase.cxx
    ${PROJECT_BINARY_DIR}/mfront/src/MFrontHeader.cxx)

if(HAVE_GNUPLOT)
    set(mfront_SOURCES
	MFrontGnuplotLawInterface.cxx
	${mfront_SOURCES})
endif(HAVE_GNUPLOT)

if(HAVE_PYTHON)
    set(mfront_SOURCES
	MFrontPythonLawInterface.cxx
	${mfront_SOURCES})
endif(HAVE_PYTHON)

if(HAVE_FORTRAN)
    set(mfront_SOURCES
	MFrontFortranLawInterface.cxx
	${mfront_SOURCES})
endif(HAVE_FORTRAN)

mfront_library(MFrontLogStream
  MFrontLogStream.cxx)

if(enable-cxx-11)
  mfront_library(MFrontProfiling
    MFrontBehaviourProfiler.cxx)
  if(UNIX AND (NOT APPLE))
    target_link_libraries(MFrontProfiling rt)
  endif(UNIX AND (NOT APPLE))
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
	MFrontUMATInterface.cxx
	MFrontCastemLawInterface.cxx
	${mfront_SOURCES})
    set(UMATInterface_SOURCES
	UMATInterfaceBase.cxx
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
    MFrontAsterInterface.cxx
    ${mfront_SOURCES})
  set(AsterInterface_SOURCES
    AsterInterfaceBase.cxx
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
    MFrontZMATInterface.cxx
    ${mfront_SOURCES})
endif(HAVE_ZMAT)

if(HAVE_CYRANO)
  set(mfront_SOURCES
    MFrontCyranoInterface.cxx
    ${mfront_SOURCES})
  set(CyranoInterface_SOURCES
    CyranoException.cxx
    CyranoInterfaceBase.cxx
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
  if(WIN32 AND enable-static)
    target_link_libraries(mfront
      MFrontLogStream-static
      TFELMaterial-static
      TFELMathParser-static
      TFELSystem-static
      TFELUtilities-static
      TFELGlossary-static
      TFELException-static)
    set_target_properties(mfront PROPERTIES COMPILE_FLAGS "-DTFEL_STATIC_BUILD")
    set_target_properties(mfront PROPERTIES LINK_FLAGS "-static-libgcc -static") 
    set_target_properties(mfront PROPERTIES LINK_SEARCH_END_STATIC ON) 
  else(WIN32 AND enable-static)
    target_link_libraries(mfront
      MFrontLogStream
      TFELMaterial
      TFELMathParser
      TFELSystem
      TFELUtilities
      TFELGlossary
      TFELException)
  endif(WIN32 AND enable-static)
endif(UNIX)

install(TARGETS mfront DESTINATION bin
  COMPONENT mfront)
