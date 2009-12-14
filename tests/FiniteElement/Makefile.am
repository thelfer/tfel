## Makefile.am -- Process this file with automake to produce a Makefile.in file.

INCLUDES = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/FiniteElement
test_PROGRAMS = NodesManager \
		 Mesh

LDADD = -lTFELMath -lTFELException      \
	-L$(top_builddir)/src/Math      \
	-L$(top_builddir)/src/Exception


NodesManager_SOURCES = NodesManager.cxx      
Mesh_SOURCES         = Mesh.cxx      

TESTS=$(test_PROGRAMS)

endif
