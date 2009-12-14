if WITH_TESTS

INCLUDES = -I$(top_srcdir)/include

testdir = $(pkgdatadir)/tests/MaterialLaw
test_PROGRAMS = boundsCheck

LDADD = -lTFELMaterialLaw -lTFELMath -lTFELException  \
	-L$(top_builddir)/src/MaterialLaw             \
	-L$(top_builddir)/src/Math                    \
	-L$(top_builddir)/src/Exception

boundsCheck_SOURCES = boundsCheck.cxx               

TESTS=$(test_PROGRAMS)

endif
