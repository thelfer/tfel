AM_CPPFLAGS = -I@top_srcdir@/include/                       \
	      -I@top_srcdir@/bindings/python/include/       \
	      @BOOST_CPPFLAGS@                              \
	      -I@PYTHONPATH@/include/python@PYTHON_VERSION@

stddir = @pythondir@/std

std_PYTHON   = __init__.py

std_LTLIBRARIES = _stl_vector.la \
		  _stl_map.la    \
		  _stl_pair.la   \
		  _stl_tuple.la

_stl_vector_la_SOURCES = vector.cxx
_stl_map_la_SOURCES    = map.cxx
_stl_pair_la_SOURCES   = pair.cxx
_stl_tuple_la_SOURCES  = tuple.cxx

_stl_vector_la_LIBADD  =  @BOOST_LIBS@ @BOOST_PYTHON_LIBS@            \
		                  -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
_stl_vector_la_LDFLAGS = -module

_stl_map_la_LIBADD     = @BOOST_LIBS@ @BOOST_PYTHON_LIBS@            \
		         -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
_stl_map_la_LDFLAGS    = -module

_stl_pair_la_LIBADD    = @BOOST_LIBS@ @BOOST_PYTHON_LIBS@            \
		         -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
_stl_pair_la_LDFLAGS   = -module

_stl_tuple_la_LIBADD   = @BOOST_LIBS@ @BOOST_PYTHON_LIBS@            \
		         -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
_stl_tuple_la_LDFLAGS  = -module

EXTRA_DIST=CMakeLists.txt
