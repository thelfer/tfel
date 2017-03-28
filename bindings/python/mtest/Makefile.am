AM_CPPFLAGS = -I@top_srcdir@/include/                       \
	      -I@top_srcdir@/bindings/python/include/       \
	      -I@top_srcdir@/mfront/include/                \
	      -I@top_srcdir@/mtest/include/                 \
	      @BOOST_CPPFLAGS@                              \
	      -I@PYTHONPATH@/include/python@PYTHON_VERSION@

mtestdir = @pythondir@/mtest
mtest_LTLIBRARIES = _mtest.la
mtest_PYTHON = __init__.py

_mtest_la_SOURCES =  pymtest.cxx                \
		     Behaviour.cxx              \
		     SchemeBase.cxx             \
		     SingleStructureScheme.cxx  \
		     MTest.cxx                  \
		     PipeMesh.cxx               \
		     PipeTest.cxx               \
		     MTestParser.cxx            \
		     CurrentState.cxx           \
		     StructureCurrentState.cxx  \
		     StudyCurrentState.cxx      \
		     SolverWorkSpace.cxx        \
		     MFrontLogStream.cxx

_mtest_la_LIBADD  = -L@top_builddir@/mtest/src                   \
		    -L@top_builddir@/mfront/src                  \
		    -L@top_builddir@/src/Utilities               \
		    -L@top_builddir@/src/Material                \
		    -lTFELMTest    -lMFrontLogStream             \
		    -lTFELMaterial -lTFELUtilities               \
		    @BOOST_LIBS@ @BOOST_PYTHON_LIBS@             \
		    -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
_mtest_la_LDFLAGS = -module

EXTRA_DIST=CMakeLists.txt
