## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/St2toSt2

test_PROGRAMS =	st2tost2      \
		st2tost2-2    \
		st2tost2-3    \
		ST2toST2FTMV  \
		ST2toST2FTMV2

LDADD = -lTFELMath -lTFELException -lTFELTests \
	-L$(top_builddir)/src/Tests            \
	-L$(top_builddir)/src/Math             \
	-L$(top_builddir)/src/Exception

st2tost2_SOURCES      = st2tost2.cxx
st2tost2_2_SOURCES    = st2tost2-2.cxx
st2tost2_3_SOURCES    = st2tost2-3.cxx
ST2toST2FTMV_SOURCES  = ST2toST2FTMV.cxx
ST2toST2FTMV2_SOURCES = ST2toST2FTMV2.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
