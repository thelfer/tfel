## Makefile.am -- Process this file with automake to produce a Makefile.in file.
SUBDIRS = include
INCLUDES = -I$(top_srcdir)/include \
	   -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/UnaryTests
test_PROGRAMS = FracSimplify        \
		name                \
		promote             \
		stensor             \
		tmatrix             \
		tvector             \
		tuple

LDADD = -lTFELMath -lTFELUtilities -lTFELException \
	-L$(top_builddir)/src/Math                 \
	-L$(top_builddir)/src/Utilities            \
	-L$(top_builddir)/src/Exception

FracSimplify_SOURCES        = FracSimplify.cxx
name_SOURCES                = name.cxx
promote_SOURCES             = promote.cxx
stensor_SOURCES             = stensor.cxx
tmatrix_SOURCES             = tmatrix.cxx
tvector_SOURCES             = tvector.cxx
tuple_SOURCES               = tuple.cxx
gentype_SOURCES             = gentype.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt