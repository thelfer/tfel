## Makefile.am -- Process this file with automake to produce a Makefile.in file.

nobase_include_HEADERS= MFront/MFront.hxx                          \
			MFront/MFrontGenericData.hxx               \
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
			MFront/MFrontLawInterfaceFactory.hxx       \
			MFront/MFrontLawInterfaceProxy.hxx         \
			MFront/MFrontLawInterfaceProxy.ixx         \
			MFront/MFrontBehaviourVirtualInterface.hxx \
			MFront/MFrontBehaviourInterfaceProxy.hxx   \
			MFront/MFrontBehaviourInterfaceProxy.ixx   \
			MFront/MFrontBehaviourInterfaceFactory.hxx \
			MFront/MFrontModelData.hxx                 \
			MFront/MFrontModelVirtualInterface.hxx     \
			MFront/MFrontModelInterfaceProxy.hxx       \
			MFront/MFrontModelInterfaceProxy.ixx       \
			MFront/MFrontModelInterfaceFactory.hxx     \
			MFront/MFrontModelParserCommon.hxx         \
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
			MFront/MFrontMaterialLawParser.hxx         \
			MFront/MFrontModelParser.hxx               \
			MFront/VariableModifier.hxx                \
			MFront/VariableModifier.ixx                \
			MFront/WordAnalyser.hxx                    \
			MFront/WordAnalyser.ixx

nobase_noinst_HEADERS = MFront/MFrontIsotropicBehaviourParserBase.hxx               \
			MFront/MFrontIsotropicMisesCreepParser.hxx                  \
			MFront/MFrontIsotropicStrainHardeningMisesCreepParser.hxx   \
			MFront/MFrontIsotropicMisesPlasticFlowParser.hxx            \
			MFront/MFrontMultipleIsotropicMisesFlowsParser.hxx          \
			MFront/MFrontRungeKuttaParser.hxx                           \
			MFront/MFrontImplicitParserBase.hxx                         \
			MFront/MFrontImplicitParser.hxx                             \
			MFront/MFrontImplicitParserII.hxx

nobase_noinst_HEADERS += MFront/MFrontCLawInterfaceBase.hxx          \
			 MFront/MFrontCLawInterface.hxx              \
			 MFront/MFrontMFrontLawInterface.hxx         \
			 MFront/MFrontFortranLawInterface.hxx        \
			 MFront/MFrontCppLawInterface.hxx            \
			 MFront/MFrontCppTestLawInterface.hxx        \
			 MFront/MFrontGnuplotLawInterface.hxx        \
			 MFront/MFrontExcelLawInterface.hxx          \
			 MFront/MFrontLaTeXLawInterface.hxx          \
			 MFront/MFrontPythonLawInterface.hxx

nobase_noinst_HEADERS += MFront/MTestTypes.hxx	            \
			 MFront/MTest.hxx	            \
			 MFront/MTestBehaviour.hxx          \
			 MFront/MTestUmatBehaviourBase.hxx  \
			 MFront/MTestUmatBehaviour.hxx      \
			 MFront/MTestAsterBehaviour.hxx     \
			 MFront/MTestEvolution.hxx          \
			 MFront/MTestConstraint.hxx         \
			 MFront/MTestImposedStrain.hxx      \
			 MFront/MTestImposedStress.hxx

if HAVE_CASTEM
nobase_include_HEADERS += MFront/MFrontCastemLawInterface.hxx   \
	 	          MFront/MFrontUMATInterface.hxx
if LOCAL_CASTEM_HEADER
nobase_include_HEADERS += castem.h
endif 

nobase_include_HEADERS += MFront/UMAT/UMATException.hxx                     \
			  MFront/UMAT/UMATGetModellingHypothesis.hxx        \
			  MFront/UMAT/UMATOutOfBoundsPolicy.hxx             \
			  MFront/UMAT/UMATInterface.hxx                     \
			  MFront/UMAT/UMATInterfaceBase.hxx                 \
			  MFront/UMAT/UMATFiniteStrain.hxx                  \
			  MFront/UMAT/UMATBehaviourHandler.hxx              \
			  MFront/UMAT/UMATIsotropicBehaviourHandler.hxx     \
			  MFront/UMAT/UMATOrthotropicBehaviourHandler.hxx   \
			  MFront/UMAT/UMATGenericPlaneStressHandler.hxx     \
			  MFront/UMAT/UMATRotationMatrix.hxx                \
			  MFront/UMAT/UMATOrthotropicBehaviour.hxx          \
			  MFront/UMAT/UMATComputeStiffnessTensor.hxx        \
			  MFront/UMAT/UMATComputeThermalExpansionTensor.hxx \
			  MFront/UMAT/UMATTraits.hxx                        \
			  MFront/UMAT/UMAT.hxx                              \
			  MFront/UMAT/UMATConfig.hxx
endif

if HAVE_ASTER
nobase_include_HEADERS += MFront/MFrontAsterInterface.hxx
nobase_include_HEADERS += MFront/Aster/Aster.hxx                              \
			  MFront/Aster/AsterConfig.hxx                        \
			  MFront/Aster/AsterTraits.hxx                        \
			  MFront/Aster/AsterException.hxx 		      \
			  MFront/Aster/AsterInterface.hxx	              \
			  MFront/Aster/AsterInterfaceBase.hxx                 \
			  MFront/Aster/AsterBehaviourHandler.hxx              \
			  MFront/Aster/AsterIsotropicBehaviourHandler.hxx     \
			  MFront/Aster/AsterOrthotropicBehaviourHandler.hxx   \
			  MFront/Aster/AsterRotationMatrix.hxx                \
			  MFront/Aster/AsterOrthotropicBehaviour.hxx          \
			  MFront/Aster/AsterComputeStiffnessTensor.hxx        \
			  MFront/Aster/AsterComputeThermalExpansionTensor.hxx \
			  MFront/Aster/AsterTangentOperator.hxx
# MFront/Aster/AsterGetModellingHypothesis.hxx        \
# MFront/Aster/AsterOutOfBoundsPolicy.hxx

endif


EXTRA_DIST = CMakeLists.txt