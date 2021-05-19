## Makefile.am -- Process this file with automake to produce a Makefile.in file.
SUBDIRS = include ExpressionTemplates RungeKutta  \
	  stensor tensor st2tost2 t2tost2 st2tot2 \
	  t2tot2 cadna

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math
test_PROGRAMS = IndexingPoliciesTest             \
        view                                     \
        hill                                     \
		promote                                  \
		derivative_type                          \
		derivative_view                          \
		power                                    \
		qt                                       \
		qt2                                      \
		qt3                                      \
		CubicRoots                               \
		conj                                     \
		lu                                       \
		lu2                                      \
		lu3                                      \
		invert                                   \
		invert2                                  \
		tinymatrixsolve                          \
		tinymatrixsolve2                         \
		qr                                       \
		CubicSplineTest                          \
		krigeage                                 \
		krigeage1D                               \
		krigeage2D                               \
		newton_raphson                           \
		solve                                    \
		fsarray                                  \
		runtime_array                            \
		tmatrix                                  \
		tmatrix1                                 \
		tmatrix2                                 \
		tmatrix4                                 \
		tmatrix5                                 \
		tmatrix6                                 \
		tvector                                  \
		tvector2                                 \
		tvector3                                 \
		tvector4                                 \
		tvector5                                 \
		tvector6                                 \
		tvector7                                 \
		VectorResultType                         \
		parser                                   \
		parser2                                  \
		parser3                                  \
		parser4                                  \
		parser5                                  \
		parser6                                  \
		parser7                                  \
		parser8                                  \
		parser9                                  \
		parser10                                 \
		parser11                                 \
		integerparser                            \
		broyden                                  \
		broyden2                                 \
        TinyVectorOfTinyVectorFromTinyVectorView \
		TinyVectorOfStensorFromTinyVectorView    \
		levenberg-marquardt                      \
		levenberg-marquardt2                     \
		levenberg-marquardt3                     \
		levenberg-marquardt4                     \
		discretization1D                         \
		TensorProductCheck                       \
        CorotationnalCauchyStressToSecondPioloKirchhoffStressTest \
		StridedRandomAccessIteratorTest          \
		DifferenceRandomAccessIteratorTest       \
	        IEEE754Test                          \
        ScalarNewtonRaphsonTest                  \
        BissectionAlgorithmTest                  \
        FixedSizeArrayDerivativeTest

LDADD = -L$(top_builddir)/src/Math      \
	-L$(top_builddir)/src/Utilities \
	-L$(top_builddir)/src/Exception \
        -L$(top_builddir)/src/Tests     \
	-lTFELMath -lTFELUtilities      \
	-lTFELException -lTFELTests

