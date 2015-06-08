## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/Functions
test_PROGRAMS = function        \
		power           \
		vector_function \
		derivate 

LDADD = -lTFELMath -lTFELException       \
	-L$(top_builddir)/src/Math       \
	-L$(top_builddir)/src/Exception

function_SOURCES        = function.cxx
power_SOURCES           = power.cxx
vector_function_SOURCES = vector-function.cxx
derivate_SOURCES        = derivate.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
