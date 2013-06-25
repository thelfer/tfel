INCLUDES = -I$(top_builddir)/src/TFELConfig \
	   -I$(top_srcdir)/include          \
	   -I$(top_srcdir)/mfront/include

lib_LTLIBRARIES = libTFELMTest.la

bin_PROGRAMS  = mtest
mtest_SOURCES = MTestMain.cxx

libTFELMTest_la_SOURCES = MTest.cxx                             \
			  MTestAnalyticalTest.cxx               \
			  MTestReferenceFileComparisonTest.cxx  \
			  MTestLogStream.cxx                    \
			  MTestEvolution.cxx                    \
			  MTestCastemEvolution.cxx              \
			  MTestFunctionEvolution.cxx            \
			  MTestBehaviour.cxx                    \
			  MTestUmatBehaviourBase.cxx            \
			  MTestUmatNormaliseTangentOperator.cxx \
			  MTestConstraint.cxx                   \
			  MTestImposedStrain.cxx                \
			  MTestImposedStress.cxx

libTFELMTest_la_LIBADD  = -L$(top_builddir)/src/Math                      \
			  -lTFELMathParser -lTFELMath                     \
		 	  -L$(top_builddir)/src/System    -lTFELSystem    \
			  -L$(top_builddir)/src/Utilities -lTFELUtilities \
			  -L$(top_builddir)/src/Exception -lTFELException \
			  -L$(top_builddir)/src/Tests     -lTFELTests
libTFELMTest_la_CPPFLAGS = -DTFELMTest_EXPORTS

mtest_LDADD = -L$(top_builddir)/mfront/mtest              \
	      -L$(top_builddir)/src/Tests     -lTFELTests \
	      -lTFELMTest

mtest_DEPENDENCIES = libTFELMTest.la

if HAVE_CASTEM
libTFELMTest_la_SOURCES += MTestUmatBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lUMATInterface
endif

if HAVE_ASTER
libTFELMTest_la_SOURCES += MTestAsterBehaviour.cxx
libTFELMTest_la_LIBADD  += -L$(top_builddir)/mfront/src -lAsterInterface
endif

EXTRA_DIST=CMakeLists.txt