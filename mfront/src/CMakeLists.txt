include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/mfront/include")
if(HAVE_CASTEM)
include_directories("${CASTEMHOME}/include")
endif(HAVE_CASTEM)

link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

configure_file(${PROJECT_SOURCE_DIR}/mfront/src/MFrontHeader.cxx.in
               ${PROJECT_BINARY_DIR}/mfront/src/MFrontHeader.cxx)

set(mfront_SOURCES
    main.cxx
    MFront.cxx
    MFrontLock.cxx
    MFrontInitInterfaces.cxx
    MFrontInitParsers.cxx
    MFrontParserFactory.cxx
    BehaviourCharacteristic.cxx
    BoundsDescription.cxx
    VarHandler.cxx
    StaticVarHandler.cxx
    ComputedVarHandler.cxx
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
    UnaryLoadingDefinition.cxx
    MFrontLawInterfaceFactory.cxx
    MFrontBehaviourInterfaceFactory.cxx
    MFrontModelInterfaceFactory.cxx
    MFrontDefaultParser.cxx
    MFrontMaterialLawParser.cxx
    MFrontIsotropicMisesCreepParser.cxx
    MFrontIsotropicStrainHardeningMisesCreepParser.cxx
    MFrontIsotropicMisesPlasticFlowParser.cxx
    MFrontRungeKuttaParser.cxx
    MFrontImplicitParser.cxx
    MFrontMultipleIsotropicMisesFlowsParser.cxx
    MFrontIsotropicBehaviourParserBase.cxx
    MFrontMFrontLawInterface.cxx
    MFrontCLawInterfaceBase.cxx
    MFrontCLawInterface.cxx
    MFrontCppLawInterface.cxx
    MFrontCppTestLawInterface.cxx
    MFrontExcelLawInterface.cxx
    MFrontLaTeXLawInterface.cxx
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

if(HAVE_CASTEM)
    set(mfront_SOURCES
	MFrontUMATInterface.cxx
	MFrontCastemLawInterface.cxx
	${mfront_SOURCES})
    set(UMATInterface_SOURCES
	UMATInterfaceBase.cxx
	UMATComputeStiffnessTensor.cxx
	UMATComputeThermalExpansionTensor.cxx
	UMATRotationMatrix.cxx
	UMATOutOfBoundsPolicy.cxx
	UMATGetModellingHypothesis.cxx
	UMATException.cxx)
    tfel_library(UMATInterface ${UMATInterface_SOURCES})
    target_link_libraries(UMATInterface
	                  TFELUtilities
			  TFELException)
endif(HAVE_CASTEM)

add_executable(mfront ${mfront_SOURCES})

if(UNIX)
target_link_libraries(mfront
                      TFELSystem
		      TFELUtilities
		      TFELException
		      -ldl -pthread)
else(UNIX)
target_link_libraries(mfront
                      TFELSystem
		      TFELUtilities
		      TFELException)
endif(UNIX)

install(TARGETS mfront DESTINATION bin)
