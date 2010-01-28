## Makefile.am -- Process this file with automake to produce a Makefile.in file.

include_HEADERS= MFront/MFront.hxx                          \
		 MFront/MFrontLock.hxx                      \
		 MFront/MFrontInitParsers.hxx               \
		 MFront/MFrontInitInterfaces.hxx            \
		 MFront/MFrontHeader.hxx                    \
		 MFront/MFrontVirtualParser.hxx             \
		 MFront/MFrontBehaviourParserCommon.hxx     \
		 MFront/MFrontBehaviourParserBase.hxx       \
		 MFront/MFrontBehaviourParserBase.ixx       \
		 MFront/MFrontParserFactory.hxx             \
		 MFront/MFrontParserProxy.hxx               \
		 MFront/MFrontParserProxy.ixx               \
		 MFront/MFrontLawVirtualInterface.hxx       \
		 MFront/MFrontBehaviourVirtualInterface.hxx \
		 MFront/MFrontBehaviourInterfaceProxy.hxx   \
		 MFront/MFrontBehaviourInterfaceProxy.ixx   \
		 MFront/MFrontBehaviourInterfaceFactory.hxx \
		 MFront/MFrontLawInterfaceFactory.hxx       \
		 MFront/MFrontLawInterfaceProxy.hxx         \
		 MFront/MFrontLawInterfaceProxy.ixx         \
		 MFront/MFrontModelParserBase.hxx           \
		 MFront/MFrontModelParserBase.ixx           \
		 MFront/BehaviourCharacteristic.hxx         \
		 MFront/UnaryLoadingDefinition.hxx          \
		 MFront/ParserBase.hxx                      \
		 MFront/InterfaceBase.hxx                   \
		 MFront/ParserUtilities.hxx                 \
		 MFront/StaticVarHandler.hxx                \
		 MFront/ComputedVarHandler.hxx              \
		 MFront/SupportedTypes.hxx                  \
		 MFront/BoundsDescription.hxx               \
		 MFront/VariableBoundsDescription.hxx       \
		 MFront/VarHandler.hxx                      \
		 MFront/LawFunction.hxx                     \
		 MFront/MFrontDefaultParser.hxx             \
		 MFront/MFrontMaterialLawParser.hxx

noinst_HEADERS = MFront/MFrontIsotropicBehaviourParserBase.hxx               \
		 MFront/MFrontIsotropicMisesCreepParser.hxx                  \
		 MFront/MFrontIsotropicStrainHardeningMisesCreepParser.hxx   \
		 MFront/MFrontIsotropicMisesPlasticFlowParser.hxx            \
		 MFront/MFrontMultipleIsotropicMisesFlowsParser.hxx          \
		 MFront/MFrontPleiadesModelParser.hxx                        \
		 MFront/MFrontPleiadesModelParserArchitecture1.4.hxx         \
		 MFront/MFrontRungeKuttaParser.hxx                           \
		 MFront/MFrontImplicitParser.hxx

noinst_HEADERS += MFront/MFrontCLawInterfaceBase.hxx    \
		  MFront/MFrontCLawInterface.hxx        \
		  MFront/MFrontMFrontLawInterface.hxx   \
		  MFront/MFrontFortranLawInterface.hxx  \
		  MFront/MFrontCppLawInterface.hxx      \
		  MFront/MFrontCppTestLawInterface.hxx  \
		  MFront/MFrontGnuplotLawInterface.hxx  \
		  MFront/MFrontExcelLawInterface.hxx    \
		  MFront/MFrontOctaveLawInterface.hxx   \
		  MFront/MFrontPleiadesLawInterface.hxx \
		  MFront/MFrontPythonLawInterface.hxx

if HAVE_CASTEM

noinst_HEADERS += MFront/MFrontCastemLawInterface.hxx   \
		  MFront/MFrontUMATInterface.hxx

nobase_include_HEADERS = MFront/UMAT/UMATException.hxx                     \
			 MFront/UMAT/UMATOutOfBoundsPolicy.hxx             \
			 MFront/UMAT/UMATInterface.hxx                     \
			 MFront/UMAT/UMATRotationMatrix.hxx                \
			 MFront/UMAT/UMATOrthotropicBehaviour.hxx          \
			 MFront/UMAT/UMATComputeStiffnessTensor.hxx        \
			 MFront/UMAT/UMATComputeThermalExpansionTensor.hxx \
			 MFront/UMAT/UMATTraits.hxx                        \
			 MFront/UMAT/UMAT.hxx
endif

