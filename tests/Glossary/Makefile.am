## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Glossary
test_PROGRAMS = GlossaryTest

LDADD = -lTFELGlossary -lTFELException -lTFELTests \
	-L$(top_builddir)/src/Glossary           \
	-L$(top_builddir)/src/Tests              \
	-L$(top_builddir)/src/Exception

GlossaryTest_SOURCES = GlossaryTest.cxx

TESTS=$(test_PROGRAMS)

clean-local:
	-$(RM) *.xml

endif

EXTRA_DIST=CMakeLists.txt
