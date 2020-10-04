SUBDIRS = umatxx mtest

AM_CPPFLAGS = -I@top_srcdir@/include/                       \
	      -I@top_srcdir@/mfront/include/                \
	      -I@top_srcdir@/bindings/python/include/       \
	      @BOOST_CPPFLAGS@                              \
	      -I@PYTHONPATH@/include/python@PYTHON_VERSION@

mfrontdir = @pythondir@/mfront
mfront_PYTHON = __init__.py

mfront_LTLIBRARIES = _mfront.la
_mfront_la_SOURCES = mfront.cxx                \
		     AbstractDSL.cxx                   \
		     FileDescription.cxx               \
		     TargetsDescription.cxx            \
		     CompiledTargetDescriptionBase.cxx \
		     LibraryDescription.cxx            \
		     DSLFactory.cxx                    \
		     VariableBoundsDescription.cxx     \
		     VariableDescription.cxx           \
		     AbstractBehaviourDSL.cxx          \
		     BehaviourAttribute.cxx            \
		     BehaviourData.cxx                 \
		     BehaviourSymmetryType.cxx         \
		     BehaviourDescription.cxx          \
		     SearchPathsHandler.cxx            \
		     GeneratorOptions.cxx              \
		     MakefileGenerator.cxx             \
		     CMakeGenerator.cxx                \
		     OverridableImplementation.cxx

_mfront_la_LDFLAGS = -module
_mfront_la_LIBADD  = -L@top_builddir@/mfront/src                       \
		     -lTFELMFront -lMFrontLogStream                    \
	   	     -L$(top_builddir)/src/Material  -lTFELMaterial    \
		     -L$(top_builddir)/src/Math      -lTFELMathParser  \
		     -L$(top_builddir)/src/System    -lTFELSystem      \
		     -L$(top_builddir)/src/Glossary  -lTFELGlossary    \
		     -L$(top_builddir)/src/Utilities -lTFELUtilities   \
		     -L$(top_builddir)/src/Exception -lTFELException   \
		     -L$(top_builddir)/src/Config    -lTFELConfig      \
		     @BOOST_LIBS@ @BOOST_PYTHON_LIBS@                  \
		     -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@

EXTRA_DIST=CMakeLists.txt          \
	   __init__.py.version.in
