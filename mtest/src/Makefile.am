AM_CPPFLAGS = -I$(top_builddir)/src/TFELConfig \
	      -I$(top_srcdir)/include          \
	      -I$(top_srcdir)/mfront/include   \
	      -I$(top_srcdir)/mtest/include

lib_LTLIBRARIES = libTFELMTest.la

bin_PROGRAMS  = mtest
mtest_SOURCES = MTestMain.cxx

libTFELMTest_la_SOURCES = MTest.cxx                                \
			  MTestParser.cxx                          \
			  AnalyticalTest.cxx                  \
			  ReferenceFileComparisonTest.cxx     \
			  Evolution.cxx                       \
			  CastemEvolution.cxx                 \
			  FunctionEvolution.cxx               \
			  Behaviour.cxx                       \
			  UmatBehaviourBase.cxx               \
			  UmatNormaliseTangentOperator.cxx    \
			  Constraint.cxx                      \
			  ImposedDrivingVariable.cxx          \
			  ImposedThermodynamicForce.cxx       \
			  AccelerationAlgorithmFactory.cxx    \
			  AccelerationAlgorithm.cxx           \
			  AlternateSecantAccelerationAlgorithm.cxx     \
			  AlternateDelta2AccelerationAlgorithm.cxx     \
			  Alternate2DeltaAccelerationAlgorithm.cxx     \
			  CrossedSecantAccelerationAlgorithm.cxx     \
			  CrossedDelta2AccelerationAlgorithm.cxx     \
			  Crossed2DeltaAccelerationAlgorithm.cxx     \
			  Crossed2DeltabisAccelerationAlgorithm.cxx     \
			  SecantAccelerationAlgorithm.cxx     \
			  CastemAccelerationAlgorithm.cxx     \
			  IronsTuckAccelerationAlgorithm.cxx  \
			  SteffensenAccelerationAlgorithm.cxx

libTFELMTest_la_LIBADD  = -L$(top_builddir)/mfront/src -lMFrontLogStream  \
			  -L$(top_builddir)/src/Material -lTFELMaterial   \
			  -L$(top_builddir)/src/Math                      \
			  -lTFELMathParser -lTFELMath                     \
		 	  -L$(top_builddir)/src/System    -lTFELSystem    \
			  -L$(top_builddir)/src/Utilities -lTFELUtilities \
			  -L$(top_builddir)/src/Exception -lTFELException \
			  -L$(top_builddir)/src/Tests     -lTFELTests
libTFELMTest_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELMTest_EXPORTS
if TFEL_WIN
libTFELMTest_la_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif # TFEL_WIN

mtest_LDADD = -L$(top_builddir)/mtest/src  -lTFELMTest        \
	      -L$(top_builddir)/mfront/src -lMFrontLogStream  \
	      -L$(top_builddir)/src/Utilities -lTFELUtilities \
	      -L$(top_builddir)/src/System    -lTFELSystem    \
	      -L$(top_builddir)/src/Tests     -lTFELTests

mtest_DEPENDENCIES = libTFELMTest.la

if HAVE_CASTEM
libTFELMTest_la_SOURCES += UmatStandardBehaviour.cxx          \
			   UmatSmallStrainBehaviour.cxx       \
			   UmatFiniteStrainBehaviour.cxx      \
			   UmatCohesiveZoneModel.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lUMATInterface
endif

if HAVE_ASTER
libTFELMTest_la_SOURCES += AsterStandardBehaviour.cxx    \
			   AsterSmallStrainBehaviour.cxx \
			   AsterFiniteStrainBehaviour.cxx \
			   AsterCohesiveZoneModel.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lAsterInterface
endif

if HAVE_CYRANO
libTFELMTest_la_SOURCES += CyranoBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lCyranoInterface
endif

EXTRA_DIST=CMakeLists.txt
