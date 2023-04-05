AM_CPPFLAGS = -I@top_srcdir@/include/                       \
	      -I@top_srcdir@/bindings/python/include/       \
	      @BOOST_CPPFLAGS@                              \
	      -I@PYTHONPATH@/include/python@PYTHON_VERSION@

tfeldir = @pythondir@/tfel
tfel_PYTHON = __init__.py

tfel_LTLIBRARIES = _tfel.la     \
		   tests.la     \
		   math.la      \
		   utilities.la \
		   glossary.la  \
		   material.la  \
		   system.la

if TFEL_NUMPY_SUPPORT
tfel_LTLIBRARIES += libTFELNumpySupport.la
libTFELNumpySupport_la_SOURCES  = InitNumpy.cxx \
                                  ndarray.cxx
libTFELNumpySupport_la_LIBADD   = @BOOST_LIBS@ @BOOST_PYTHON_LIBS@ @BOOST_NUMPY_LIBS@ \
                                  -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
libTFELNumpySupport_la_CPPFLAGS = $(AM_CPPFLAGS)
if TFEL_WIN
libTFELNumpySupport_la_CPPFLAGS += -DTFELNumpySupport_EXPORTS
endif
endif


_tfel_la_SOURCES = tfel.cxx              \
           Config.cxx                    \
		   PhysicalConstants.cxx
_tfel_la_LIBADD  = -L@top_builddir@/src/Config                 \
		           -lTFELConfig                                \
                    @BOOST_LIBS@ @BOOST_PYTHON_LIBS@            \
		           -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
_tfel_la_LDFLAGS = -module

tests_la_SOURCES = tests.cxx         \
		   TestResult.cxx    \
		   XMLTestOutput.cxx

tests_la_LIBADD  = -L@top_builddir@/src/Tests -lTFELTests       \
		   @BOOST_LIBS@ @BOOST_PYTHON_LIBS@             \
		   -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
tests_la_LDFLAGS = -module

math_la_SOURCES = math.cxx           \
		  vector.cxx                 \
		  tvector.cxx                \
		  stensor.cxx                \
		  st2tost2.cxx               \
		  Evaluator.cxx

if TFEL_NUMPY_SUPPORT
math_la_SOURCES  += AccelerationAlgorithms.cxx
math_la_CPPFLAGS  = $(AM_CPPFLAGS) -DTFEL_NUMPY_SUPPORT
else
math_la_CPPFLAGS  = $(AM_CPPFLAGS)
endif

math_la_LIBADD  = -L@top_builddir@/src/Math                   \
		  -lTFELMath -lTFELMathParser                 \
                  @BOOST_LIBS@ @BOOST_PYTHON_LIBS@            \
                  -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@

if TFEL_NUMPY_SUPPORT
math_la_LIBADD += -L@builddir@                \
                  -LTFELNumpySupport
endif

math_la_LDFLAGS = -module

utilities_la_SOURCES = utilities.cxx \
		       Data.cxx      \
		       TextData.cxx
utilities_la_LIBADD  = -L@top_builddir@/src/Utilities -lTFELUtilities         \
		  @BOOST_LIBS@ @BOOST_PYTHON_LIBS@             \
		  -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
utilities_la_LDFLAGS = -module

glossary_la_SOURCES = pyglossary.cxx \
		      Glossary.cxx   \
		      GlossaryEntry.cxx
glossary_la_LIBADD  = -L@top_builddir@/src/Glossary -lTFELGlossary  \
		      @BOOST_LIBS@ @BOOST_PYTHON_LIBS@              \
		      -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
glossary_la_LDFLAGS = -module

material_la_SOURCES = material.cxx            \
		      ModellingHypothesis.cxx \
		      OutOfBoundsPolicy.cxx   \
		      CrystalStructure.cxx    \
		      PiPlane.cxx             \
                      Hosford.cxx             \
		      Barlat.cxx
material_la_LIBADD  = -L@top_builddir@/src/Material -lTFELMaterial \
		      @BOOST_LIBS@ @BOOST_PYTHON_LIBS@             \
		      -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
material_la_LDFLAGS = -module

system_la_SOURCES = system.cxx                       \
		    ProcessManagerCommand.cxx                \
		    LibraryInformation.cxx                   \
		    ExternalLibraryManager.cxx               \
		    ExternalMaterialKnowledgeDescription.cxx \
		    ExternalMaterialPropertyDescription.cxx  \
		    ExternalBehaviourDescription.cxx
system_la_LIBADD  = -L@top_builddir@/src/System -lTFELSystem    \
		    @BOOST_LIBS@ @BOOST_PYTHON_LIBS@            \
	            -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
system_la_LDFLAGS = -module

EXTRA_DIST=CMakeLists.txt         \
	   __init__.py.version.in
