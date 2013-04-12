INCLUDES = -I$(top_srcdir)/include        \
	   -I$(top_srcdir)/mfront/include

bin_PROGRAMS  = mtest
mtest_SOURCES = mtest.cxx	           \
		MTestEvolution.cxx         \
		MTestBehaviour.cxx         \
		MTestUmatBehaviourBase.cxx \
		MTestConstraint.cxx        \
		MTestImposedStrain.cxx     \
		MTestImposedStress.cxx

mtest_LDADD   = -L$(top_builddir)/src/Math      -lTFELMath    \
		-L$(top_builddir)/src/System    -lTFELSystem    \
		-L$(top_builddir)/src/Utilities -lTFELUtilities \
		-L$(top_builddir)/src/Exception -lTFELException

if HAVE_CASTEM
mtest_SOURCES += MTestUmatBehaviour.cxx
mtest_LDADD   += -L$(top_builddir)/mfront/src -lUMATInterface
endif

if HAVE_ASTER
mtest_SOURCES += MTestAsterBehaviour.cxx
mtest_LDADD   += -L$(top_builddir)/mfront/src -lAsterInterface
endif

EXTRA_DIST=CMakeLists.txt