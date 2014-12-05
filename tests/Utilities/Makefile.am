## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Utilities
test_PROGRAMS = gentype          \
		gentype2         \
		gentype3         \
		Name             \
		Name2            \
		IntegersSet      \
		StringAlgorithms

LDADD = -lTFELUtilities -lTFELException  \
	-lTFELTests                      \
	-L$(top_builddir)/src/Utilities  \
	-L$(top_builddir)/src/Tests      \
	-L$(top_builddir)/src/Exception

gentype_SOURCES          = gentype.cxx
gentype2_SOURCES         = gentype2.cxx
gentype3_SOURCES         = gentype3.cxx
Name_SOURCES	         = Name.cxx
Name2_SOURCES	         = Name2.cxx
IntegersSet_SOURCES      = IntegersSet.cxx
StringAlgorithms_SOURCES = StringAlgorithms.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt