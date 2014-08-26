## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/T2toT2

test_PROGRAMS =	t2tot2

LDADD = -lTFELMath -lTFELException -lTFELTests \
	-L$(top_builddir)/src/Tests            \
	-L$(top_builddir)/src/Math             \
	-L$(top_builddir)/src/Exception

t2tot2_SOURCES      = t2tot2.cxx
# t2tot2_2_SOURCES    = t2tot2-2.cxx
# t2tot2_3_SOURCES    = t2tot2-3.cxx
# T2tot2FromTinyMatrixView_SOURCES  = T2tot2FromTinyMatrixView.cxx
# T2tot2FromTinyMatrixView2_SOURCES = T2tot2FromTinyMatrixView2.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
