## Makefile.am -- Process this file with automake to produce a Makefile.in file.
SUBDIRS = include ExpressionTemplates RungeKutta Functions St2toSt2 Composite
INCLUDES = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math
test_PROGRAMS = hill                  \
		array2                \
		array3                \
		array4                \
		array                 \
		complex_operations    \
		conj                  \
		Function              \
		lu                    \
		lu2                   \
		tinymatrixsolve       \
		tinymatrixsolve2      \
		krigeage              \
		krigeage1D            \
		krigeage2D            \
		newton_raphson        \
		solve                 \
		stensor2              \
		stensor3              \
		stensor4              \
		stensor               \
		stensor_eigenvalues   \
		stensor_eigenvectors2 \
		stensor_eigenvectors  \
		tmatrix               \
		tmatrix1              \
		tmatrix2              \
		tvector               \
		tvector2              \
		tvector3              \
		VectorResultType      \
		vector1               \
		parser                \
		parser2               \
		parser3               \
		broyden               \
		broyden2              \
		Range                 \
		SFTMCV                \
		levenberg-marquardt   \
		levenberg-marquardt2  \
		levenberg-marquardt3

LDADD = -L$(top_builddir)/src/Math                 \
	-L$(top_builddir)/src/Utilities            \
	-L$(top_builddir)/src/Exception            \
	-lTFELMath -lTFELUtilities -lTFELException

hill_SOURCES                  = hill.cxx               
array2_SOURCES                = array2.cxx               
array3_SOURCES                = array3.cxx               
array4_SOURCES                = array4.cxx               
array_SOURCES                 = array.cxx                
complex_operations_SOURCES    = complex_operations.cxx   
conj_SOURCES                  = conj.cxx                 
Function_SOURCES              = Function.cxx             
lu_SOURCES                    = lu.cxx
lu2_SOURCES                   = lu2.cxx
tinymatrixsolve_SOURCES       = tinymatrixsolve.cxx
tinymatrixsolve2_SOURCES      = tinymatrixsolve2.cxx
krigeage_SOURCES              = krigeage.cxx
krigeage1D_SOURCES            = krigeage1D.cxx
krigeage2D_SOURCES            = krigeage2D.cxx
newton_raphson_SOURCES        = newton_raphson.cxx       
solve_SOURCES                 = solve.cxx                
stensor2_SOURCES              = stensor2.cxx
stensor3_SOURCES              = stensor3.cxx
stensor4_SOURCES              = stensor4.cxx
stensor_SOURCES               = stensor.cxx              
stensor_eigenvalues_SOURCES   = stensor_eigenvalues.cxx  
stensor_eigenvectors2_SOURCES = stensor_eigenvectors2.cxx
stensor_eigenvectors_SOURCES  = stensor_eigenvectors.cxx 
tmatrix_SOURCES               = tmatrix.cxx
tmatrix1_SOURCES              = tmatrix1.cxx
tmatrix2_SOURCES              = tmatrix2.cxx
tvector_SOURCES               = tvector.cxx
tvector2_SOURCES              = tvector2.cxx
tvector3_SOURCES              = tvector3.cxx
vector1_SOURCES               = vector1.cxx   
VectorResultType_SOURCES      = VectorResultType.cxx
Range_SOURCES                 = Range.cxx
broyden_SOURCES               = broyden.cxx
broyden2_SOURCES              = broyden2.cxx
SFTMCV_SOURCES                = SFTMCV.cxx
levenberg_marquardt_SOURCES   = levenberg-marquardt.cxx
levenberg_marquardt2_SOURCES  = levenberg-marquardt2.cxx
levenberg_marquardt3_SOURCES  = levenberg-marquardt3.cxx

parser_SOURCES                = parser.cxx
parser_LDADD = -L$(top_builddir)/src/Math      \
	       -L$(top_builddir)/src/Utilities \
	       -L$(top_builddir)/src/Exception \
	       -lTFELMathParser -lTFELMath     \
	       -lTFELUtilities -lTFELException
if HAVE_OCTAVE
parser_LDADD += @OCTAVELIBS@
endif

parser2_SOURCES               = parser2.cxx
parser2_LDADD = -L$(top_builddir)/src/Math      \
	        -L$(top_builddir)/src/Utilities \
	        -L$(top_builddir)/src/Exception \
	        -lTFELMathParser -lTFELMath     \
	        -lTFELUtilities -lTFELException
if HAVE_OCTAVE
parser2_LDADD += @OCTAVELIBS@
endif

parser3_SOURCES               = parser3.cxx
parser3_LDADD = -L$(top_builddir)/src/Math      \
	        -L$(top_builddir)/src/Utilities \
	        -L$(top_builddir)/src/Exception \
	        -lTFELMathParser -lTFELMath     \
	        -lTFELUtilities -lTFELException
if HAVE_OCTAVE
parser3_LDADD += @OCTAVELIBS@
endif

TESTS=$(test_PROGRAMS)

endif
