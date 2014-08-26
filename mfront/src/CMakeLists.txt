include_directories("${PROJECT_BINARY_DIR}/src/TFELConfig"
                    "${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/mfront/include")
if(HAVE_CASTEM)
include_directories("${CASTEMHOME}/include")
endif(HAVE_CASTEM)

link_directories("${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

configure_file(${PROJECT_SOURCE_DIR}/mfront/src/MFrontHeader.cxx.in
               ${PROJECT_BINARY_DIR}/mfront/src/MFrontHeader.cxx)

set(mfront_SOURCES
    main.cxx
    MFront.cxx
    MFrontLock.cxx
    MFrontSearchFile.cxx
    MFrontInitInterfaces.cxx
    MFrontInitParsers.cxx
    MFrontParserFactory.cxx
    DrivingVariable.cxx
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
    MFrontLawInterfaceFactory.cxx
    MFrontBehaviourInterfaceFactory.cxx
    MFrontModelInterfaceFactory.cxx
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
    MFrontMultipleIsotropicMisesFlowsParser.cxx
    MFrontIsotropicBehaviourParserBase.cxx
    MFrontMFrontLawInterface.cxx
    MFrontCLawInterfaceBase.cxx
    MFrontCLawInterface.cxx
    MFrontCppLawInterface.cxx
    MFrontCppTestLawInterface.cxx
    MFrontExcelLawInterface.cxx
    MFrontLaTeXLawInterface.cxx
    MFrontUMATInterfaceBase.cxx
    VariableModifier.cxx
    WordAnalyser.cxx
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

tfel_library(MTestFileGenerator
  MTestFileGeneratorBase.cxx
  UMATSmallStrainMTestFileGenerator.cxx
  UMATFiniteStrainMTestFileGenerator.cxx)
target_link_libraries(MTestFileGenerator)

if(HAVE_CASTEM)
    set(mfront_SOURCES
	MFrontUMATInterface.cxx
	MFrontCastemLawInterface.cxx
	${mfront_SOURCES})
    set(UMATInterface_SOURCES
	UMATInterfaceBase.cxx
	UMATFiniteStrain.cxx
	UMATComputeStiffnessOperator.cxx
	UMATComputeThermalExpansionTensor.cxx
	UMATRotationMatrix.cxx
	UMATTangentOperator.cxx
	UMATOutOfBoundsPolicy.cxx
	UMATGetModellingHypothesis.cxx
	UMATException.cxx)
    tfel_library(UMATInterface ${UMATInterface_SOURCES})
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
    AsterComputeStiffnessOperator.cxx
    AsterComputeThermalExpansionTensor.cxx
    AsterTangentOperator.cxx)
  tfel_library(AsterInterface ${AsterInterface_SOURCES})
  target_link_libraries(AsterInterface
    MTestFileGenerator
    TFELUtilities
    TFELException)
endif(HAVE_ASTER)

if(HAVE_CYRANO)
    set(mfront_SOURCES
    	MFrontCyranoInterface.cxx
    	${mfront_SOURCES})
    set(CyranoInterface_SOURCES
        CyranoException.cxx
        CyranoInterfaceBase.cxx
	CyranoComputeStiffnessOperator.cxx
	CyranoComputeThermalExpansionTensor.cxx
	CyranoOutOfBoundsPolicy.cxx
	CyranoGetModellingHypothesis.cxx)
    tfel_library(CyranoInterface ${CyranoInterface_SOURCES})
    target_link_libraries(CyranoInterface
      MTestFileGenerator
      TFELUtilities
      TFELException)
endif(HAVE_CYRANO)

add_executable(mfront ${mfront_SOURCES})

if(UNIX)
  target_link_libraries(mfront
    TFELMaterial  TFELMathParser
    TFELSystem    TFELUtilities
    TFELException -pthread @TFEL_DL_LIBRARY@)
else(UNIX)
target_link_libraries(mfront
                      TFELMaterial
                      TFELMathParser
                      TFELSystem
		      TFELUtilities
		      TFELException)
endif(UNIX)

install(TARGETS mfront DESTINATION bin)
