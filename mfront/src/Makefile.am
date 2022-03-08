# Makefile.am -- Process this file with automake to produce a Makefile.in file.

AM_CPPFLAGS = -I$(top_srcdir)/include          \
	      -I$(top_srcdir)/mfront/include

if HAVE_CASTEM
if !LOCAL_CASTEM_HEADER
AM_CPPFLAGS += -I$(CASTEM_ROOT)/include
AM_CPPFLAGS += -I$(CASTEM_ROOT)/include/c
endif
AM_CPPFLAGS += $(CASTEM_CPPFLAGS)
endif

bin_PROGRAMS = mfront
lib_LTLIBRARIES = libMFrontLogStream.la libTFELMFront.la libMTestFileGenerator.la

libMFrontLogStream_la_SOURCES  = MFrontLogStream.cxx
libMFrontLogStream_la_CPPFLAGS = $(AM_CPPFLAGS) -DMFrontLogStream_EXPORTS
if TFEL_WIN
libMFrontLogStream_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN

libTFELMFront_la_SOURCES = MFront.cxx                                     \
			   MFrontBase.cxx                                             \
			   PathSpecifier.cxx                                          \
               GlobalDomainSpecificLanguageOptionsManager.cxx              \
               MaterialKnowledgeDescription.cxx                           \
			   MFrontUtilities.cxx                                        \
			   GeneratorOptions.cxx                                       \
			   CMakeGenerator.cxx                                         \
			   MakefileGenerator.cxx                                      \
		           FileDescription.cxx                                    \
		           TargetsDescription.cxx                                 \
		           OverridableImplementation.cxx                          \
		           CompiledTargetDescriptionBase.cxx                      \
		           LibraryDescription.cxx                                 \
		           ExecutableDescription.cxx                              \
		           SpecificTargetDescription.cxx                          \
		           CodeBlock.cxx                                          \
		           FiniteStrainBehaviourTangentOperatorConversion.cxx     \
		           FiniteStrainBehaviourTangentOperatorConversionPath.cxx \
		           MFrontDebugMode.cxx                                    \
		           PedanticMode.cxx                                       \
		           MFrontLock.cxx                                         \
		           InitInterfaces.cxx                                     \
		           InitDSLs.cxx                                           \
		           DSLFactory.cxx                                         \
		           Gradient.cxx                                           \
		           ThermodynamicForce.cxx                                 \
		           LawFunction.cxx                                        \
		           MaterialPropertyDescription.cxx                        \
			   MaterialPropertyParametersHandler.cxx                  \
		           BehaviourData.cxx                                      \
		           BehaviourDescription.cxx                               \
			   VariableDescriptionBase.cxx                            \
			   VariableDescription.cxx                                \
		           StaticVariableDescription.cxx                          \
		           VariableBoundsDescription.cxx                          \
		           BehaviourDSLCommon.cxx                                 \
		           ModelDescription.cxx                                   \
		           ModelDSLCommon.cxx                                     \
                   BehaviourDSLDescription.cxx                            \
		           AbstractDSL.cxx                                        \
		           AbstractBehaviourDSL.cxx                               \
		           AbstractBehaviourInterface.cxx                         \
		           AbstractMaterialPropertyInterface.cxx                  \
		           AbstractModelInterface.cxx                             \
		           SupportedTypes.cxx                                     \
		           MFrontHeader.cxx                                       \
		           DSLBase.cxx                                            \
		           InterfaceBase.cxx                                      \
		           DSLUtilities.cxx                                       \
		           InstallPath.cxx                                        \
		           SearchPathsHandler.cxx                                 \
		           MaterialPropertyInterfaceFactory.cxx                   \
		           BehaviourInterfaceFactory.cxx                          \
		           ModelInterfaceFactory.cxx                              \
		           DefaultDSLBase.cxx                                     \
		           DefaultDSL.cxx                                         \
		           DefaultCZMDSL.cxx                                      \
		           DefaultFiniteStrainDSL.cxx                             \
		           DefaultGenericBehaviourDSL.cxx                         \
		           DefaultModelDSL.cxx                                    \
		           ModelDSL.cxx                                           \
		           MaterialPropertyDSL.cxx                                \
		           IsotropicMisesCreepDSL.cxx                             \
		           IsotropicStrainHardeningMisesCreepDSL.cxx              \
		           IsotropicMisesPlasticFlowDSL.cxx                       \
		           RungeKuttaDSLBase.cxx                                  \
		           RungeKuttaDSL.cxx                                      \
		           RungeKuttaFiniteStrainDSL.cxx                          \
		           RungeKuttaGenericBehaviourDSL.cxx                      \
		           RungeKuttaModelDSL.cxx                                 \
		           ImplicitDSLBase.cxx                                    \
		           ImplicitDSL.cxx                                        \
		           ImplicitDSLII.cxx                                      \
		           ImplicitFiniteStrainDSL.cxx                            \
		           ImplicitGenericBehaviourDSL.cxx                        \
		           ImplicitModelDSL.cxx                                   \
		           NonLinearSystemSolver.cxx                              \
		           NonLinearSystemSolverBase.cxx                          \
		           NonLinearSystemSolverFactory.cxx                       \
		           PowellDogLegAlgorithmBase.cxx                          \
		           NewtonRaphsonSolvers.cxx                               \
		           BroydenSolvers.cxx		      		          \
		           SecondBroydenSolver.cxx			          \
		           LevenbergMarquardtSolvers.cxx			  \
		           UserDefinedNonLinearSystemSolver.cxx			  \
		           PerformanceProfiling.cxx                               \
		           MultipleIsotropicMisesFlowsDSL.cxx                     \
		           IsotropicBehaviourDSLBase.cxx                          \
		           MFrontMaterialPropertyInterface.cxx                    \
		           CMaterialPropertyInterfaceBase.cxx                     \
		           CMaterialPropertyInterface.cxx                         \
		           CppMaterialPropertyInterface.cxx                       \
		           CppTestMaterialPropertyInterface.cxx                   \
		           ExcelMaterialPropertyInterface.cxx                     \
		           ExcelMaterialPropertyInternalInterface.cxx             \
			   OctaveMaterialPropertyInterface.cxx                    \
		           MFrontModelInterface.cxx                               \
		           BehaviourBrickDescription.cxx                             \
		           AbstractBehaviourBrick.cxx                             \
		           BehaviourBrickBase.cxx                                 \
		           StandardElasticityBrick.cxx                            \
		           StandardElastoViscoPlasticityBrick.cxx                 \
		           DDIF2Brick.cxx                                         \
		           CamClayElasticityBrick.cxx                             \
			   FiniteStrainSingleCrystalBrick.cxx                        \
		           LocalDataStructure.cxx                                 \
		           BehaviourBrickFactory.cxx                              \
		           BehaviourMaterialProperty.cxx                          \
		           StandardBehaviourInterface.cxx                         \
                           UMATInterfaceBase.cxx                          \
		           SymbolsGenerator.cxx                                   \
		           GenericBehaviourInterface.cxx                          \
		           GenericBehaviourSymbolsGenerator.cxx                   \
		           OptionDescription.cxx                                  \
		           BrickUtilities.cxx                                     \
		           StressPotential.cxx                                    \
		           StressPotentialBase.cxx                                \
		           HookeStressPotentialBase.cxx                           \
		           HookeStressPotential.cxx                               \
		           IsotropicDamageHookeStressPotentialBase.cxx            \
		           IsotropicDamageHookeStressPotential.cxx                \
		           DDIF2StressPotential.cxx                               \
		           CamClayStressPotential.cxx                             \
		           StressPotentialFactory.cxx                             \
		           InelasticFlowFactory.cxx                               \
		           InelasticFlow.cxx                                      \
		           InelasticFlowBase.cxx                                  \
		           ViscoplasticFlowBase.cxx                               \
		           NortonHoffViscoplasticFlow.cxx                         \
		           HyperbolicSineViscoplasticFlow.cxx                     \
		           PlasticInelasticFlow.cxx                               \
		           HarmonicSumOfNortonHoffViscoplasticFlows.cxx           \
		           PorosityNucleationModelFactory.cxx                     \
		           PorosityNucleationModel.cxx                            \
		           PorosityNucleationModelBase.cxx                        \
                   StrainBasedPorosityNucleationModelBase.cxx             \
                   StressBasedPorosityNucleationModelBase.cxx             \
                   PowerLawStrainBasedPorosityNucleationModel.cxx         \
                   PowerLawStressBasedPorosityNucleationModel.cxx         \
                   ChuNeedleman1980StrainBasedPorosityNucleationModel.cxx \
                   ChuNeedleman1980StressBasedPorosityNucleationModel.cxx \
		           StressCriterion.cxx                                    \
		           StressCriterionBase.cxx                                \
		           StandardStressCriterionBase.cxx                        \
		           StandardPorousStressCriterionBase.cxx                  \
		           MisesStressCriterion.cxx                               \
		           Hill1948StressCriterion.cxx                            \
		           Barlat2004StressCriterion.cxx                          \
		           Hosford1972StressCriterion.cxx                         \
		           Drucker1949StressCriterion.cxx                         \
		           Cazacu2001StressCriterion.cxx                          \
		           Cazacu2004IsotropicStressCriterion.cxx                 \
		           Cazacu2004OrthotropicStressCriterion.cxx               \
		           MohrCoulombStressCriterion.cxx                         \
		           MichelAndSuquet1992HollowSphereStressCriterion.cxx     \
		           GursonTvergaardNeedleman1982StressCriterion.cxx        \
		           RousselierTanguyBesson2002StressCriterion.cxx          \
		           StressCriterionFactory.cxx                             \
		           IsotropicHardeningRule.cxx                             \
		           LinearIsotropicHardeningRule.cxx                       \
		           PowerIsotropicHardeningRule.cxx                        \
		           SwiftIsotropicHardeningRule.cxx                        \
		           VoceIsotropicHardeningRule.cxx                         \
		           IsotropicHardeningRuleFactory.cxx                      \
		           KinematicHardeningRule.cxx                             \
		           KinematicHardeningRuleBase.cxx                         \
		           PragerKinematicHardeningRule.cxx                       \
		           Chaboche2012KinematicHardeningRule.cxx                 \
		           BurletCailletaudKinematicHardeningRule.cxx             \
		           ArmstrongFrederickKinematicHardeningRule.cxx           \
		           KinematicHardeningRuleFactory.cxx

if HAVE_GNUPLOT
libTFELMFront_la_SOURCES += GnuplotMaterialPropertyInterface.cxx
endif

if HAVE_FORTRAN
libTFELMFront_la_SOURCES += FortranMaterialPropertyInterface.cxx   \
			    Fortran03MaterialPropertyInterface.cxx
endif

if HAVE_PYTHON
libTFELMFront_la_SOURCES += PythonMaterialPropertyInterface.cxx
endif

if HAVE_JAVA
libTFELMFront_la_SOURCES += JavaMaterialPropertyInterface.cxx
endif

if HAVE_CASTEM
libTFELMFront_la_SOURCES += CastemInterface.cxx                 \
                            Castem21Interface.cxx               \
                            CastemSymbolsGenerator.cxx          \
                            CastemMaterialPropertyInterface.cxx \
                            AmitexInterface.cxx
endif

if HAVE_ASTER
libTFELMFront_la_SOURCES += AsterInterface.cxx \
                            AsterSymbolsGenerator.cxx
endif

if HAVE_ABAQUS
libTFELMFront_la_SOURCES += AbaqusInterfaceBase.cxx            \
			    AbaqusInterface.cxx                \
			    AbaqusSymbolsGenerator.cxx         \
			    AbaqusExplicitInterface.cxx        \
			    AbaqusExplicitSymbolsGenerator.cxx
endif

if HAVE_LSDYNA
libTFELMFront_la_SOURCES += LSDYNAInterface.cxx \
                            LSDYNASymbolsGenerator.cxx
endif

if HAVE_DIANAFEA
libTFELMFront_la_SOURCES += DianaFEAInterface.cxx \
                            DianaFEASymbolsGenerator.cxx
endif

if HAVE_CALCULIX
libTFELMFront_la_SOURCES += CalculiXInterface.cxx \
                            CalculiXSymbolsGenerator.cxx
endif

if HAVE_COMSOL
libTFELMFront_la_SOURCES += ComsolInterface.cxx
#                            ComsolSymbolsGenerator.cxx
endif

if HAVE_ANSYS
libTFELMFront_la_SOURCES += AnsysInterface.cxx \
                            AnsysSymbolsGenerator.cxx
endif

if HAVE_EUROPLEXUS
libTFELMFront_la_SOURCES += EuroplexusInterface.cxx        \
                            EuroplexusSymbolsGenerator.cxx
endif

if HAVE_ZMAT
libTFELMFront_la_SOURCES += ZMATInterface.cxx
endif

if HAVE_CYRANO
libTFELMFront_la_SOURCES += CyranoMaterialPropertyInterface.cxx \
                            CyranoInterface.cxx                 \
                            CyranoSymbolsGenerator.cxx
endif
libTFELMFront_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELMFront_EXPORTS

libTFELMFront_la_LIBADD  = -L$(top_builddir)/mfront/src -lMFrontLogStream      \
	   	           -L$(top_builddir)/src/Material  -lTFELMaterial            \
		           -L$(top_builddir)/src/Math      -lTFELMathParser          \
		           -L$(top_builddir)/src/System    -lTFELSystem              \
		           -L$(top_builddir)/src/Glossary  -lTFELGlossary            \
		           -L$(top_builddir)/src/Utilities -lTFELUtilities           \
		           -L$(top_builddir)/src/UnicodeSupport -lTFELUnicodeSupport \
		           -L$(top_builddir)/src/Exception -lTFELException           \
		           -L$(top_builddir)/src/Config    -lTFELConfig
if TFEL_WIN
libTFELMFront_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN

if !TFEL_WIN
libTFELMFront_la_LIBADD   +=  -lpthread
if TFEL_LINUX
libTFELMFront_la_LIBADD   +=  -ldl
endif
endif # !TFEL_WIN

libTFELMFront_la_DEPENDENCIES = libMFrontLogStream.la

lib_LTLIBRARIES += libMFrontProfiling.la
libMFrontProfiling_la_SOURCES = BehaviourProfiler.cxx
if TFEL_LINUX
libMFrontProfiling_la_LIBADD   = -lrt
endif
libMFrontProfiling_la_CPPFLAGS = $(AM_CPPFLAGS) -DMFrontProfiling_EXPORTS
if TFEL_WIN
libMFrontProfiling_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN

libMTestFileGenerator_la_SOURCES = MTestFileGeneratorBase.cxx         \
				   UmatSmallStrainMTestFileGenerator.cxx              \
				   UmatFiniteStrainMTestFileGenerator.cxx             \
				   GenericBehaviourSmallStrainMTestFileGenerator.cxx  \
				   GenericBehaviourFiniteStrainMTestFileGenerator.cxx

libMTestFileGenerator_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities \
				   -L$(top_builddir)/src/Material  -lTFELMaterial  \
				   -L$(top_builddir)/src/Math      -lTFELMath      \
				   -L$(top_builddir)/src/Exception -lTFELException
libMTestFileGenerator_la_CPPFLAGS = $(AM_CPPFLAGS) -DMTestFileGenerator_EXPORTS
if TFEL_WIN
libMTestFileGenerator_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN

# Castem Interface
if HAVE_CASTEM
lib_LTLIBRARIES += libCastemInterface.la
libCastemInterface_la_SOURCES = CastemInterfaceExceptions.cxx                      \
			        CastemFiniteStrain.cxx                             \
			        CastemComputeStiffnessTensor.cxx                   \
			        CastemComputeThermalExpansionCoefficientTensor.cxx \
			        CastemRotationMatrix.cxx                           \
			        CastemTangentOperator.cxx                          \
		                CastemOutOfBoundsPolicy.cxx                        \
		                CastemGetModellingHypothesis.cxx                   \
			        CastemStressFreeExpansionHandler.cxx               \
		                CastemException.cxx
libCastemInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities        \
			      -L$(top_builddir)/src/Exception -lTFELException          \
			      -L$(top_builddir)/mfront/src    -lMTestFileGenerator
libCastemInterface_la_DEPENDENCIES = libMTestFileGenerator.la
libCastemInterface_la_CPPFLAGS = $(AM_CPPFLAGS) -DCastemInterface_EXPORTS
if TFEL_WIN
libCastemInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_CASTEM

