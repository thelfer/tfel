## Makefile.am -- Process this file with automake to produce a Makefile.in file.
if TFEL_WIN
AM_LDFLAGS=-no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

INCLUDES = -I@top_srcdir@/include

lib_LTLIBRARIES  = libTFELMath.la 
lib_LTLIBRARIES += libTFELMathCubicSpline.la
lib_LTLIBRARIES += libTFELMathKriging.la
lib_LTLIBRARIES += libTFELMathParser.la

libTFELMath_la_SOURCES = MathException.cxx              \
			 LUException.cxx                \
			 QRException.cxx                \
			 Discretization1D.cxx           \
		         BasicOperations.cxx            \
		         Id.cxx                         \
		         Lambda.cxx                     \
		         power.cxx                      \
		         Sqrt.cxx                       \
		         OpDiff.cxx                     \
		         CompositeTag.cxx               \
		         ArrayTag.cxx                   \
		         LambdaTag.cxx                  \
		         St2toSt2Tag.cxx                \
		         VectorTag.cxx                  \
		         FunctionTag.cxx                \
		         MatrixTag.cxx                  \
		         StensorTag.cxx                 \
			 StandardFunctions.cxx
if TFEL_WIN
libTFELMath_la_CPPFLAGS = -DTFELMath_EXPORTS
endif

libTFELMathCubicSpline_la_SOURCES = CubicSpline.cxx
if TFEL_WIN
libTFELMathCubicSpline_la_CPPFLAGS = -DTFELMathCubicSpline_EXPORTS
endif

libTFELMathKriging_la_SOURCES = KrigingErrors.cxx         \
				KrigingUtilities.cxx      \
				Kriging1D.cxx             \
				Kriging2D.cxx             \
				Kriging3D.cxx             \
				FactorizedKriging1D1D.cxx \
				FactorizedKriging1D2D.cxx \
				FactorizedKriging1D3D.cxx
if TFEL_WIN
libTFELMathKriging_la_CPPFLAGS = -DTFELMathKriging_EXPORTS
endif

libTFELMathParser_la_SOURCES = ExternalCFunction.cxx                         \
			       ExternalCastemFunction.cxx                    \
			       ExternalFunctionExpr.cxx                      \
			       ExternalFunctionExpr2.cxx                     \
			       KrigedFunction.cxx                            \
			       DifferentiatedFunctionExpr.cxx                \
			       Expr.cxx	                                     \
			       BinaryFunction.cxx                            \
			       BinaryOperator.cxx                            \
			       LogicalExpr.cxx                               \
			       ConditionalExpr.cxx                           \
			       ExternalFunction.cxx                          \
			       ConstantExternalFunction.cxx                  \
			       EvaluatorBase.cxx                             \
			       EvaluatorTExpr.cxx	                     \
			       EvaluatorFunction.cxx	                     \
			       Evaluator.cxx	                             \
			       Function.cxx	                             \
			       Negation.cxx	                             \
			       Number.cxx                                    \
			       LevenbergMarquardtEvaluatorWrapper.cxx        \
			       LevenbergMarquardtExternalFunctionWrapper.cxx \
			       Variable.cxx

if TFEL_WIN
libTFELMathParser_la_CPPFLAGS = -DTFELMathParser_EXPORTS
endif


libTFELMathCubicSpline_la_DEPENDENCIES = libTFELMath.la
libTFELMathKriging_la_DEPENDENCIES     = libTFELMath.la
libTFELMathParser_la_DEPENDENCIES      = libTFELMath.la \
				         libTFELMathKriging.la

libTFELMath_la_LIBADD       = -L@top_builddir@/src/Exception/ \
			      -lTFELException

libTFELMathParser_la_LIBADD = -L@top_builddir@/src/Exception/ \
			      -L@top_builddir@/src/Math/      \
			      -lTFELMath -lTFELException

libTFELMathCubicSpline_la_LIBADD = -L@top_builddir@/src/Exception/ \
			           -L@top_builddir@/src/Math/      \
			           -lTFELMath -lTFELException

libTFELMathKriging_la_LIBADD = -L@top_builddir@/src/Exception/ \
			       -L@top_builddir@/src/Math/      \
			       -lTFELMath -lTFELException

EXTRA_DIST = CMakeLists.txt