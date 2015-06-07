## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Metaprogramming
test_PROGRAMS = HasConstIterator \
		IsConstCallable  \
		typelist2

HasConstIterator_SOURCES = HasConstIterator.cxx 
typelist2_SOURCES        = typelist2.cxx         
IsConstCallable_SOURCES  = IsConstCallable.cxx         

LDADD = -L$(top_builddir)/src/Tests \
	-L$(top_builddir)/src/Utilities \
        -lTFELUtilities -lTFELTests


TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
