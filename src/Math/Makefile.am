## Makefile.am -- Process this file with automake to produce a Makefile.in file.

INCLUDES = -I@top_srcdir@/include
if HAVE_GSL
INCLUDES += @GSLINCLUDES@
endif
if HAVE_OCTAVE
INCLUDES += @OCTAVEINCLUDES@
endif

lib_LTLIBRARIES    = libTFELMath.la 
libTFELMath_la_SOURCES = MathException.cxx              \
			 LUException.cxx                \
		         BasicOperations.cxx            \
		         Id.cxx                         \
		         Lambda.cxx                     \
		         Power.cxx                      \
		         OpDiff.cxx                     \
		         StandardFunctions.cxx          \
		         CompositeTag.cxx               \
		         ArrayTag.cxx                   \
		         LambdaTag.cxx                  \
		         St2toSt2Tag.cxx                \
		         VectorTag.cxx                  \
		         FunctionTag.cxx                \
		         MatrixTag.cxx                  \
		         StensorTag.cxx                 \
		         BinaryFunction.cxx             \
		         BinaryOperator.cxx             \
		         LogicalExpr.cxx                \
		         ConditionalExpr.cxx            \
		         ExternalFunction.cxx           \
		         EvaluatorBase.cxx              \
		         EvaluatorTExpr.cxx	        \
		         EvaluatorFunction.cxx	        \
		         Evaluator.cxx	                \
		         ExternalCFunction.cxx          \
		         ExternalCastemFunction.cxx     \
		         ExternalFunctionExpr.cxx       \
		         ExternalFunctionExpr2.cxx      \
		         DifferentiatedFunctionExpr.cxx \
		         Expr.cxx	                \
		         Function.cxx	                \
		         Negation.cxx	                \
		         Number.cxx                     \
		         Variable.cxx                   \
			 KrigingErrors.cxx

libTFELMath_la_LIBADD   = -L@top_builddir@/src/Exception/ \
			 -lTFELException

if HAVE_GSL
libTFELMath_la_SOURCES += EvaluatorGSLWrapper.cxx
libTFELMath_la_LIBADD  += @GSLLIBS@
endif

if HAVE_OCTAVE
libTFELMath_la_SOURCES += ExternalOctaveFunction.cxx
libTFELMath_la_LIBADD  += @OCTAVELIBS@
endif 
