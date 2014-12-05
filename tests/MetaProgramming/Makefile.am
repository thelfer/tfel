## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Metaprogramming
test_PROGRAMS = HasConstIterator \
		typelist         \
		typelist2

HasConstIterator_SOURCES = HasConstIterator.cxx 
typelist_SOURCES         = typelist.cxx         
typelist2_SOURCES        = typelist2.cxx         

LDADD = -L$(top_builddir)/src/Utilities \
        -lTFELUtilities


TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt