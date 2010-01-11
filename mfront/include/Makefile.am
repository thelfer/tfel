## Makefile.am -- Process this file with automake to produce a Makefile.in file.

include_HEADERS= MFront.hxx                          \
		 MFrontLock.hxx                      \
		 MFrontInitParsers.hxx               \
		 MFrontInitInterfaces.hxx            \
		 MFrontHeader.hxx                    \
		 MFrontVirtualParser.hxx             \
		 MFrontBehaviourParserCommon.hxx     \
		 MFrontBehaviourParserBase.hxx       \
		 MFrontBehaviourParserBase.ixx       \
		 MFrontParserFactory.hxx             \
		 MFrontParserProxy.hxx               \
		 MFrontParserProxy.ixx               \
		 MFrontLawVirtualInterface.hxx       \
		 MFrontBehaviourVirtualInterface.hxx \
		 MFrontBehaviourInterfaceProxy.hxx   \
		 MFrontBehaviourInterfaceProxy.ixx   \
		 MFrontBehaviourInterfaceFactory.hxx \
		 MFrontLawInterfaceFactory.hxx       \
		 MFrontLawInterfaceProxy.hxx         \
		 MFrontLawInterfaceProxy.ixx         \
		 MFrontModelParserBase.hxx           \
		 MFrontModelParserBase.ixx           \
		 BehaviourCharacteristic.hxx         \
		 UnaryLoadingDefinition.hxx          \
		 ParserBase.hxx                      \
		 InterfaceBase.hxx                   \
		 ParserUtilities.hxx                 \
		 StaticVarHandler.hxx                \
		 ComputedVarHandler.hxx              \
		 SupportedTypes.hxx                  \
		 BoundsDescription.hxx               \
		 VariableBoundsDescription.hxx       \
		 VarHandler.hxx                      \
		 LawFunction.hxx                     \
		 MFrontDefaultParser.hxx             \
		 MFrontMaterialLawParser.hxx

noinst_HEADERS = MFrontIsotropicBehaviourParserBase.hxx               \
		 MFrontIsotropicMisesCreepParser.hxx                  \
		 MFrontIsotropicStrainHardeningMisesCreepParser.hxx   \
		 MFrontIsotropicMisesPlasticFlowParser.hxx            \
		 MFrontMultipleIsotropicMisesFlowsParser.hxx          \
		 MFrontPleiadesModelParser.hxx                        \
		 MFrontPleiadesModelParserArchitecture1.4.hxx         \
		 MFrontRungeKuttaParser.hxx                           \
		 MFrontImplicitParser.hxx

noinst_HEADERS += MFrontCLawInterfaceBase.hxx    \
		  MFrontCLawInterface.hxx        \
		  MFrontMFrontLawInterface.hxx   \
		  MFrontFortranLawInterface.hxx  \
		  MFrontCppLawInterface.hxx      \
		  MFrontCppTestLawInterface.hxx  \
		  MFrontGnuplotLawInterface.hxx  \
		  MFrontExcelLawInterface.hxx    \
		  MFrontOctaveLawInterface.hxx   \
		  MFrontPleiadesLawInterface.hxx \
		  MFrontPythonLawInterface.hxx

if HAVE_CASTEM

noinst_HEADERS += MFrontCastemLawInterface.hxx   \
		  MFrontUMATInterface.hxx

nobase_include_HEADERS = UMAT/UMATException.hxx                     \
			 UMAT/UMATOutOfBoundsPolicy.hxx             \
			 UMAT/UMATInterface.hxx                     \
			 UMAT/UMATRotationMatrix.hxx                \
			 UMAT/UMATOrthotropicBehaviour.hxx          \
			 UMAT/UMATComputeStiffnessTensor.hxx        \
			 UMAT/UMATComputeThermalExpansionTensor.hxx \
			 UMAT/UMATTraits.hxx                        \
			 UMAT/UMAT.hxx
endif

