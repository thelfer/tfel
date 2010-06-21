## Makefile.am -- Process this file with automake to produce a Makefile.in file.

INCLUDES = -I$(top_srcdir)/include        \
	   -I$(top_srcdir)/mfront/include

if HAVE_CASTEM
INCLUDES += -I$(CASTEM_ROOT)/include $(CASTEM_CPPFLAGS)
endif

bin_PROGRAMS = mfront
mfront_SOURCES = main.cxx                                             \
		 MFront.cxx                                           \
		 MFrontLock.cxx                                       \
		 MFrontInitInterfaces.cxx                             \
		 MFrontInitParsers.cxx                                \
		 MFrontParserFactory.cxx                              \
		 BehaviourCharacteristic.cxx                          \
		 BoundsDescription.cxx                                \
		 VarHandler.cxx                                       \
		 StaticVarHandler.cxx                                 \
		 ComputedVarHandler.cxx                               \
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
		 UnaryLoadingDefinition.cxx                           \
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
		 MFrontImplicitParser.cxx                             \
		 MFrontMultipleIsotropicMisesFlowsParser.cxx          \
		 MFrontIsotropicBehaviourParserBase.cxx               \
		 MFrontMFrontLawInterface.cxx                         \
		 MFrontCLawInterfaceBase.cxx                          \
		 MFrontCLawInterface.cxx                              \
		 MFrontCppLawInterface.cxx                            \
		 MFrontCppTestLawInterface.cxx                        \
		 MFrontExcelLawInterface.cxx                          \
		 MFrontOctaveLawInterface.cxx


if HAVE_GNUPLOT
mfront_SOURCES += MFrontGnuplotLawInterface.cxx
endif

if HAVE_FORTRAN
mfront_SOURCES += MFrontFortranLawInterface.cxx
endif

if HAVE_PYTHON
mfront_SOURCES += MFrontPythonLawInterface.cxx
endif

if SUPPORT_PLEIADES
mfront_SOURCES += MFrontPleiadesLawInterface.cxx
mfront_SOURCES += MFrontCelaenoModelInterface.cxx
mfront_SOURCES += MFrontGerminalModelInterface.cxx
mfront_SOURCES += MFrontCelaenoModelInterface_v1.3.cxx
mfront_SOURCES += MFrontPleiadesModelInterfaceBase.cxx
mfront_SOURCES += MFrontPleiadesModelParser.cxx
mfront_SOURCES += MFrontPleiadesModelParserArchitecture1.4.cxx
endif

if HAVE_CASTEM
mfront_SOURCES += MFrontUMATInterface.cxx      \
		  MFrontCastemLawInterface.cxx
endif


mfront_LDFLAGS = -export-dynamic
mfront_LDADD   = -L$(top_builddir)/src/System    -lTFELSystem    \
		 -L$(top_builddir)/src/Utilities -lTFELUtilities \
		 -L$(top_builddir)/src/Exception -lTFELException \
		 -ldl -lpthread

# UMAT Interfaces

if HAVE_CASTEM
lib_LTLIBRARIES = libUMATInterface.la
libUMATInterface_la_SOURCES = UMATComputeStiffnessTensor.cxx        \
			      UMATComputeThermalExpansionTensor.cxx \
			      UMATRotationMatrix.cxx                \
		              UMATOutOfBoundsPolicy.cxx             \
		              UMATException.cxx
libUMATInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities/ -lTFELUtilities
endif

EXTRA_DIST = CMakeLists.txt