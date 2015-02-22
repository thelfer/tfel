## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include
if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/ExpressionTemplates
test_PROGRAMS = Expr         \
		Expr2        \
		Expr3        \
		Expr4        \
		Expr5        \
		Expr6        \
		Expr7        \
		Expr8        \
		Expr9        \
		test_expr1   \
		test_expr2   \
		test_expr6   \
		test_expr7   \
		test_expr9   \
		test_expr12

LDADD = -L$(top_builddir)/src/Math                 \
	-L$(top_builddir)/src/Utilities            \
	-L$(top_builddir)/src/Exception            \
	-L$(top_builddir)/src/Tests                \
	-lTFELMath -lTFELUtilities -lTFELException \
	-lTFELTests

Expr_SOURCES         = Expr.cxx
Expr2_SOURCES        = Expr2.cxx
Expr3_SOURCES        = Expr3.cxx
Expr4_SOURCES        = Expr4.cxx
Expr5_SOURCES        = Expr5.cxx
Expr6_SOURCES        = Expr6.cxx
Expr7_SOURCES        = Expr7.cxx
Expr8_SOURCES        = Expr8.cxx
Expr9_SOURCES        = Expr9.cxx
test_expr1_SOURCES   = test_expr1.cxx    
test_expr2_SOURCES   = test_expr2.cxx   
test_expr6_SOURCES   = test_expr6.cxx   
test_expr7_SOURCES   = test_expr7.cxx   
test_expr9_SOURCES   = test_expr9.cxx   
test_expr12_SOURCES  = test_expr12.cxx  

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt