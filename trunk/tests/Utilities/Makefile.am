## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Utilities
test_PROGRAMS = gentype                                \
		gentype2                               \
		gentype3                               \
		CxxTokenizerTest                       \
		CxxTokenizerTest2                      \
		CxxTokenizerOffsetTest                 \
		CxxTokenizerKeepCommentBoundariesTest  \
		DataTest                               \
		StringAlgorithms                       \
                FCString

LDADD = -lTFELUtilities -lTFELException  \
	-lTFELTests                      \
	-L$(top_builddir)/src/Utilities  \
	-L$(top_builddir)/src/Tests      \
	-L$(top_builddir)/src/Exception

gentype_SOURCES                = gentype.cxx
gentype2_SOURCES               = gentype2.cxx
gentype3_SOURCES               = gentype3.cxx
CxxTokenizerTest_SOURCES       = CxxTokenizerTest.cxx
CxxTokenizerTest2_SOURCES      = CxxTokenizerTest2.cxx
CxxTokenizerOffsetTest_SOURCES = CxxTokenizerOffsetTest.cxx
CxxTokenizerKeepCommentBoundariesTest_SOURCES = CxxTokenizerKeepCommentBoundariesTest.cxx
DataTest_SOURCES               = DataTest.cxx
StringAlgorithms_SOURCES       = StringAlgorithms.cxx
FCString_SOURCES               = FCString.cxx

TESTS=$(test_PROGRAMS)

clean-local:
	-$(RM) *.xml

endif

EXTRA_DIST=CMakeLists.txt
