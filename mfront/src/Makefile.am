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

libTFELMFront_la_SOURCES = MFront.cxx                                             \
			   MFrontBase.cxx                                         \
			   MFrontUtilities.cxx                                    \
			   GeneratorOptions.cxx                                   \
			   CMakeGenerator.cxx                                     \
			   MakefileGenerator.cxx                                  \
		           FileDescription.cxx                                    \
		           TargetsDescription.cxx                                 \
		           LibraryDescription.cxx                                 \
		           CodeBlock.cxx                                          \
		           FiniteStrainBehaviourTangentOperatorConversion.cxx     \
		           FiniteStrainBehaviourTangentOperatorConversionPath.cxx \
		           MFrontDebugMode.cxx                                    \
		           PedanticMode.cxx                                       \
		           MFrontLock.cxx                                         \
		           InitInterfaces.cxx                                     \
		           InitDSLs.cxx                                           \
		           DSLFactory.cxx                                         \
		           DrivingVariable.cxx                                    \
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
		           SearchPathsHandler.cxx                                 \
		           MaterialPropertyInterfaceFactory.cxx                   \
		           BehaviourInterfaceFactory.cxx                          \
		           ModelInterfaceFactory.cxx                              \
		           DefaultDSLBase.cxx                                     \
		           DefaultDSL.cxx                                         \
		           DefaultCZMDSL.cxx                                      \
		           DefaultFiniteStrainDSL.cxx                             \
		           ModelDSL.cxx                                           \
		           MaterialPropertyDSL.cxx                                \
		           IsotropicMisesCreepDSL.cxx                             \
		           IsotropicStrainHardeningMisesCreepDSL.cxx              \
		           IsotropicMisesPlasticFlowDSL.cxx                       \
		           RungeKuttaDSLBase.cxx                                  \
		           RungeKuttaDSL.cxx                                      \
		           RungeKuttaFiniteStrainDSL.cxx                          \
		           ImplicitDSLBase.cxx                                    \
		           ImplicitDSL.cxx                                        \
		           ImplicitDSLII.cxx                                      \
		           ImplicitFiniteStrainDSL.cxx                            \
		           NonLinearSystemSolver.cxx                              \
		           NonLinearSystemSolverBase.cxx                          \
		           NonLinearSystemSolverFactory.cxx                       \
		           PowellDogLegAlgorithmBase.cxx                          \
		           NewtonRaphsonSolvers.cxx                               \
		           BroydenSolvers.cxx		      		          \
		           SecondBroydenSolver.cxx			          \
		           LevenbergMarquardtSolvers.cxx			  \
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
		           AbstractBehaviourBrick.cxx                             \
		           BehaviourBrickBase.cxx                                 \
			   BehaviourBrickProviderIdentifier.cxx                      \
			   BehaviourBrickRequirement.cxx                             \
			   BehaviourBrickRequirementManager.cxx                      \
			   BehaviourBrickProvider.cxx                                \
		           StandardElasticityBrick.cxx                            \
		           StandardElastoViscoPlasticityBrick.cxx                 \
		           DDIF2Brick.cxx                                         \
			   FiniteStrainSingleCrystalBrick.cxx                        \
		           LocalDataStructure.cxx                                 \
		           AbstractBehaviourBrickFactory.cxx                      \
		           UMATInterfaceBase.cxx                                  \
		           OptionDescription.cxx                                  \
		           BrickUtilities.cxx                                     \
		           StressPotential.cxx                                    \
		           HookeStressPotential.cxx                               \
		           DDIF2StressPotential.cxx                               \
		           StressPotentialFactory.cxx                             \
		           InelasticFlow.cxx                                      \
		           InelasticFlowBase.cxx                                  \
		           NortonInelasticFlow.cxx                                \
		           PlasticInelasticFlow.cxx                               \
		           InelasticFlowFactory.cxx                               \
		           StressCriterion.cxx                                    \
		           MisesStressCriterion.cxx                               \
		           Hill1948StressCriterion.cxx                            \
		           StressCriterionFactory.cxx                             \
		           IsotropicHardeningRule.cxx                             \
		           LinearIsotropicHardeningRule.cxx                       \
		           VoceIsotropicHardeningRule.cxx                         \
		           IsotropicHardeningRuleFactory.cxx                      \
		           KinematicHardeningRule.cxx                             \
		           KinematicHardeningRuleBase.cxx                         \
		           PragerKinematicHardeningRule.cxx                       \
		           AmstrongFrederickKinematicHardeningRule.cxx            \
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
libTFELMFront_la_SOURCES += CastemInterface.cxx      \
		  CastemMaterialPropertyInterface.cxx
endif

if HAVE_ASTER
libTFELMFront_la_SOURCES += AsterInterface.cxx
endif

if HAVE_ABAQUS
libTFELMFront_la_SOURCES += AbaqusInterfaceBase.cxx     \
			    AbaqusInterface.cxx         \
			    AbaqusExplicitInterface.cxx
endif

if HAVE_CALCULIX
libTFELMFront_la_SOURCES += CalculiXInterface.cxx
endif

if HAVE_ANSYS
libTFELMFront_la_SOURCES += AnsysInterface.cxx
endif

if HAVE_EUROPLEXUS
libTFELMFront_la_SOURCES += EuroplexusInterface.cxx
endif

if HAVE_ZMAT
libTFELMFront_la_SOURCES += ZMATInterface.cxx
endif

if HAVE_CYRANO
libTFELMFront_la_SOURCES += CyranoInterface.cxx
endif
libTFELMFront_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELMTest_EXPORTS

libTFELMFront_la_LIBADD  = -L$(top_builddir)/mfront/src    -lMFrontLogStream \
	   	           -L$(top_builddir)/src/Material  -lTFELMaterial    \
		           -L$(top_builddir)/src/Math      -lTFELMathParser  \
		           -L$(top_builddir)/src/System    -lTFELSystem      \
		           -L$(top_builddir)/src/Glossary  -lTFELGlossary    \
		           -L$(top_builddir)/src/Utilities -lTFELUtilities   \
		           -L$(top_builddir)/src/Exception -lTFELException   \
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

libMTestFileGenerator_la_SOURCES = MTestFileGeneratorBase.cxx             \
				   UmatSmallStrainMTestFileGenerator.cxx  \
				   UmatFiniteStrainMTestFileGenerator.cxx

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

mfront_SOURCES = main.cxx
mfront_LDADD = -L$(top_builddir)/mfront/src -lTFELMFront -lMFrontLogStream \
               -L$(top_builddir)/src/Utilities -lTFELUtilities             \
               -L$(top_builddir)/src/System -lTFELSystem

mfront_DEPENDENCIES = libTFELMFront.la libMFrontLogStream.la
if !TFEL_WIN
mfront_LDFLAGS  = -export-dynamic
endif # TFEL_WIN

EXTRA_DIST = CMakeLists.txt
