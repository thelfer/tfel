if WITH_TESTS

INCLUDES = -I$(top_srcdir)/include

testdir = $(pkgdatadir)/tests/Material
test_PROGRAMS = boundsCheck

LDADD = -lTFELMaterial -lTFELMath -lTFELException \
	-L$(top_builddir)/src/Material            \
	-L$(top_builddir)/src/Math                \
	-L$(top_builddir)/src/Exception

boundsCheck_SOURCES = boundsCheck.cxx               

TESTS=$(test_PROGRAMS)

endif
