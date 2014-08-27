## Makefile.am -- Process this file with automake to produce a Makefile.in file.

INCLUDES = -I$(top_builddir)/src/TFELConfig \
	   -I$(top_srcdir)/include          \
	   -I$(top_srcdir)/mfront/include

if HAVE_CASTEM
if !LOCAL_CASTEM_HEADER
INCLUDES += -I$(CASTEM_ROOT)/include
endif
INCLUDES += $(CASTEM_CPPFLAGS)
endif

bin_PROGRAMS = mfront
mfront_SOURCES = main.cxx                                               \
		 MFront.cxx                                             \
		 FiniteStrainBehaviourTangentOperatorConversion.cxx     \
		 FiniteStrainBehaviourTangentOperatorConversionPath.cxx \
	 	 MFrontDebugMode.cxx                                    \
	 	 MFrontPedanticMode.cxx                                 \
		 MFrontLock.cxx                                         \
		 MFrontInitAnalysers.cxx                                \
		 MFrontInitInterfaces.cxx                               \
		 MFrontInitParsers.cxx                                  \
		 MFrontParserFactory.cxx                                \
		 DrivingVariable.cxx                                    \
		 MaterialPropertyDescription.cxx                        \
		 MechanicalBehaviourData.cxx                            \
		 MechanicalBehaviourDescription.cxx                     \
		 BoundsDescription.cxx                                  \
		 VariableDescription.cxx                                \
		 StaticVariableDescription.cxx                          \
		 MFrontBehaviourParserCommon.cxx                        \
		 MFrontModelParserCommon.cxx                            \
		 MFrontBehaviourVirtualParser.cxx                       \
		 MFrontBehaviourVirtualInterface.cxx                    \
		 MFrontLawVirtualInterface.cxx                          \
		 MFrontModelVirtualInterface.cxx                        \
		 MFrontBehaviourAnalyser.cxx                            \
		 SupportedTypes.cxx                                     \
		 MFrontHeader.cxx                                       \
		 ParserBase.cxx                                         \
		 InterfaceBase.cxx                                      \
		 ParserUtilities.cxx                                    \
		 MFrontSearchFile.cxx                                   \
		 MFrontLawInterfaceFactory.cxx                          \
		 MFrontBehaviourInterfaceFactory.cxx                    \
		 MFrontBehaviourAnalyserFactory.cxx                     \
		 MFrontModelInterfaceFactory.cxx                        \
		 MFrontDefaultParserBase.cxx                            \
		 MFrontDefaultParser.cxx                                \
		 MFrontDefaultCZMParser.cxx                             \
		 MFrontDefaultFiniteStrainParser.cxx                    \
		 MFrontModelParser.cxx                                  \
		 MFrontMaterialLawParser.cxx                            \
		 MFrontIsotropicMisesCreepParser.cxx                    \
		 MFrontIsotropicStrainHardeningMisesCreepParser.cxx     \
		 MFrontIsotropicMisesPlasticFlowParser.cxx              \
		 MFrontRungeKuttaParser.cxx                             \
		 MFrontImplicitParserBase.cxx                           \
		 MFrontImplicitParser.cxx                               \
		 MFrontImplicitParserII.cxx                             \
		 MFrontImplicitFiniteStrainParser.cxx                   \
		 MFrontNonLinearSystemSolver.cxx                        \
		 MFrontNonLinearSystemSolverBase.cxx                    \
		 MFrontNonLinearSystemSolverFactory.cxx                 \
		 MFrontPowellDogLegAlgorithmBase.cxx                    \
		 MFrontNewtonRaphsonSolvers.cxx                         \
		 MFrontBroydenSolvers.cxx				\
		 MFrontSecondBroydenSolver.cxx				\
		 MFrontLevenbergMarquardtSolvers.cxx			\
		 MFrontPerformanceProfiling.cxx                         \
		 MFrontMultipleIsotropicMisesFlowsParser.cxx            \
		 MFrontIsotropicBehaviourParserBase.cxx                 \
		 MFrontMFrontLawInterface.cxx                           \
		 MFrontCLawInterfaceBase.cxx                            \
		 MFrontCLawInterface.cxx                                \
		 MFrontCppLawInterface.cxx                              \
		 MFrontCppTestLawInterface.cxx                          \
		 MFrontExcelLawInterface.cxx                            \
		 MFrontLaTeXLawInterface.cxx                            \
		 MFrontLaTeXBehaviourAnalyser.cxx                       \
		 MFrontMarkdownBehaviourAnalyser.cxx                    \
		 MFrontUMATInterfaceBase.cxx