view_SOURCES  = view.cxx
IndexingPoliciesTest_SOURCES  = IndexingPoliciesTest.cxx
power_SOURCES                 = power.cxx
promote_SOURCES               = promote.cxx
derivative_type_SOURCES       = derivative_type.cxx
derivative_view_SOURCES       = derivative_view.cxx
qt_SOURCES                    = qt.cxx
qt2_SOURCES                   = qt2.cxx
qt3_SOURCES                   = qt3.cxx
hill_SOURCES                  = hill.cxx
CubicRoots_SOURCES            = CubicRoots.cxx
conj_SOURCES                  = conj.cxx                 
invert_SOURCES                = invert.cxx
invert2_SOURCES               = invert2.cxx
tinymatrixsolve_SOURCES       = tinymatrixsolve.cxx
tinymatrixsolve2_SOURCES      = tinymatrixsolve2.cxx
newton_raphson_SOURCES        = newton_raphson.cxx       
solve_SOURCES                 = solve.cxx                
fsarray_SOURCES               = fsarray.cxx
runtime_array_SOURCES         = runtime_array.cxx
tmatrix_SOURCES               = tmatrix.cxx
tmatrix1_SOURCES              = tmatrix1.cxx
tmatrix2_SOURCES              = tmatrix2.cxx
tmatrix4_SOURCES              = tmatrix4.cxx
tmatrix5_SOURCES              = tmatrix5.cxx
tmatrix6_SOURCES              = tmatrix6.cxx
tvector_SOURCES               = tvector.cxx
tvector2_SOURCES              = tvector2.cxx
tvector3_SOURCES              = tvector3.cxx
tvector4_SOURCES              = tvector4.cxx
tvector5_SOURCES              = tvector5.cxx
tvector6_SOURCES              = tvector6.cxx
tvector7_SOURCES              = tvector7.cxx
VectorResultType_SOURCES      = VectorResultType.cxx
broyden_SOURCES               = broyden.cxx
broyden2_SOURCES              = broyden2.cxx
TinyVectorOfTinyVectorFromTinyVectorView_SOURCES = TinyVectorOfTinyVectorFromTinyVectorView.cxx
TinyVectorOfStensorFromTinyVectorView_SOURCES    = TinyVectorOfStensorFromTinyVectorView.cxx
levenberg_marquardt_SOURCES   = levenberg-marquardt.cxx
levenberg_marquardt2_SOURCES  = levenberg-marquardt2.cxx
levenberg_marquardt3_SOURCES  = levenberg-marquardt3.cxx
TensorProductCheck_SOURCES    = TensorProductCheck.cxx
CorotationnalCauchyStressToSecondPioloKirchhoffStressTest_SOURCES = CorotationnalCauchyStressToSecondPioloKirchhoffStressTest.cxx
StridedRandomAccessIteratorTest_SOURCES    = StridedRandomAccessIteratorTest.cxx
DifferenceRandomAccessIteratorTest_SOURCES = DifferenceRandomAccessIteratorTest.cxx
IEEE754Test_SOURCES = IEEE754Test.cxx
ScalarNewtonRaphsonTest_SOURCES = ScalarNewtonRaphsonTest.cxx
BissectionAlgorithmTest_SOURCES = BissectionAlgorithmTest.cxx
FixedSizeArrayDerivativeTest_SOURCES = FixedSizeArrayDerivativeTest.cxx

discretization1D_SOURCES = discretization1D.cxx
discretization1D_LDADD   = -L$(top_builddir)/src/Math      \
		          -L$(top_builddir)/src/Utilities  \
		          -L$(top_builddir)/src/Exception  \
                          -L$(top_builddir)/src/Tests      \
		          -lTFELMath -lTFELException       \
			  -lTFELTests

CubicSplineTest_SOURCES = CubicSplineTest.cxx
CubicSplineTest_LDADD   = -L$(top_builddir)/src/Math       \
		          -L$(top_builddir)/src/Utilities  \
		          -L$(top_builddir)/src/Exception  \
                          -L$(top_builddir)/src/Tests      \
		          -lTFELMathCubicSpline -lTFELMath \
		          -lTFELUtilities -lTFELException  \
			  -lTFELTests

krigeage_SOURCES = krigeage.cxx
krigeage_LDADD   = -L$(top_builddir)/src/Math       \
		   -L$(top_builddir)/src/Utilities  \
		   -L$(top_builddir)/src/Exception  \
		   -lTFELMathKriging -lTFELMath     \
		   -lTFELUtilities -lTFELException

krigeage1D_SOURCES = krigeage1D.cxx
krigeage1D_LDADD   = -L$(top_builddir)/src/Math       \
		     -L$(top_builddir)/src/Utilities  \
		     -L$(top_builddir)/src/Exception  \
		     -lTFELMathKriging -lTFELMath     \
		     -lTFELUtilities -lTFELException

krigeage2D_SOURCES = krigeage2D.cxx
krigeage2D_LDADD   = -L$(top_builddir)/src/Math       \
		     -L$(top_builddir)/src/Utilities  \
		     -L$(top_builddir)/src/Exception  \
		     -lTFELMathKriging -lTFELMath     \
		     -lTFELUtilities -lTFELException

levenberg_marquardt4_SOURCES  = levenberg-marquardt4.cxx
levenberg_marquardt4_LDADD = -L$(top_builddir)/src/Math      \
	       -L$(top_builddir)/src/Utilities    \
	       -L$(top_builddir)/src/Exception    \
	       -lTFELMathParser -lTFELMathKriging \
	       -lTFELMath -lTFELUtilities         \
	       -lTFELException

