## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Utilities
test_PROGRAMS = gentype        \
		gentype2       \
		tuple          \
		Name           \
		Name2          \
		IntegersSet    \
		MultipleDeque  \
		MultipleVector

LDADD = -lTFELUtilities -lTFELException  \
	-L$(top_builddir)/src/Utilities  \
	-L$(top_builddir)/src/Exception

gentype_SOURCES        = gentype.cxx
gentype2_SOURCES       = gentype2.cxx
tuple_SOURCES          = tuple.cxx
Name_SOURCES	       = Name.cxx
Name2_SOURCES	       = Name2.cxx
MultipleDeque_SOURCES  = MultipleDeque.cxx
MultipleVector_SOURCES = MultipleVector.cxx
IntegersSet_SOURCES    = IntegersSet.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt