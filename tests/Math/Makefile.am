## Makefile.am -- Process this file with automake to produce a Makefile.in file.
SUBDIRS = include ExpressionTemplates RungeKutta Functions St2toSt2 Composite
INCLUDES = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math
test_PROGRAMS = hill                  \
		promote               \
		FracSimplify          \
		qt                    \
		qt2                   \
		qt3                   \
		CubicRoots            \
		array                 \
		array2                \
		array2                \
		array3                \
		array4                \
		complex_operations    \
		conj                  \
		Function              \
		lu                    \
		lu2                   \
		tinymatrixsolve       \
		tinymatrixsolve2      \
		CubicSplineTest       \
		krigeage              \
		krigeage1D            \
		krigeage2D            \
		newton_raphson        \
		solve                 \
		stensor2              \
		stensor3              \
		stensor4              \
		stensor5              \
		stensor               \
		stensor_eigenvalues   \
		stensor_eigenvectors2 \
		stensor_eigenvectors  \
		tmatrix               \
		tmatrix1              \
		tmatrix2              \
		tmatrix3              \
		tmatrix4              \
		tmatrix5              \
		tvector               \
		tvector2              \
		tvector3              \
		tvector4              \
		tvector5              \
		VectorResultType      \
		vector1               \
		parser                \
		parser2               \
		parser3               \
		parser4               \
		parser5               \
		parser6               \
		parser7               \
		parser8               \
		parser9               \
		broyden               \
		broyden2              \
		Range                 \
		SFTMCV                \
		levenberg-marquardt   \
		levenberg-marquardt2  \
		levenberg-marquardt3  \
		levenberg-marquardt4  \
		discretization1D

LDADD = -L$(top_builddir)/src/Math      \
	-L$(top_builddir)/src/Utilities \
	-L$(top_builddir)/src/Exception \
        -L$(top_builddir)/src/Tests     \
	-lTFELMath -lTFELUtilities      \
	-lTFELException -lTFELTests

FracSimplify_SOURCES          = FracSimplify.cxx
promote_SOURCES               = promote.cxx
qt_SOURCES                    = qt.cxx
qt2_SOURCES                   = qt2.cxx
qt3_SOURCES                   = qt3.cxx
hill_SOURCES                  = hill.cxx
CubicRoots_SOURCES            = CubicRoots.cxx
array2_SOURCES                = array2.cxx               
array3_SOURCES                = array3.cxx               
array4_SOURCES                = array4.cxx               
array_SOURCES                 = array.cxx                
complex_operations_SOURCES    = complex_operations.cxx   
conj_SOURCES                  = conj.cxx                 
Function_SOURCES              = Function.cxx             
tinymatrixsolve_SOURCES       = tinymatrixsolve.cxx
tinymatrixsolve2_SOURCES      = tinymatrixsolve2.cxx
newton_raphson_SOURCES        = newton_raphson.cxx       
solve_SOURCES                 = solve.cxx                
stensor2_SOURCES              = stensor2.cxx
stensor3_SOURCES              = stensor3.cxx
stensor4_SOURCES              = stensor4.cxx
stensor5_SOURCES              = stensor5.cxx
stensor_SOURCES               = stensor.cxx              
stensor_eigenvalues_SOURCES   = stensor_eigenvalues.cxx  
stensor_eigenvectors2_SOURCES = stensor_eigenvectors2.cxx
stensor_eigenvectors_SOURCES  = stensor_eigenvectors.cxx 
tmatrix_SOURCES               = tmatrix.cxx
tmatrix1_SOURCES              = tmatrix1.cxx
tmatrix2_SOURCES              = tmatrix2.cxx
tmatrix3_SOURCES              = tmatrix3.cxx
tmatrix4_SOURCES              = tmatrix4.cxx
tmatrix5_SOURCES              = tmatrix5.cxx
tvector_SOURCES               = tvector.cxx
tvector2_SOURCES              = tvector2.cxx
tvector3_SOURCES              = tvector3.cxx
tvector4_SOURCES              = tvector4.cxx
tvector5_SOURCES              = tvector5.cxx
vector1_SOURCES               = vector1.cxx   
VectorResultType_SOURCES      = VectorResultType.cxx
Range_SOURCES                 = Range.cxx
broyden_SOURCES               = broyden.cxx
broyden2_SOURCES              = broyden2.cxx
SFTMCV_SOURCES                = SFTMCV.cxx
levenberg_marquardt_SOURCES   = levenberg-marquardt.cxx
levenberg_marquardt2_SOURCES  = levenberg-marquardt2.cxx
levenberg_marquardt3_SOURCES  = levenberg-marquardt3.cxx

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

parser4_SOURCES               = parser4.cxx
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

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt