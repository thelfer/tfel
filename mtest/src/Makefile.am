AM_CPPFLAGS = -I$(top_srcdir)/include    \
	      -I$(top_srcdir)/mfront/include \
	      -I$(top_srcdir)/mtest/include

lib_LTLIBRARIES = libTFELMTest.la

bin_PROGRAMS  = mtest
mtest_SOURCES = MTestMain.cxx

libTFELMTest_la_SOURCES =                               \
              MTestFileExport.cxx                       \
              MaterialProperty.cxx                      \
              MaterialPropertyBase.cxx                  \
              GenericMaterialProperty.cxx               \
              CastemMaterialProperty.cxx                \
              CyranoMaterialProperty.cxx                \
              RoundingMode.cxx                          \
			  Study.cxx                                 \
			  StudyCurrentState.cxx                     \
			  StructureCurrentState.cxx                 \
			  CurrentState.cxx                          \
			  Scheme.cxx                                \
			  SchemeBase.cxx                            \
			  SingleStructureScheme.cxx                 \
			  AnalyticalTest.cxx                        \
			  ReferenceFileComparisonTest.cxx           \
			  Evolution.cxx                             \
			  GenericEvolution.cxx                       \
			  CastemEvolution.cxx                       \
			  CyranoEvolution.cxx                       \
			  FunctionEvolution.cxx                     \
			  Behaviour.cxx                             \
			  BehaviourWrapperBase.cxx                  \
			  LogarithmicStrain1DBehaviourWrapper.cxx   \
			  SmallStrainTridimensionalBehaviourWrapper.cxx \
			  BehaviourWorkSpace.cxx                    \
			  GenericBehaviour.cxx                      \
			  StandardBehaviourBase.cxx                 \
			  UmatNormaliseTangentOperator.cxx          \
			  Constraint.cxx                            \
			  ConstraintBase.cxx                        \
			  NonLinearConstraint.cxx                   \
			  ImposedGradient.cxx                       \
			  ImposedThermodynamicForce.cxx             \
			  Solver.cxx                                \
		 	  SolverOptions.cxx                         \
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
			  UAndersonAccelerationAlgorithm.cxx        \
			  FAndersonAccelerationAlgorithm.cxx        \
			  UserDefinedPostProcessing.cxx             \
			  MTest.cxx                                 \
			  PipeTest.cxx                              \
			  PipeLinearElement.cxx                     \
			  PipeQuadraticElement.cxx                  \
			  PipeCubicElement.cxx                      \
			  PipeProfile.cxx                           \
			  PipeProfileHandler.cxx                    \
			  PipeFailureCriterion.hxx                  \
			  PipeFailureCriteriaFactory.hxx            \
			  TextDataUtilities.cxx                     \
			  SchemeParserBase.cxx                      \
			  SingleStructureSchemeParser.cxx           \
			  MTestParser.cxx                           \
			  PipeTestParser.cxx                        \
			  GasEquationOfState.cxx

libTFELMTest_la_LIBADD  = -L$(top_builddir)/mfront/src -lMFrontLogStream  \
			  -L$(top_builddir)/src/Material -lTFELMaterial   \
			  -L$(top_builddir)/src/Math                      \
			  -lTFELMathParser -lTFELMath                     \
		 	  -L$(top_builddir)/src/System    -lTFELSystem    \
			  -L$(top_builddir)/src/Utilities -lTFELUtilities \
			  -L$(top_builddir)/src/Exception -lTFELException \
			  -L$(top_builddir)/src/Tests     -lTFELTests     \
			  -L$(top_builddir)/src/Config     -lTFELConfig
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
libTFELMTest_la_SOURCES += CastemStandardBehaviour.cxx         \
			   CastemSmallStrainBehaviour.cxx      \
			   CastemFiniteStrainBehaviour.cxx     \
			   CastemFiniteStrainBehaviour2.cxx    \
			   CastemCohesiveZoneModel.cxx         \
			   CastemModel.cxx                     \
			   CastemUmatStandardBehaviour.cxx     \
			   CastemUmatSmallStrainBehaviour.cxx  \
			   CastemUmatFiniteStrainBehaviour.cxx \
			   MistralBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lCastemInterface
endif

if HAVE_ASTER
libTFELMTest_la_SOURCES += AsterStandardBehaviour.cxx     \
			   AsterSmallStrainBehaviour.cxx              \
			   AsterFiniteStrainBehaviour.cxx             \
			   AsterCohesiveZoneModel.cxx                 \
			   AsterLogarithmicStrainBehaviourWrapper.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lAsterInterface
endif

if HAVE_EUROPLEXUS
libTFELMTest_la_SOURCES += EuroplexusStandardBehaviour.cxx    \
			   EuroplexusFiniteStrainBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lEuroplexusInterface
endif

if HAVE_ABAQUS
libTFELMTest_la_SOURCES += AbaqusStandardBehaviour.cxx     \
			   AbaqusSmallStrainBehaviour.cxx  \
			   AbaqusFiniteStrainBehaviour.cxx \
			   AbaqusExplicitBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lAbaqusInterface
endif

if HAVE_CALCULIX
libTFELMTest_la_SOURCES += CalculiXStandardBehaviour.cxx     \
			   CalculiXSmallStrainBehaviour.cxx  \
			   CalculiXFiniteStrainBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lCalculiXInterface
endif

if HAVE_ANSYS
libTFELMTest_la_SOURCES += AnsysStandardBehaviour.cxx        \
			   AnsysSmallStrainBehaviour.cxx     \
			   AnsysFiniteStrainBehaviour.cxx    \
			   AnsysNormaliseTangentOperator.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lAnsysInterface
endif

if HAVE_CYRANO
libTFELMTest_la_SOURCES += CyranoBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lCyranoInterface
endif

if HAVE_DIANAFEA
libTFELMTest_la_SOURCES += DianaFEASmallStrainBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lDianaFEAInterface
endif

EXTRA_DIST=CMakeLists.txt
