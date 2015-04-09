## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/St2toT2

test_PROGRAMS =	st2tot2                   \
		StensorProductDerivative  \
		StensorProductDerivative2

LDADD = -lTFELMath -lTFELException -lTFELTests \
	-L$(top_builddir)/src/Tests            \
	-L$(top_builddir)/src/Math             \
	-L$(top_builddir)/src/Exception

st2tot2_SOURCES      = st2tot2.cxx
StensorProductDerivative_SOURCES  = StensorProductDerivative.cxx
StensorProductDerivative2_SOURCES = StensorProductDerivative2.cxx


endif

EXTRA_DIST=CMakeLists.txt
