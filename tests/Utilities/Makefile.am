## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Utilities
test_PROGRAMS = gentype          \
		gentype2         \
		gentype3         \
		StringAlgorithms

LDADD = -lTFELUtilities -lTFELException  \
	-lTFELTests                      \
	-L$(top_builddir)/src/Utilities  \
	-L$(top_builddir)/src/Tests      \
	-L$(top_builddir)/src/Exception

gentype_SOURCES          = gentype.cxx
gentype2_SOURCES         = gentype2.cxx
gentype3_SOURCES         = gentype3.cxx
StringAlgorithms_SOURCES = StringAlgorithms.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
