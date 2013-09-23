INCLUDES= -I@top_srcdir@/include/                       \
	  -I@top_srcdir@/mfront/include/                \
	  @BOOST_CPPFLAGS@                              \
	  -I@PYTHONPATH@/include/python@PYTHON_VERSION@

mfrontdir = @pythondir@/mfront
mfront_PYTHON = __init__.py

mfront_LTLIBRARIES = mtest.la

mtest_la_SOURCES =  pymtest.cxx        \
		   MTest.cxx          \
		   MTestParser.cxx    \
		   MTestLogStream.cxx
mtest_la_LIBADD  = -L@top_builddir@/mfront/mtest                \
		  -lTFELMTest                                  \
		  @BOOST_LIBS@ @BOOST_PYTHON_LIBS@             \
		  -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
mtest_la_LDFLAGS = -module

EXTRA_DIST=CMakeLists.txt