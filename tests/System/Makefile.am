if WITH_TESTS

AM_CPPFLAGS = -I$(top_srcdir)/include \
              -I@builddir@/include

mfronts = Test.mfront

testdir = $(pkgdatadir)/tests/System

LDADD = -lTFELSystem -lTFELException -lTFELTests \
	-L$(top_builddir)/src/System             \
	-L$(top_builddir)/src/Exception          \
	-L$(top_builddir)/src/Tests

if !TFEL_WIN
test_PROGRAMS   = process_test_target \
		  process             \
		  rwstream            \
		  binary_write

process_test_target_SOURCES = process_test_target.cxx
process_SOURCES             = process.cxx
rwstream_SOURCES            = rwstream.cxx
binary_write_SOURCES        = binary_write.cxx

if HAVE_CASTEM
test_PROGRAMS               += CastemParameterTest \
			       CastemSourceTest
CastemParameterTest_SOURCES  = CastemParameterTest.cxx
CastemSourceTest_SOURCES     = CastemSourceTest.cxx

check_LTLIBRARIES = libCastemMaterialLaw.la
nodist_libCastemMaterialLaw_la_SOURCES = src/Test-castem.cxx
libCastemMaterialLaw_la_LDFLAGS        = -rpath /

endif

src/%-castem.cxx : %.mfront
	@top_builddir@/mfront/src/mfront --interface=castem  $<

endif

TESTS=$(test_PROGRAMS)

endif

clean-local:
	-rm -fr src include

EXTRA_DIST = CMakeLists.txt \
	     $(mfronts)
