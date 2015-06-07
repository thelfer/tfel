if WITH_TESTS

AM_CPPFLAGS = -I$(top_srcdir)/include

testdir = $(pkgdatadir)/tests/Material
test_PROGRAMS = boundsCheck \
		FiniteStrainBehaviourTangentOperator

LDADD = -lTFELMaterial -lTFELMath       \
	-lTFELUtilities -lTFELException \
	-lTFELTests                     \
	-L$(top_builddir)/src/Material  \
	-L$(top_builddir)/src/Math      \
	-L$(top_builddir)/src/Utilities \
	-L$(top_builddir)/src/Tests     \
	-L$(top_builddir)/src/Exception

boundsCheck_SOURCES = boundsCheck.cxx               
FiniteStrainBehaviourTangentOperator_SOURCES = FiniteStrainBehaviourTangentOperator.cxx               

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
