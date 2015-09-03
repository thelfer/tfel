# Makefile.am -- Process this file with automake to produce a Makefile.in file.

AM_CPPFLAGS = -I$(top_builddir)/src/TFELConfig \
	      -I$(top_srcdir)/include          \
	      -I$(top_srcdir)/mfront/include

if HAVE_CASTEM
if !LOCAL_CASTEM_HEADER
AM_CPPFLAGS += -I$(CASTEM_ROOT)/include
AM_CPPFLAGS += -I$(CASTEM_ROOT)/include/c
endif
AM_CPPFLAGS += $(CASTEM_CPPFLAGS)
endif

bin_PROGRAMS = mfront mfront-query mfront-doc
lib_LTLIBRARIES = libMFrontLogStream.la libTFELMFront.la libMTestFileGenerator.la

libMFrontLogStream_la_SOURCES  = MFrontLogStream.cxx
libMFrontLogStream_la_CPPFLAGS = $(AM_CPPFLAGS) -DMFrontLogStream_EXPORTS
if TFEL_WIN
libMFrontLogStream_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN

libTFELMFront_la_SOURCES = MFront.cxx                                             \
			   MFrontBase.cxx                                         \
			   MFrontUtilities.cxx                                    \
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
		           BehaviourData.cxx                                      \
		           BehaviourDescription.cxx                               \
		           BoundsDescription.cxx                                  \
		           VariableDescription.cxx                                \
		           StaticVariableDescription.cxx                          \
		           VariableBoundsDescription.cxx                          \
		           BehaviourDSLCommon.cxx                                 \
		           ModelData.cxx                                          \
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
		           SearchFile.cxx                                         \
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
		           RungeKuttaDSL.cxx                                      \
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
		           AbstractBehaviourBrick.cxx                             \
		           BehaviourBrickBase.cxx                                 \
			   BehaviourBrickProviderIdentifier.cxx                   \
			   BehaviourBrickRequirement.cxx                          \
			   BehaviourBrickRequirementManager.cxx                   \
			   BehaviourBrickProvider.cxx                             \
		           ElasticityBehaviourBrick.cxx                           \
		           AbstractBehaviourBrickFactory.cxx                      \
		           UMATInterfaceBase.cxx


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
libTFELMFront_la_SOURCES += UMATInterface.cxx      \
		  CastemMaterialPropertyInterface.cxx
endif

if HAVE_ASTER
libTFELMFront_la_SOURCES += AsterInterface.cxx
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
		           -L$(top_builddir)/src/Exception -lTFELException
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
				   UMATSmallStrainMTestFileGenerator.cxx  \
				   UMATFiniteStrainMTestFileGenerator.cxx

libMTestFileGenerator_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities \
				   -L$(top_builddir)/src/Material  -lTFELMaterial  \
				   -L$(top_builddir)/src/Math      -lTFELMath      \
				   -L$(top_builddir)/src/Exception -lTFELException
libMTestFileGenerator_la_CPPFLAGS = $(AM_CPPFLAGS) -DMTestFileGenerator_EXPORTS
if TFEL_WIN
libMTestFileGenerator_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN

# UMAT Interface
if HAVE_CASTEM
lib_LTLIBRARIES += libUMATInterface.la
libUMATInterface_la_SOURCES = UMATInterfaceExceptions.cxx                      \
			      UMATFiniteStrain.cxx                             \
			      UMATComputeStiffnessTensor.cxx                   \
			      UMATComputeThermalExpansionCoefficientTensor.cxx \
			      UMATRotationMatrix.cxx                           \
			      UMATFiniteStrainRotationMatrix.cxx               \
			      UMATTangentOperator.cxx                          \
		              UMATOutOfBoundsPolicy.cxx                        \
		              UMATGetModellingHypothesis.cxx                   \
			      UMATStressFreeExpansionHandler.cxx               \
		              UMATException.cxx
libUMATInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities          \
			      -L$(top_builddir)/src/Exception -lTFELException          \
			      -L$(top_builddir)/mfront/src    -lMTestFileGenerator
libUMATInterface_la_DEPENDENCIES = libMTestFileGenerator.la
libUMATInterface_la_CPPFLAGS = $(AM_CPPFLAGS) -DUMATInterface_EXPORTS
if TFEL_WIN
libUMATInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_CASTEM

if HAVE_CYRANO
lib_LTLIBRARIES += libCyranoInterface.la
libCyranoInterface_la_SOURCES = CyranoInterfaceExceptions.cxx                      \
			        CyranoComputeStiffnessTensor.cxx                   \
			        CyranoComputeThermalExpansionCoefficientTensor.cxx \
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

mfront_SOURCES = main.cxx
mfront_LDADD = -L$(top_builddir)/mfront/src -lTFELMFront -lMFrontLogStream \
               -L$(top_builddir)/src/Utilities -lTFELUtilities             \
               -L$(top_builddir)/src/System -lTFELSystem

mfront_DEPENDENCIES = libTFELMFront.la libMFrontLogStream.la
if !TFEL_WIN
mfront_LDFLAGS  = -export-dynamic
endif # TFEL_WIN

mfront_query_SOURCES = mfront-query.cxx BehaviourQuery.cxx
mfront_query_LDADD = -L$(top_builddir)/mfront/src -lTFELMFront -lMFrontLogStream \
                     -L$(top_builddir)/src/Glossary -lTFELGlossary   \
                     -L$(top_builddir)/src/Material -lTFELMaterial   \
                     -L$(top_builddir)/src/Math -lTFELMath           \
                     -L$(top_builddir)/src/Utilities -lTFELUtilities \
                     -L$(top_builddir)/src/System -lTFELSystem
mfront_query_DEPENDENCIES = libTFELMFront.la libMFrontLogStream.la

if !TFEL_WIN
mfront_query_LDFLAGS  = -export-dynamic
endif # TFEL_WIN

mfront_doc_SOURCES = mfront-doc.cxx BehaviourDocumentationGenerator.cxx
mfront_doc_LDADD = -L$(top_builddir)/mfront/src -lTFELMFront -lMFrontLogStream \
                     -L$(top_builddir)/src/Glossary -lTFELGlossary   \
                     -L$(top_builddir)/src/Material -lTFELMaterial   \
                     -L$(top_builddir)/src/Math -lTFELMath           \
                     -L$(top_builddir)/src/Utilities -lTFELUtilities \
                     -L$(top_builddir)/src/System -lTFELSystem
mfront_doc_DEPENDENCIES = libTFELMFront.la libMFrontLogStream.la

if !TFEL_WIN
mfront_doc_LDFLAGS  = -export-dynamic
endif # TFEL_WIN

EXTRA_DIST = CMakeLists.txt