if HAVE_GNUPLOT
mfront_SOURCES += MFrontGnuplotLawInterface.cxx
endif

if HAVE_FORTRAN
mfront_SOURCES += MFrontFortranLawInterface.cxx
endif

if HAVE_PYTHON
mfront_SOURCES += MFrontPythonLawInterface.cxx
endif

if HAVE_CASTEM
mfront_SOURCES += MFrontUMATInterface.cxx      \
		  MFrontCastemLawInterface.cxx
endif

if HAVE_ASTER
mfront_SOURCES += MFrontAsterInterface.cxx
endif

if HAVE_ZMAT
mfront_SOURCES += MFrontZMATInterface.cxx
endif

if HAVE_CYRANO
mfront_SOURCES += MFrontCyranoInterface.cxx
endif

mfront_LDADD   = -L$(top_builddir)/mfront/src    -lMFrontLogStream \
	   	 -L$(top_builddir)/src/Material  -lTFELMaterial    \
		 -L$(top_builddir)/src/Math      -lTFELMathParser  \
		 -L$(top_builddir)/src/System    -lTFELSystem      \
		 -L$(top_builddir)/src/Glossary  -lTFELGlossary    \
		 -L$(top_builddir)/src/Utilities -lTFELUtilities   \
		 -L$(top_builddir)/src/Exception -lTFELException

if !TFEL_WIN
mfront_LDADD   +=  -lpthread
if TFEL_LINUX
mfront_LDADD   +=  -ldl
endif
mfront_LDFLAGS  = -export-dynamic
endif # !TFEL_WIN

lib_LTLIBRARIES = libMFrontLogStream.la libMTestFileGenerator.la

libMFrontLogStream_la_SOURCES  = MFrontLogStream.cxx
libMFrontLogStream_la_CPPFLAGS = -DMFrontLogStream_EXPORTS
if TFEL_WIN
libMFrontLogStream_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN

if HAVE_CXX_11
lib_LTLIBRARIES += libMFrontProfiling.la
libMFrontProfiling_la_SOURCES = MFrontBehaviourProfiler.cxx
if ! TFEL_WIN
libMFrontProfiling_la_LIBADD   = -lrt
endif
libMFrontProfiling_la_CPPFLAGS = -DMFrontProfiling_EXPORTS
if TFEL_WIN
libMFrontProfiling_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_CXX_11

libMTestFileGenerator_la_SOURCES = MTestFileGeneratorBase.cxx             \
				   UMATSmallStrainMTestFileGenerator.cxx  \
				   UMATFiniteStrainMTestFileGenerator.cxx

libMTestFileGenerator_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities \
				   -L$(top_builddir)/src/Material  -lTFELMaterial  \
				   -L$(top_builddir)/src/Math      -lTFELMath      \
				   -L$(top_builddir)/src/Exception -lTFELException
libMTestFileGenerator_la_CPPFLAGS = -DMTestFileGenerator_EXPORTS
if TFEL_WIN
libMTestFileGenerator_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN

# UMAT Interface
if HAVE_CASTEM
lib_LTLIBRARIES += libUMATInterface.la
libUMATInterface_la_SOURCES = UMATInterfaceBase.cxx                            \
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
libUMATInterface_la_CPPFLAGS = -DUMATInterface_EXPORTS
if TFEL_WIN
libUMATInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_CASTEM

if HAVE_CYRANO
lib_LTLIBRARIES += libCyranoInterface.la
libCyranoInterface_la_SOURCES = CyranoInterfaceBase.cxx                            \
			        CyranoComputeStiffnessTensor.cxx                   \
			        CyranoComputeThermalExpansionCoefficientTensor.cxx \
		                CyranoOutOfBoundsPolicy.cxx                        \
		                CyranoGetModellingHypothesis.cxx                   \
		                CyranoException.cxx
libCyranoInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities          \
			        -L$(top_builddir)/src/Exception -lTFELException          \
			        -L$(top_builddir)/mfront/src    -lMTestFileGenerator
libCyranoInterface_la_DEPENDENCIES = libMTestFileGenerator.la
libCyranoInterface_la_CPPFLAGS = -DCyranoInterface_EXPORTS
if TFEL_WIN
libCyranoInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_CYRANO

# Aster Interface
if HAVE_ASTER
lib_LTLIBRARIES += libAsterInterface.la
libAsterInterface_la_SOURCES = AsterInterfaceBase.cxx                            \
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
libAsterInterface_la_CPPFLAGS     = -DAsterInterface_EXPORTS
if TFEL_WIN
libAsterInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
endif # HAVE_ASTER


EXTRA_DIST = CMakeLists.txt