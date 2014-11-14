INCLUDES = -I$(top_builddir)/src/TFELConfig \
	   -I$(top_srcdir)/include          \
	   -I$(top_srcdir)/mfront/include

lib_LTLIBRARIES = libTFELMTest.la

bin_PROGRAMS  = mtest
mtest_SOURCES = MTestMain.cxx

libTFELMTest_la_SOURCES = MTest.cxx                                \
			  MTestParser.cxx                          \
			  MTestAnalyticalTest.cxx                  \
			  MTestReferenceFileComparisonTest.cxx     \
			  MTestEvolution.cxx                       \
			  MTestCastemEvolution.cxx                 \
			  MTestFunctionEvolution.cxx               \
			  MTestBehaviour.cxx                       \
			  MTestUmatBehaviourBase.cxx               \
			  MTestUmatNormaliseTangentOperator.cxx    \
			  MTestConstraint.cxx                      \
			  MTestImposedDrivingVariable.cxx          \
			  MTestImposedThermodynamicForce.cxx       \
			  MTestAccelerationAlgorithmFactory.cxx    \
			  MTestAccelerationAlgorithm.cxx           \
			  MTestSecantAccelerationAlgorithm.cxx     \
			  MTestCastemAccelerationAlgorithm.cxx     \
			  MTestIronsTuckAccelerationAlgorithm.cxx  \
			  MTestSteffensenAccelerationAlgorithm.cxx

libTFELMTest_la_LIBADD  = -L$(top_builddir)/mfront/src -lMFrontLogStream  \
			  -L$(top_builddir)/src/Material -lTFELMaterial   \
			  -L$(top_builddir)/src/Math                      \
			  -lTFELMathParser -lTFELMath                     \
		 	  -L$(top_builddir)/src/System    -lTFELSystem    \
			  -L$(top_builddir)/src/Utilities -lTFELUtilities \
			  -L$(top_builddir)/src/Exception -lTFELException \
			  -L$(top_builddir)/src/Tests     -lTFELTests
libTFELMTest_la_CPPFLAGS = -DTFELMTest_EXPORTS
if TFEL_WIN
libTFELMTest_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN

mtest_LDADD = -L$(top_builddir)/mfront/mtest                 \
	      -L$(top_builddir)/mfront/src -lMFrontLogStream \
	      -L$(top_builddir)/src/System    -lTFELSystem   \
	      -L$(top_builddir)/src/Tests     -lTFELTests    \
	      -lTFELMTest

mtest_DEPENDENCIES = libTFELMTest.la

if HAVE_CASTEM
libTFELMTest_la_SOURCES += MTestUmatStandardBehaviour.cxx          \
			   MTestUmatSmallStrainBehaviour.cxx       \
			   MTestUmatFiniteStrainBehaviour.cxx      \
			   MTestUmatCohesiveZoneModel.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lUMATInterface
endif

if HAVE_ASTER
libTFELMTest_la_SOURCES += MTestAsterStandardBehaviour.cxx    \
			   MTestAsterSmallStrainBehaviour.cxx \
			   MTestAsterFiniteStrainBehaviour.cxx \
			   MTestAsterCohesiveZoneModel.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lAsterInterface
endif

if HAVE_CYRANO
libTFELMTest_la_SOURCES += MTestCyranoBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lCyranoInterface
endif

EXTRA_DIST=CMakeLists.txt
