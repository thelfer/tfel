## Makefile.am -- Process this file with automake to produce a Makefile.in file.
SUBDIRS = include
INCLUDES = -I$(top_srcdir)/include \
	   -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/UnaryTests
test_PROGRAMS = CubicRoots          \
		FracSimplify        \
		IntegersSet         \
		MultipleDeque       \
		MultipleVector      \
		concept_check_test  \
		name                \
		promote             \
		qt                  \
		qt2                 \
		qt3                 \
		stensor             \
		terminal            \
		tmatrix             \
		tvector             \
		tvector2            \
		typelist            \
		tuple               \
		gentype

LDADD = -lTFELMath -lTFELUtilities -lTFELException \
	-L$(top_builddir)/src/Math                 \
	-L$(top_builddir)/src/Utilities            \
	-L$(top_builddir)/src/Exception

CubicRoots_SOURCES          = CubicRoots.cxx
FracSimplify_SOURCES        = FracSimplify.cxx
IntegersSet_SOURCES         = IntegersSet.cxx
MultipleDeque_SOURCES       = MultipleDeque.cxx
MultipleVector_SOURCES      = MultipleVector.cxx
concept_check_test_SOURCES  = concept_check_test.cxx
name_SOURCES                = name.cxx
promote_SOURCES             = promote.cxx
qt_SOURCES                  = qt.cxx
qt2_SOURCES                 = qt2.cxx
qt3_SOURCES                 = qt3.cxx
stensor_SOURCES             = stensor.cxx
terminal_SOURCES            = terminal.cxx
tmatrix_SOURCES             = tmatrix.cxx
tvector_SOURCES             = tvector.cxx
tvector2_SOURCES            = tvector2.cxx
typelist_SOURCES            = typelist.cxx
tuple_SOURCES               = tuple.cxx
gentype_SOURCES             = gentype.cxx

TESTS=$(test_PROGRAMS)

endif
