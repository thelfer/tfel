## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/T2tost2

test_PROGRAMS =	t2tost2
# t2tost2-2    \
# t2tost2-3    \
# T2tost2FromTinyMatrixView  \
# T2tost2FromTinyMatrixView2

LDADD = -lTFELMath -lTFELException -lTFELTests \
	-L$(top_builddir)/src/Tests            \
	-L$(top_builddir)/src/Math             \
	-L$(top_builddir)/src/Exception

t2tost2_SOURCES      = t2tost2.cxx
# t2tost2_2_SOURCES    = t2tost2-2.cxx
# t2tost2_3_SOURCES    = t2tost2-3.cxx
# T2tost2FromTinyMatrixView_SOURCES  = T2tost2FromTinyMatrixView.cxx
# T2tost2FromTinyMatrixView2_SOURCES = T2tost2FromTinyMatrixView2.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
