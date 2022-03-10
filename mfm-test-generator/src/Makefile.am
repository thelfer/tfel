AM_CPPFLAGS = -I$(top_srcdir)/include                    \
              -I$(top_srcdir)/mtest/include              \
              -I$(top_srcdir)/mfront/include             \
              -I$(top_srcdir)/mfm-test-generator/include

bin_PROGRAMS = mfm-test-generator
lib_LTLIBRARIES = libMFMTestGenerator.la

libMFMTestGenerator_la_SOURCES  = \
   Log.cxx                        \
   BehaviourData.cxx              \
   AbstractTestCase.cxx           \
   TestCaseBase.cxx               \
   AbstractTestCaseFactory.cxx    \
   InputFileGeneratorFactory.cxx  \
   TestCaseParameters.cxx         \
   UniaxialTensileTest.cxx        \
   ClosedPipeTest.cxx             \
   MTestInputFileGenerators.cxx   \
   LicosInputFileGenerators.cxx   \
   MFMTestGeneratorFileExport.cxx

libMFMTestGenerator_la_CPPFLAGS = $(AM_CPPFLAGS) -DMFMTestGenerator_EXPORTS
if TFEL_WIN
libMFMTestGenerator_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN
libMFMTestGenerator_la_LIBADD = \
    -L$(top_builddir)/mtest/src          -lTFELMTest            \
    -L$(top_builddir)/mfront/src         -lMFrontLogStream      \
    -L$(top_builddir)/src/Material       -lTFELMaterial         \
    -L$(top_builddir)/src/System         -lTFELSystem           \
    -L$(top_builddir)/src/Utilities      -lTFELUtilities

mfm_test_generator_SOURCES  = mfm-test-generator.cxx
mfm_test_generator_LDADD    =                                   \
    -L$(top_builddir)/mfm-test-generator/src -lMFMTestGenerator \
    -L$(top_builddir)/mfront/src             -lMFrontLogStream  \
    -L$(top_builddir)/src/System             -lTFELSystem       \
    -L$(top_builddir)/src/Utilities          -lTFELUtilities

mfm_test_generator_DEPENDENCIES = libMFMTestGenerator.la

EXTRA_DIST=CMakeLists.txt
