AM_CPPFLAGS = -I$(top_builddir)/src/TFELConfig \
	      -I$(top_srcdir)/include          \
	      -I$(top_srcdir)/mfront/include   \
	      -I$(top_srcdir)/mtest/include

lib_LTLIBRARIES = libTFELMTest.la

bin_PROGRAMS  = mtest
mtest_SOURCES = MTestMain.cxx

libTFELMTest_la_SOURCES = Study.cxx                                 \
			  StudyCurrentState.cxx                     \
			  StructureCurrentState.cxx                 \
			  CurrentState.cxx                          \
			  Scheme.cxx                                \
			  SchemeBase.cxx                            \
			  SingleStructureScheme.cxx                 \
			  AnalyticalTest.cxx                        \
			  ReferenceFileComparisonTest.cxx           \
			  Evolution.cxx                             \
			  CastemEvolution.cxx                       \
			  FunctionEvolution.cxx                     \
			  Behaviour.cxx                             \
			  BehaviourWorkSpace.cxx                    \
			  UmatBehaviourBase.cxx                     \
			  UmatNormaliseTangentOperator.cxx          \
			  Constraint.cxx                            \
			  ImposedDrivingVariable.cxx                \
			  ImposedThermodynamicForce.cxx             \
			  Solver.cxx                                \
			  GenericSolver.cxx                         \
			  AccelerationAlgorithmFactory.cxx          \
			  AccelerationAlgorithm.cxx                 \
			  AlternateSecantAccelerationAlgorithm.cxx  \
			  AlternateDelta2AccelerationAlgorithm.cxx  \
			  Alternate2DeltaAccelerationAlgorithm.cxx  \
			  CrossedSecantAccelerationAlgorithm.cxx    \
			  CrossedDelta2AccelerationAlgorithm.cxx    \
			  Crossed2DeltaAccelerationAlgorithm.cxx    \
			  Crossed2DeltabisAccelerationAlgorithm.cxx \
			  SecantAccelerationAlgorithm.cxx           \
			  CastemAccelerationAlgorithm.cxx           \
			  IronsTuckAccelerationAlgorithm.cxx        \
			  SteffensenAccelerationAlgorithm.cxx       \
			  MTest.cxx                                 \
			  MTestParser.cxx                           \
			  PipeTest.cxx                              \
			  PipeLinearElement.cxx                     \
			  PipeQuadraticElement.cxx

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
libTFELMTest_la_SOURCES += CastemStandardBehaviour.cxx          \
			   CastemSmallStrainBehaviour.cxx       \
			   CastemFiniteStrainBehaviour.cxx      \
			   CastemCohesiveZoneModel.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lCastemInterface
endif

if HAVE_ASTER
libTFELMTest_la_SOURCES += AsterStandardBehaviour.cxx    \
			   AsterSmallStrainBehaviour.cxx \
			   AsterFiniteStrainBehaviour.cxx \
			   AsterCohesiveZoneModel.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lAsterInterface
endif

if HAVE_ABAQUS
libTFELMTest_la_SOURCES += AbaqusStandardBehaviour.cxx    \
			   AbaqusSmallStrainBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lAbaqusInterface
endif

if HAVE_CYRANO
libTFELMTest_la_SOURCES += CyranoBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lCyranoInterface
endif

EXTRA_DIST=CMakeLists.txt
