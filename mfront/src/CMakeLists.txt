include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/mfront/include")
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
    MFrontBehaviourVirtualParser.cxx
    MFrontBehaviourVirtualInterface.cxx
    MFrontLawVirtualInterface.cxx
    SupportedTypes.cxx
    ParserBase.cxx
    InterfaceBase.cxx
    ParserUtilities.cxx
    UnaryLoadingDefinition.cxx
    MFrontBehaviourInterfaceFactory.cxx
    MFrontLawInterfaceFactory.cxx
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
    MFrontOctaveLawInterface.cxx
    ${PROJECT_BINARY_DIR}/mfront/src/MFrontHeader.cxx)

add_executable(mfront ${mfront_SOURCES})
target_link_libraries(mfront
                      TFELSystem
		      TFELUtilities
		      TFELException
		      -ldl -pthread)
install(TARGET mfront DESTINATION bin)