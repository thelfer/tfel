AM_CPPFLAGS = -I@top_srcdir@/include/                       \
	          -I@top_srcdir@/bindings/python/include/       \
	          -I@top_srcdir@/mfront/include/                \
	          -I@top_srcdir@/mtest/include/                 \
	          -I@top_srcdir@/mfm-test-generator/include/    \
	          @BOOST_CPPFLAGS@                              \
	          -I@PYTHONPATH@/include/python@PYTHON_VERSION@

mfm_test_generatordir = @pythondir@/mfm_test_generator
mfm_test_generator_LTLIBRARIES = _mfm_test_generator.la
mfm_test_generator_PYTHON = __init__.py

_mfm_test_generator_la_SOURCES = pymfm_test_generator.cxx       \
                                 MFMTestGeneratorFileExport.cxx

_mfm_test_generator_la_LIBADD  = -L@top_builddir@/mfm_test_generator/src      \
                       		    -l@top_builddir@/mtest/src                   \
                       		    -L@top_builddir@/mfront/src                  \
                      		    -L@top_builddir@/src/Utilities               \
                       		    -L@top_builddir@/src/Material                \
                       		    -lMFMTestGenerator                           \
                       		    -lTFELMTest    -lMFrontLogStream             \
                       		    -lTFELMaterial -lTFELUtilities               \
                       		    @BOOST_LIBS@ @BOOST_PYTHON_LIBS@             \
                       		    -L@PYTHONPATH@/lib -lpython@PYTHON_VERSION@
_mfm_test_generator_la_LDFLAGS = -module

EXTRA_DIST=CMakeLists.txt          \
      	   __init__.py.version.in
