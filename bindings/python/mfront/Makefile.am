SUBDIRS = umatxx mtest

AM_CPPFLAGS = -I@top_srcdir@/include/                       \
	      -I@top_srcdir@/mfront/include/                \
	      -I@top_srcdir@/bindings/python/include/       \
	      @BOOST_CPPFLAGS@                              \
	      -I@PYTHONPATH@/include/python@PYTHON_VERSION@

mfrontdir = @pythondir@/mfront
mfront_PYTHON = __init__.py

mfront_LTLIBRARIES = _mfront.la
_mfront_la_SOURCES = mfront.cxx               \
		     AbstractDSL.cxx          \
		     FileDescription.cxx      \
		     LibraryDescription.cxx   \
		     TargetsDescription.cxx   \
		     DSLFactory.cxx           \
		     VariableDescription.cxx  \
		     AbstractBehaviourDSL.cxx \
		     BehaviourDescription.cxx \
		     BehaviourData.cxx

_mfront_la_LDFLAGS = -module

EXTRA_DIST=CMakeLists.txt
