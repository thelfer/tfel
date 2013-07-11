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
mfront_SOURCES = main.cxx                                             \
		 MFront.cxx                                           \
		 MFrontLock.cxx                                       \
		 MFrontInitInterfaces.cxx                             \
		 MFrontInitParsers.cxx                                \
		 MFrontParserFactory.cxx                              \
		 DrivingVariable.cxx                                  \
		 MechanicalBehaviourDescription.cxx                   \
		 BoundsDescription.cxx                                \
		 VarHandler.cxx                                       \
		 StaticVarHandler.cxx                                 \
		 MFrontBehaviourParserCommon.cxx                      \
		 MFrontModelParserCommon.cxx                          \
		 MFrontBehaviourVirtualParser.cxx                     \
		 MFrontBehaviourVirtualInterface.cxx                  \
		 MFrontLawVirtualInterface.cxx                        \
		 MFrontModelVirtualInterface.cxx                      \
		 SupportedTypes.cxx                                   \
		 MFrontHeader.cxx                                     \
		 ParserBase.cxx                                       \
		 InterfaceBase.cxx                                    \
		 ParserUtilities.cxx                                  \
		 MFrontSearchFile.cxx                                 \
		 MFrontLawInterfaceFactory.cxx                        \
		 MFrontBehaviourInterfaceFactory.cxx                  \
		 MFrontModelInterfaceFactory.cxx                      \
		 MFrontDefaultParser.cxx                              \
		 MFrontModelParser.cxx                                \
		 MFrontMaterialLawParser.cxx                          \
		 MFrontIsotropicMisesCreepParser.cxx                  \
		 MFrontIsotropicStrainHardeningMisesCreepParser.cxx   \
		 MFrontIsotropicMisesPlasticFlowParser.cxx            \
		 MFrontRungeKuttaParser.cxx                           \
		 MFrontImplicitParserBase.cxx                         \
		 MFrontImplicitParser.cxx                             \
		 MFrontImplicitParserII.cxx                           \
		 MFrontMultipleIsotropicMisesFlowsParser.cxx          \
		 MFrontIsotropicBehaviourParserBase.cxx               \
		 MFrontMFrontLawInterface.cxx                         \
		 MFrontCLawInterfaceBase.cxx                          \
		 MFrontCLawInterface.cxx                              \
		 MFrontCppLawInterface.cxx                            \
		 MFrontCppTestLawInterface.cxx                        \
		 MFrontExcelLawInterface.cxx                          \
		 MFrontLaTeXLawInterface.cxx                          \
		 MFrontUMATInterfaceBase.cxx                          \
		 VariableModifier.cxx                                 \
		 WordAnalyser.cxx


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

mfront_LDADD   = -L$(top_builddir)/src/Material  -lTFELMaterial   \
		 -L$(top_builddir)/src/Math      -lTFELMathParser \
		 -L$(top_builddir)/src/System    -lTFELSystem     \
		 -L$(top_builddir)/src/Utilities -lTFELUtilities  \
		 -L$(top_builddir)/src/Exception -lTFELException

if !TFEL_WIN
mfront_LDADD   += -ldl -lpthread
mfront_LDFLAGS  = -export-dynamic
endif # !TFEL_WIN

lib_LTLIBRARIES = libMTestFileGenerator.la
libMTestFileGenerator_la_SOURCES = MTestFileGeneratorBase.cxx            \
				   UMATSmallStrainMTestFileGenerator.cxx
libMTestFileGenerator_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities \
				       -L$(top_builddir)/src/Exception -lTFELException
libMTestFileGenerator_la_CPPFLAGS = -DMTestFileGenerator_EXPORTS
if TFEL_WIN
libMTestFileGenerator_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN

# UMAT Interface
if HAVE_CASTEM
lib_LTLIBRARIES += libUMATInterface.la
libUMATInterface_la_SOURCES = UMATInterfaceBase.cxx                 \
			      UMATFiniteStrain.cxx                  \
			      UMATComputeStiffnessTensor.cxx        \
			      UMATComputeThermalExpansionTensor.cxx \
			      UMATRotationMatrix.cxx                \
		              UMATOutOfBoundsPolicy.cxx             \
		              UMATGetModellingHypothesis.cxx        \
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

# Aster Interface
if HAVE_ASTER
lib_LTLIBRARIES += libAsterInterface.la
libAsterInterface_la_SOURCES = AsterInterfaceBase.cxx                 \
		               AsterException.cxx                     \
		               AsterTangentOperator.cxx               \
			       AsterRotationMatrix.cxx                \
			       AsterComputeStiffnessTensor.cxx        \
			       AsterComputeThermalExpansionTensor.cxx

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