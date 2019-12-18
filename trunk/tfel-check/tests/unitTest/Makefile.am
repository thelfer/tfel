AM_CPPFLAGS= -I$(top_srcdir)/include             \
	     -I$(top_srcdir)/tfel-check/include 

check_PROGRAMS = tests
TESTS = $(check_PROGRAMS)

tests_SOURCES = tests.cxx                         \
		test_Column.cxx		          \
		test_LinearInterpolation.cxx      \
	        test_SplineLocalInterpolation.cxx \
	        test_SplineInterpolation.cxx      \
		test_Test.cxx

tests_LDADD = -L$(top_builddir)/src/Tests      -lTFELTests     \
	      -L$(top_builddir)/src/Utilities  -lTFELUtilities \
	      -L$(top_builddir)/src/Utilities  -lTFELUtilities \
	      -L$(top_builddir)/src/Math  -lTFELMathParser     \
	      -lTFELMathCubicSpline                            \
	      -L$(top_builddir)/tfel-check/src -lTFELCheck

EXTRA_DIST=CMakeLists.txt

clean-local:
	$(RM) -f *.xml *.res


