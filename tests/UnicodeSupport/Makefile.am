## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/UnicodeSupport
test_PROGRAMS = UnicodeSupportTest

LDADD = -lTFELUnicodeSupport -lTFELException  \
	-lTFELTests                              \
	-L$(top_builddir)/src/UnicodeSupport     \
	-L$(top_builddir)/src/Tests              \
	-L$(top_builddir)/src/Exception

UnicodeSupportTest_SOURCES = UnicodeSupportTest.cxx

TESTS=$(test_PROGRAMS)

clean-local:
	-$(RM) *.xml

endif

EXTRA_DIST=CMakeLists.txt
