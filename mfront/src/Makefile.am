## Makefile.am -- Process this file with automake to produce a Makefile.in file.

INCLUDES = -I$(top_srcdir)/include        \
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
		 MFrontLaTeXLawInterface.cxx


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

mfront_LDADD   = -L$(top_builddir)/src/System    -lTFELSystem    \
		 -L$(top_builddir)/src/Utilities -lTFELUtilities \
		 -L$(top_builddir)/src/Exception -lTFELException

if !TFEL_WIN
mfront_LDADD   += -ldl -lpthread
mfront_LDFLAGS  = -export-dynamic
endif

# UMAT Interfaces

if HAVE_CASTEM
lib_LTLIBRARIES = libUMATInterface.la
libUMATInterface_la_SOURCES = UMATInterfaceBase.cxx                 \
			      UMATComputeStiffnessTensor.cxx        \
			      UMATComputeThermalExpansionTensor.cxx \
			      UMATRotationMatrix.cxx                \
		              UMATOutOfBoundsPolicy.cxx             \
		              UMATGetModellingHypothesis.cxx        \
		              UMATException.cxx
libUMATInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities \
			      -L$(top_builddir)/src/Exception -lTFELException
libUMATInterface_la_CPPFLAGS = -DUMATInterface_EXPORTS
if TFEL_WIN
libUMATInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

endif

if HAVE_ASTER
if HAVE_CASTEM
    lib_LTLIBRARIES += libAsterInterface.la
else
    lib_LTLIBRARIES  = libAsterInterface.la
endif
libAsterInterface_la_SOURCES = AsterInterfaceBase.cxx   \
		               AsterException.cxx       \
		               AsterTangentOperator.cxx
# AsterComputeStiffnessTensor.cxx        \
# AsterComputeThermalExpansionTensor.cxx \
# AsterRotationMatrix.cxx                \
# AsterOutOfBoundsPolicy.cxx             \
# AsterGetModellingHypothesis.cxx
libAsterInterface_la_LIBADD  = -L$(top_builddir)/src/Utilities -lTFELUtilities \
			      -L$(top_builddir)/src/Exception -lTFELException
libAsterInterface_la_CPPFLAGS = -DAsterInterface_EXPORTS
if TFEL_WIN
libAsterInterface_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

endif


EXTRA_DIST = CMakeLists.txt