## Makefile.am -- Process this file with automake to produce a Makefile.in file.
if TFEL_WIN
AM_LDFLAGS=-no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

AM_CPPFLAGS = -I@top_srcdir@/include

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
		         ST2toST2Tag.cxx                \
		         VectorTag.cxx                  \
		         FunctionTag.cxx                \
		         MatrixTag.cxx                  \
		         StensorTag.cxx                 \
		         TensorTag.cxx                  \
		         TensorConcept.cxx              \
			 StandardFunctions.cxx
libTFELMath_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELMath_EXPORTS

libTFELMathCubicSpline_la_SOURCES = CubicSpline.cxx
libTFELMathCubicSpline_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELMathCubicSpline_EXPORTS

libTFELMathKriging_la_SOURCES = KrigingErrors.cxx         \
				KrigingUtilities.cxx      \
				Kriging1D.cxx             \
				Kriging2D.cxx             \
				Kriging3D.cxx             \
				FactorizedKriging1D1D.cxx \
				FactorizedKriging1D2D.cxx \
				FactorizedKriging1D3D.cxx
libTFELMathKriging_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELMathKriging_EXPORTS

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
			       Variable.cxx                                  \
			       IntegerEvaluator.cxx                          \
			       IntegerEvaluatorTExpr.cxx
libTFELMathParser_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELMathParser_EXPORTS

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