parser_SOURCES                = parser.cxx
parser_LDADD = -L$(top_builddir)/src/Math         \
	       -L$(top_builddir)/src/Utilities    \
	       -L$(top_builddir)/src/Exception    \
	       -lTFELMathParser -lTFELMathKriging \
	       -lTFELMath -lTFELUtilities         \
	       -lTFELException

parser2_SOURCES               = parser2.cxx
parser2_LDADD = -L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath -lTFELUtilities         \
		-lTFELException

parser3_SOURCES               = parser3.cxx
parser3_LDADD = -L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath -lTFELUtilities         \
		-lTFELException

parser4_SOURCES = parser4.cxx
parser4_LDADD = -L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath -lTFELUtilities         \
		-lTFELException

parser5_SOURCES               = parser5.cxx
parser5_LDADD = -L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath -lTFELUtilities         \
		-lTFELException

parser6_SOURCES               = parser6.cxx
parser6_LDADD = -L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath -lTFELUtilities         \
		-lTFELException

lu_SOURCES    = lu.cxx
lu_LDADD      = -L$(top_builddir)/src/Tests        \
		-L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath  -lTFELUtilities        \
		-lTFELException	-lTFELTests

lu2_SOURCES   = lu2.cxx
lu2_LDADD     = -L$(top_builddir)/src/Tests        \
		-L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath  -lTFELUtilities        \
		-lTFELException	-lTFELTests

lu3_SOURCES   = lu3.cxx
lu3_LDADD     = -L$(top_builddir)/src/Tests        \
		        -L$(top_builddir)/src/Math         \
	            -L$(top_builddir)/src/Utilities    \
	            -L$(top_builddir)/src/Exception    \
	            -lTFELMathParser -lTFELMathKriging \
		        -lTFELMath  -lTFELUtilities        \
		        -lTFELException	-lTFELTests

qr_SOURCES    = qr.cxx
qr_LDADD      = -L$(top_builddir)/src/Tests        \
		-L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath  -lTFELUtilities        \
		-lTFELException	-lTFELTests

parser7_SOURCES               = parser7.cxx
parser7_LDADD = -L$(top_builddir)/src/Tests        \
		-L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath  -lTFELUtilities        \
		-lTFELException	-lTFELTests

parser8_SOURCES               = parser8.cxx
parser8_LDADD = -L$(top_builddir)/src/Tests        \
		-L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath  -lTFELUtilities        \
		-lTFELException	-lTFELTests

parser9_SOURCES               = parser9.cxx
parser9_LDADD = -L$(top_builddir)/src/Tests        \
		-L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath  -lTFELUtilities        \
		-lTFELException	-lTFELTests

parser10_SOURCES               = parser10.cxx
parser10_LDADD = -L$(top_builddir)/src/Tests       \
		-L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath  -lTFELUtilities        \
		-lTFELException	-lTFELTests

parser11_SOURCES               = parser11.cxx
parser11_LDADD = -L$(top_builddir)/src/Tests       \
		-L$(top_builddir)/src/Math         \
	        -L$(top_builddir)/src/Utilities    \
	        -L$(top_builddir)/src/Exception    \
	        -lTFELMathParser -lTFELMathKriging \
		-lTFELMath  -lTFELUtilities        \
		-lTFELException	-lTFELTests

integerparser_SOURCES = integerparser.cxx
integerparser_LDADD = -L$(top_builddir)/src/Tests        \
		      -L$(top_builddir)/src/Math         \
	              -L$(top_builddir)/src/Utilities    \
	              -L$(top_builddir)/src/Exception    \
	              -lTFELMathParser -lTFELMathKriging \
		      -lTFELMath  -lTFELUtilities        \
		      -lTFELException	-lTFELTests


TESTS=$(test_PROGRAMS)

clean-local:
	-$(RM) *.xml
	-$(RM) CubicSplineTest.out
	-$(RM) out2D.txt out2D-2.txt
	-$(RM) data.txt out.txt

endif

EXTRA_DIST=CMakeLists.txt
