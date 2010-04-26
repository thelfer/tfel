## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Metaprogramming
test_PROGRAMS = ConceptCheck     \
		HasConstIterator \
		Name             \
		RemoveConstness  \
		typelist

ConceptCheck_SOURCES     = ConcepCheck.cxx 
HasConstIterator_SOURCES = HasConstIterator.cxx 
Name_SOURCES	         = Name.cxx	     
RemoveConstness_SOURCES  = RemoveConstness.cxx  
typelist_SOURCES         = typelist.cxx         

LDADD = -L$(top_builddir)/src/Utilities \
        -lTFELUtilities


TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt