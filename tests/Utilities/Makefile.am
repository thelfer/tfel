## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Utilities
test_PROGRAMS = gentype

LDADD = -lTFELUtilities -lTFELException  \
	-L$(top_builddir)/src/Utilities  \
	-L$(top_builddir)/src/Exception

gentype_SOURCES               = gentype.cxx               

TESTS=$(test_PROGRAMS)

endif


