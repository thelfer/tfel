## Makefile.am -- Process this file with automake to produce a Makefile.in file.

nobase_include_HEADERS= MFront/MFrontConfig.hxx                       \
			MFront/MFront.hxx                             \
			MFront/MFrontGenericData.hxx                  \
			MFront/MFrontLock.hxx                         \
			MFront/MFrontInitParsers.hxx                  \
			MFront/MFrontInitInterfaces.hxx               \
			MFront/MFrontHeader.hxx                       \
			MFront/MFrontVirtualParser.hxx                \
			MFront/MFrontBehaviourParserCommon.hxx        \
			MFront/MFrontBehaviourParserBase.hxx          \
			MFront/MFrontBehaviourParserBase.ixx          \
			MFront/MFrontParserFactory.hxx                \
			MFront/MFrontParserProxy.hxx                  \
			MFront/MFrontParserProxy.ixx                  \
			MFront/MFrontLawVirtualInterface.hxx          \
			MFront/MFrontLawInterfaceFactory.hxx          \
			MFront/MFrontLawInterfaceProxy.hxx            \
			MFront/MFrontLawInterfaceProxy.ixx            \
			MFront/MFrontBehaviourVirtualInterface.hxx    \
			MFront/MFrontBehaviourInterfaceProxy.hxx      \
			MFront/MFrontBehaviourInterfaceProxy.ixx      \
			MFront/MFrontBehaviourInterfaceFactory.hxx    \
			MFront/MFrontModelData.hxx                    \
			MFront/MFrontModelVirtualInterface.hxx        \
			MFront/MFrontModelInterfaceProxy.hxx          \
			MFront/MFrontModelInterfaceProxy.ixx          \
			MFront/MFrontModelInterfaceFactory.hxx        \
			MFront/MFrontModelParserCommon.hxx            \
			MFront/MFrontModelParserBase.hxx              \
			MFront/MFrontModelParserBase.ixx              \
			MFront/MaterialPropertyDescription.hxx        \
			MFront/MechanicalBehaviourDescription.hxx     \
			MFront/ParserBase.hxx                         \
			MFront/InterfaceBase.hxx                      \
			MFront/ParserUtilities.hxx                    \
			MFront/SupportedTypes.hxx                     \
			MFront/MFrontSearchFile.hxx                   \
			MFront/DrivingVariable.hxx                    \
			MFront/ThermodynamicForce.hxx                 \
			MFront/BoundsDescription.hxx                  \
			MFront/VariableBoundsDescription.hxx          \
			MFront/VarHandler.hxx                         \
			MFront/StaticVarHandler.hxx                   \
			MFront/VariableDescription.hxx                \
			MFront/MechanicalVariableDescription.hxx      \
			MFront/StaticVariableDescription.hxx          \
			MFront/LawFunction.hxx                        \
			MFront/MFrontDefaultParserBase.hxx            \
			MFront/MFrontDefaultParser.hxx                \
			MFront/MFrontDefaultCZMParser.hxx             \
			MFront/MFrontDefaultFiniteStrainParser.hxx    \
			MFront/MFrontMaterialLawParser.hxx            \
			MFront/MFrontModelParser.hxx                  \
			MFront/VariableModifier.hxx                   \
			MFront/VariableModifier.ixx                   \
			MFront/WordAnalyser.hxx                       \
			MFront/WordAnalyser.ixx                       \
			MFront/MFrontUMATInterfaceBase.hxx            \
			MFront/MTestFileGeneratorBase.hxx             \
			MFront/UMATSmallStrainMTestFileGenerator.hxx  \
			MFront/UMATFiniteStrainMTestFileGenerator.hxx

nobase_noinst_HEADERS = MFront/MFrontIsotropicBehaviourParserBase.hxx               \
			MFront/MFrontIsotropicMisesCreepParser.hxx                  \
			MFront/MFrontIsotropicStrainHardeningMisesCreepParser.hxx   \
			MFront/MFrontIsotropicMisesPlasticFlowParser.hxx            \
			MFront/MFrontMultipleIsotropicMisesFlowsParser.hxx          \
			MFront/MFrontRungeKuttaParser.hxx                           \
			MFront/MFrontImplicitParserBase.hxx                         \
			MFront/MFrontImplicitParser.hxx                             \
			MFront/MFrontImplicitParserII.hxx                           \
			MFront/MFrontImplicitFiniteStrainParser.hxx

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

nobase_include_HEADERS += MFront/MTestTypes.hxx	                         \
			  MFront/MTestLogStream.hxx	                 \
			  MFront/MTest.hxx	                         \
			  MFront/MTestParser.hxx	                 \
			  MFront/MTestConfig.hxx	                 \
			  MFront/MTestAnalyticalTest.hxx	         \
			  MFront/MTestReferenceFileComparisonTest.hxx    \
			  MFront/MTestBehaviour.hxx                      \
			  MFront/MTestUmatBehaviourBase.hxx              \
			  MFront/MTestUmatStandardBehaviour.hxx          \
			  MFront/MTestUmatSmallStrainBehaviour.hxx       \
			  MFront/MTestUmatFiniteStrainBehaviour.hxx      \
			  MFront/MTestUmatCohesiveZoneModelBehaviour.hxx \
			  MFront/MTestUmatNormaliseTangentOperator.hxx   \
			  MFront/MTestAsterSmallStrainBehaviour.hxx      \
			  MFront/MTestCyranoBehaviour.hxx                \
			  MFront/MTestEvolution.hxx                      \
			  MFront/MTestCastemEvolution.hxx                \
			  MFront/MTestFunctionEvolution.hxx              \
			  MFront/MTestConstraint.hxx                     \
			  MFront/MTestImposedDrivingVariable.hxx         \
			  MFront/MTestImposedThermodynamicForce.hxx

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
			  MFront/UMAT/UMATInterfaceDispatch.hxx             \
			  MFront/UMAT/UMATInterfaceBase.hxx                 \
			  MFront/UMAT/UMATFiniteStrain.hxx                  \
			  MFront/UMAT/UMATBehaviourHandler.hxx              \
			  MFront/UMAT/UMATIsotropicBehaviourHandler.hxx     \
			  MFront/UMAT/UMATOrthotropicBehaviourHandler.hxx   \
			  MFront/UMAT/UMATGenericPlaneStressHandler.hxx     \
			  MFront/UMAT/UMATRotationMatrix.hxx                \
			  MFront/UMAT/UMATOrthotropicBehaviour.hxx          \
			  MFront/UMAT/UMATComputeStiffnessOperator.hxx      \
			  MFront/UMAT/UMATComputeThermalExpansionTensor.hxx \
			  MFront/UMAT/UMATTraits.hxx                        \
			  MFront/UMAT/UMATTangentOperator.hxx               \
			  MFront/UMAT/UMAT.hxx                              \
			  MFront/UMAT/UMATConfig.hxx
endif

if HAVE_CYRANO

nobase_include_HEADERS += MFront/Cyrano/Cyrano.hxx 				\
			  MFront/Cyrano/CyranoTraits.hxx 			\
			  MFront/Cyrano/CyranoConfig.hxx 			\
			  MFront/Cyrano/CyranoException.hxx 			\
			  MFront/Cyrano/CyranoInterface.hxx 			\
			  MFront/Cyrano/CyranoInterfaceDispatch.hxx 		\
			  MFront/Cyrano/CyranoInterfaceBase.hxx 		\
			  MFront/Cyrano/CyranoBehaviourHandler.hxx 		\
			  MFront/Cyrano/CyranoIsotropicBehaviourHandler.hxx 	\
			  MFront/Cyrano/CyranoOrthotropicBehaviourHandler.hxx 	\
			  MFront/Cyrano/CyranoGenericPlaneStressHandler.hxx 	\
			  MFront/Cyrano/CyranoOutOfBoundsPolicy.hxx 		\
			  MFront/Cyrano/CyranoOrthotropicBehaviour.hxx 		\
			  MFront/Cyrano/CyranoComputeStiffnessOperator.hxx 	\
			  MFront/Cyrano/CyranoGetModellingHypothesis.hxx 	\
			  MFront/Cyrano/CyranoComputeThermalExpansionTensor.hxx \
			  MFront/MFrontCyranoInterface.hxx

endif

if HAVE_ASTER
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
			  MFront/Aster/AsterComputeStiffnessOperator.hxx      \
			  MFront/Aster/AsterComputeThermalExpansionTensor.hxx \
			  MFront/Aster/AsterTangentOperator.hxx               \
			  MFront/MFrontAsterInterface.hxx
endif

EXTRA_DIST = CMakeLists.txt