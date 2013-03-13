INCLUDES= -I@top_srcdir@/include/                       \
	  @BOOST_CPPFLAGS@                              \
	  -I@PYTHONPATH@/include/python@PYTHON_VERSION@

tfeldir = @pythondir@/tfel
tfel_PYTHON = __init__.py

tfel_LTLIBRARIES = math.la      \
		   utilities.la \
		   material.la  \
		   system.la

math_la_SOURCES = math.cxx      \
		  tvector.cxx   \
		  stensor.cxx
math_la_LIBADD  = -L@top_builddir@/src/Math -lTFELMath         \
		  @BOOST_LIBS@ @BOOST_PYTHON_LIBS@             \
		  -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
math_la_LDFLAGS = -module

utilities_la_SOURCES = utilities.cxx      \
		       TextData.cxx
utilities_la_LIBADD  = -L@top_builddir@/src/Utilities -lTFELUtilities         \
		  @BOOST_LIBS@ @BOOST_PYTHON_LIBS@             \
		  -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
utilities_la_LDFLAGS = -module


material_la_SOURCES = ModellingHypothesis.cxx
material_la_LIBADD  = -L@top_builddir@/src/Material -lTFELMaterial \
		      @BOOST_LIBS@ @BOOST_PYTHON_LIBS@             \
		      -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
material_la_LDFLAGS = -module

system_la_SOURCES = system.cxx                 \
		    ProcessManagerCommand.cxx  \
		    ExternalLibraryManager.cxx
system_la_LIBADD  = -L@top_builddir@/src/System -lTFELSystem    \
		    @BOOST_LIBS@ @BOOST_PYTHON_LIBS@            \
	            -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
system_la_LDFLAGS = -module