if HAVE_CYRANO
lib_LTLIBRARIES += libCyranoInterface.la
libCyranoInterface_la_SOURCES = CyranoInterfaceExceptions.cxx                      \
			        CyranoComputeStiffnessTensor.cxx                   \
			        CyranoComputeThermalExpansionCoefficientTensor.cxx \
			        CyranoStressFreeExpansionHandler.cxx               \
		                CyranoOutOfBoundsPolicy.cxx                        \
		                CyranoGetModellingHypothesis.cxx                   \
		                CyranoException.cxx
libCyranoInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities          \
			        -L$(top_builddir)/src/Exception -lTFELException          \
			        -L$(top_builddir)/mfront/src    -lMTestFileGenerator
libCyranoInterface_la_DEPENDENCIES = libMTestFileGenerator.la
libCyranoInterface_la_CPPFLAGS = $(AM_CPPFLAGS) -DCyranoInterface_EXPORTS
if TFEL_WIN
libCyranoInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_CYRANO

# Aster Interface
if HAVE_ASTER
lib_LTLIBRARIES += libAsterInterface.la
libAsterInterface_la_SOURCES = AsterInterfaceExceptions.cxx                      \
		               AsterException.cxx                                \
		               AsterTangentOperator.cxx                          \
			       AsterRotationMatrix.cxx                           \
			       AsterFiniteStrainRotationMatrix.cxx               \
			       AsterComputeStiffnessTensor.cxx                   \
			       AsterStressFreeExpansionHandler.cxx               \
			       AsterComputeThermalExpansionCoefficientTensor.cxx

libAsterInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities          \
			       -L$(top_builddir)/src/Exception -lTFELException          \
			       -L$(top_builddir)/mfront/src    -lMTestFileGenerator
libAsterInterface_la_DEPENDENCIES = libMTestFileGenerator.la
libAsterInterface_la_CPPFLAGS     = $(AM_CPPFLAGS) -DAsterInterface_EXPORTS
if TFEL_WIN
libAsterInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_ASTER

# Abaqus Interface
if HAVE_ABAQUS
lib_LTLIBRARIES += libAbaqusInterface.la
libAbaqusInterface_la_SOURCES = AbaqusInterfaceExceptions.cxx                      \
		                AbaqusException.cxx                                \
			        AbaqusComputeStiffnessTensor.cxx                   \
			        AbaqusStressFreeExpansionHandler.cxx               \
			        AbaqusComputeThermalExpansionCoefficientTensor.cxx \
			        AbaqusFiniteStrain.cxx

libAbaqusInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities          \
			       -L$(top_builddir)/src/Exception -lTFELException          \
			       -L$(top_builddir)/mfront/src    -lMTestFileGenerator
libAbaqusInterface_la_DEPENDENCIES = libMTestFileGenerator.la
libAbaqusInterface_la_CPPFLAGS     = $(AM_CPPFLAGS) -DAbaqusInterface_EXPORTS
if TFEL_WIN
libAbaqusInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_ABAQUS

# CalculiX Interface
if HAVE_CALCULIX
lib_LTLIBRARIES += libCalculiXInterface.la
libCalculiXInterface_la_SOURCES = CalculiXInterfaceExceptions.cxx                      \
				  CalculiXException.cxx                                \
			          CalculiXRotationMatrix.cxx                           \
				  CalculiXComputeStiffnessTensor.cxx                   \
			          CalculiXStressFreeExpansionHandler.cxx               \
			          CalculiXComputeThermalExpansionCoefficientTensor.cxx


libCalculiXInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities       \
			          -L$(top_builddir)/src/Exception -lTFELException       \
			          -L$(top_builddir)/mfront/src    -lMTestFileGenerator
libCalculiXInterface_la_DEPENDENCIES = libMTestFileGenerator.la
libCalculiXInterface_la_CPPFLAGS     = $(AM_CPPFLAGS) -DCalculiXInterface_EXPORTS
if TFEL_WIN
libCalculiXInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_CALCULIX

# Ansys Interface
if HAVE_ANSYS
lib_LTLIBRARIES += libAnsysInterface.la
libAnsysInterface_la_SOURCES = AnsysInterfaceExceptions.cxx                      \
		               AnsysException.cxx                                \
			       AnsysComputeStiffnessTensor.cxx                   \
			       AnsysStressFreeExpansionHandler.cxx               \
			       AnsysComputeThermalExpansionCoefficientTensor.cxx \
			       AnsysFiniteStrain.cxx

libAnsysInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities          \
			       -L$(top_builddir)/src/Exception -lTFELException          \
			       -L$(top_builddir)/mfront/src    -lMTestFileGenerator
libAnsysInterface_la_DEPENDENCIES = libMTestFileGenerator.la
libAnsysInterface_la_CPPFLAGS     = $(AM_CPPFLAGS) -DAnsysInterface_EXPORTS
if TFEL_WIN
libAnsysInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_ANSYS

# DianaFEA Interface
if HAVE_DIANAFEA
lib_LTLIBRARIES += libDianaFEAInterface.la
libDianaFEAInterface_la_SOURCES = DianaFEAInterfaceExceptions.cxx                   \
		               DianaFEAException.cxx                                \
		               DianaFEAOutOfBoundsPolicy.cxx                        \
		               DianaFEAComputeStiffnessTensor.cxx                   \
			       DianaFEAStressFreeExpansionHandler.cxx               \
			       DianaFEAComputeThermalExpansionCoefficientTensor.cxx

libDianaFEAInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities          \
			       -L$(top_builddir)/src/Exception -lTFELException          \
			       -L$(top_builddir)/mfront/src    -lMTestFileGenerator
libDianaFEAInterface_la_DEPENDENCIES = libMTestFileGenerator.la
libDianaFEAInterface_la_CPPFLAGS     = $(AM_CPPFLAGS) -DDianaFEAInterface_EXPORTS
if TFEL_WIN
libDianaFEAInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_DIANAFEA

# Europlexus Interface
if HAVE_EUROPLEXUS
lib_LTLIBRARIES += libEuroplexusInterface.la
libEuroplexusInterface_la_SOURCES = EuroplexusInterfaceExceptions.cxx                      \
		                    EuroplexusException.cxx                                \
			            EuroplexusComputeStiffnessTensor.cxx                   \
			            EuroplexusStressFreeExpansionHandler.cxx               \
			            EuroplexusComputeThermalExpansionCoefficientTensor.cxx \
			            EuroplexusFiniteStrain.cxx

libEuroplexusInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities          \
			            -L$(top_builddir)/src/Exception -lTFELException          \
			            -L$(top_builddir)/mfront/src    -lMTestFileGenerator
libEuroplexusInterface_la_DEPENDENCIES = libMTestFileGenerator.la
libEuroplexusInterface_la_CPPFLAGS     = $(AM_CPPFLAGS) -DEuroplexusInterface_EXPORTS
if TFEL_WIN
libEuroplexusInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_EUROPLEXUS

# LSDYNA Interface
if HAVE_LSDYNA
lib_LTLIBRARIES += libLSDYNAInterface.la
libLSDYNAInterface_la_SOURCES = LSDYNAInterfaceExceptions.cxx                      \
		                LSDYNAException.cxx                                \
			        LSDYNAComputeStiffnessTensor.cxx                   \
			        LSDYNAStressFreeExpansionHandler.cxx               \
			        LSDYNAComputeThermalExpansionCoefficientTensor.cxx \
			        LSDYNAFiniteStrain.cxx

libLSDYNAInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities          \
			       -L$(top_builddir)/src/Exception -lTFELException          \
			       -L$(top_builddir)/mfront/src    -lMTestFileGenerator
libLSDYNAInterface_la_DEPENDENCIES = libMTestFileGenerator.la
libLSDYNAInterface_la_CPPFLAGS     = $(AM_CPPFLAGS) -DLSDYNAInterface_EXPORTS
if TFEL_WIN
libLSDYNAInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_LSDYNA

mfront_SOURCES = main.cxx
mfront_LDADD = -L$(top_builddir)/mfront/src -lTFELMFront -lMFrontLogStream \
               -L$(top_builddir)/src/Utilities -lTFELUtilities             \
               -L$(top_builddir)/src/System -lTFELSystem

mfront_DEPENDENCIES = libTFELMFront.la libMFrontLogStream.la
if !TFEL_WIN
mfront_LDFLAGS  = -export-dynamic
endif # TFEL_WIN

EXTRA_DIST = CMakeLists.txt
