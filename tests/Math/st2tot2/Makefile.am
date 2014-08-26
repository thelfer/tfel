## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/St2toT2

test_PROGRAMS =	st2tot2

LDADD = -lTFELMath -lTFELException -lTFELTests \
	-L$(top_builddir)/src/Tests            \
	-L$(top_builddir)/src/Math             \
	-L$(top_builddir)/src/Exception

st2tot2_SOURCES      = st2tot2.cxx
# st2tot2_2_SOURCES    = st2tot2-2.cxx
# st2tot2_3_SOURCES    = st2tot2-3.cxx
# St2tot2FromTinyMatrixView_SOURCES  = St2tot2FromTinyMatrixView.cxx
# St2tot2FromTinyMatrixView2_SOURCES = St2tot2FromTinyMatrixView2.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
