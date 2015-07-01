## Makefile.am -- Process this file with automake to produce a Makefile.in file.

AM_CPPFLAGS = -I$(top_srcdir)/include        \
	     -I$(top_srcdir)/mfront/include

if WITH_TESTS

testdir = $(pkgdatadir)/mfront/tests/unit-tests
test_PROGRAMS = BehaviourDataTest          \
		BehaviourDescriptionTest   \
		BehaviourBrickProviderTest \
		MFrontTest

MFrontTest_SOURCES                 = MFrontTest.cxx
BehaviourDataTest_SOURCES          = BehaviourDataTest.cxx
BehaviourDescriptionTest_SOURCES   = BehaviourDescriptionTest.cxx
BehaviourBrickProviderTest_SOURCES = BehaviourBrickProviderTest.cxx

LDADD = -L$(top_builddir)/mfront/src    \
	-L$(top_builddir)/src/Material  \
        -L$(top_builddir)/src/Math      \
	-L$(top_builddir)/src/System    \
	-L$(top_builddir)/src/Glossary  \
	-L$(top_builddir)/src/Utilities \
	-L$(top_builddir)/src/Exception \
        -L$(top_builddir)/src/Tests     \
	-lTFELMFront -lTFELMaterial     \
	-lTFELMath -lTFELSystem         \
	-lTFELUtilities -lTFELGlossary  \
	-lTFELException -lTFELTests

EXTRA_DIST=CMakeLists.txt      \
	   mfront/test1.mfront 

endif
