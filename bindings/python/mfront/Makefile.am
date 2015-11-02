SUBDIRS = umatxx

AM_CPPFLAGS = -I@top_srcdir@/include/                       \
	      -I@top_srcdir@/mfront/include/                \
	      -I@top_srcdir@/mtest/include/                 \
	      @BOOST_CPPFLAGS@                              \
	      -I@PYTHONPATH@/include/python@PYTHON_VERSION@

mfrontdir = @pythondir@/mfront
mfront_PYTHON = __init__.py

mfront_LTLIBRARIES = mtest.la

mtest_la_SOURCES =  pymtest.cxx         \
		    MTest.cxx           \
		    MTestParser.cxx     \
		    CurrentState.cxx    \
		    MFrontLogStream.cxx
mtest_la_LIBADD  = -L@top_builddir@/mtest/src                   \
		   -L@top_builddir@/mfront/src                  \
		   -lTFELMTest -lMFrontLogStream                \
		   @BOOST_LIBS@ @BOOST_PYTHON_LIBS@             \
		   -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
mtest_la_LDFLAGS = -module

EXTRA_DIST=CMakeLists.txt
