## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/Composite
test_PROGRAMS = composite

LDADD = -L$(top_builddir)/src/Math                 \
	-L$(top_builddir)/src/Utilities            \
	-L$(top_builddir)/src/Exception            \
	-lTFELMath -lTFELUtilities -lTFELException

composite_SOURCES  = composite.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
