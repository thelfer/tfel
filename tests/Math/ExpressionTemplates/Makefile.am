## Makefile.am -- Process this file with automake to produce a Makefile.in file.

SUBDIRS=include

INCLUDES = -I$(top_srcdir)/include \
	   -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/ExpressionTemplates
test_PROGRAMS = test1   \
		test2   \
		test3   \
		test4   \
		test5   \
		test6   \
		test7   \
		test8   \
		test9   \
		test10  \
		test11  \
		test12  \
		tvector \
		tvector2


LDADD = -L$(top_builddir)/src/Math                 \
	-L$(top_builddir)/src/Utilities            \
	-L$(top_builddir)/src/Exception            \
	-lTFELMath -lTFELUtilities -lTFELException

test1_SOURCES   = test1.cxx    
test2_SOURCES   = test2.cxx   
test3_SOURCES   = test3.cxx   
test4_SOURCES   = test4.cxx   
test5_SOURCES   = test5.cxx   
test6_SOURCES   = test6.cxx   
test7_SOURCES   = test7.cxx   
test8_SOURCES   = test8.cxx   
test9_SOURCES   = test9.cxx   
test10_SOURCES  = test10.cxx
test11_SOURCES  = test11.cxx  
test12_SOURCES  = test12.cxx  
tvector_SOURCES = tvector.cxx 
tvector2_SOURCES= tvector2.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt