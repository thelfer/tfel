## Makefile.am -- Process this file with automake to produce a Makefile.in file.

SUBDIRS=include

INCLUDES = -I$(top_srcdir)/include \
	   -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/ExpressionTemplates
test_PROGRAMS = test_expr1   \
		test_expr2   \
		test_expr4   \
		test_expr5   \
		test_expr6   \
		test_expr7   \
		test_expr8   \
		test_expr9   \
		test_expr10  \
		test_expr11  \
		test_expr12  \
		test_expr_tvector \
		test_expr_tvector2


LDADD = -L$(top_builddir)/src/Math                 \
	-L$(top_builddir)/src/Utilities            \
	-L$(top_builddir)/src/Exception            \
	-L$(top_builddir)/src/Tests                \
	-lTFELMath -lTFELUtilities -lTFELException \
	-lTFELTests

test_expr1_SOURCES   = test_expr1.cxx    
test_expr2_SOURCES   = test_expr2.cxx   
test_expr4_SOURCES   = test_expr4.cxx   
test_expr5_SOURCES   = test_expr5.cxx   
test_expr6_SOURCES   = test_expr6.cxx   
test_expr7_SOURCES   = test_expr7.cxx   
test_expr8_SOURCES   = test_expr8.cxx   
test_expr9_SOURCES   = test_expr9.cxx   
test_expr10_SOURCES  = test_expr10.cxx
test_expr11_SOURCES  = test_expr11.cxx  
test_expr12_SOURCES  = test_expr12.cxx  
test_expr_tvector_SOURCES = test_expr_tvector.cxx 
test_expr_tvector2_SOURCES= test_expr_tvector2.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt