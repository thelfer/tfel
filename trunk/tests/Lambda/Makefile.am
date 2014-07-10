## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Lambda
test_PROGRAMS = lambda

LDADD = -lTFELMath -lTFELException       \
	-L$(top_builddir)/src/Math       \
	-L$(top_builddir)/src/Exception

lambda_SOURCES = lambda.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt