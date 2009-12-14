## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include      \
           -I$(top_srcdir)/Tests/Lambda

if WITH_TESTS

noinst_HEADERS=lambda-header.hxx

testdir = $(pkgdatadir)/tests/Lambda
test_PROGRAMS = lambda

LDADD = -lTFELMath -lTFELException       \
	-L$(top_builddir)/src/Math       \
	-L$(top_builddir)/src/Exception

lambda_SOURCES = lambda.cxx

TESTS=$(test_PROGRAMS)

endif
