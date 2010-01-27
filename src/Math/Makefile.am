## Makefile.am -- Process this file with automake to produce a Makefile.in file.

INCLUDES = -I@top_srcdir@/include
if HAVE_GSL
INCLUDES += @GSLINCLUDES@
endif
if HAVE_OCTAVE
INCLUDES += @OCTAVEINCLUDES@
endif

lib_LTLIBRARIES  = libTFELMath.la 
lib_LTLIBRARIES += libTFELMathParser.la
lib_LTLIBRARIES += libTFELMathInterpreter.la

libTFELMath_la_SOURCES = MathException.cxx              \
			 LUException.cxx                \
		         BasicOperations.cxx            \
		         Id.cxx                         \
		         Lambda.cxx                     \
		         Power.cxx                      \
		         OpDiff.cxx                     \
		         CompositeTag.cxx               \
		         ArrayTag.cxx                   \
		         LambdaTag.cxx                  \
		         St2toSt2Tag.cxx                \
		         VectorTag.cxx                  \
		         FunctionTag.cxx                \
		         MatrixTag.cxx                  \
		         StensorTag.cxx                 \
			 StandardFunctions.cxx          \
			 KrigingErrors.cxx

libTFELMathParser_la_SOURCES = ExternalCFunction.cxx          \
			       ExternalCastemFunction.cxx     \
			       ExternalFunctionExpr.cxx       \
			       ExternalFunctionExpr2.cxx      \
			       DifferentiatedFunctionExpr.cxx \
			       Expr.cxx	                      \
			       BinaryFunction.cxx             \
			       BinaryOperator.cxx             \
			       LogicalExpr.cxx                \
			       ConditionalExpr.cxx            \
			       ExternalFunction.cxx           \
			       EvaluatorBase.cxx              \
			       EvaluatorTExpr.cxx	      \
			       EvaluatorFunction.cxx	      \
			       Evaluator.cxx	              \
			       Function.cxx	              \
			       Negation.cxx	              \
			       Number.cxx                     \
			       Variable.cxx

libTFELMathParser_la_DEPENDENCIES = libTFELMath.la

libTFELMathInterpreter_la_SOURCES      = InterpreterCommon.cxx
libTFELMathInterpreter_la_DEPENDENCIES = libTFELMathParser.la \
				       	 libTFELMath.la

if HAVE_GSL
libTFELMathParser_la_SOURCES += EvaluatorGSLWrapper.cxx
endif

if HAVE_OCTAVE
libTFELMathParser_la_SOURCES += ExternalOctaveFunction.cxx
endif 

libTFELMath_la_LIBADD       = -L@top_builddir@/src/Exception/ \
			      -lTFELException

libTFELMathParser_la_LIBADD = -L@top_builddir@/src/Exception/ \
			      -L@top_builddir@/src/Math/      \
			      -lTFELMath -lTFELException

if HAVE_GSL
libTFELMathParser_la_LIBADD += @GSLLIBS@
endif

if HAVE_OCTAVE
libTFELMathParser_la_LIBADD += @OCTAVELIBS@
endif

libTFELMathInterpreter_la_LIBADD = -L@top_builddir@/src/Exception/  \
				   -L@top_builddir@/src/Utilities/  \
				   -L@top_builddir@/src/System/     \
				   -L@top_builddir@/src/Math/       \
				   -lTFELMathParser -lTFELMath      \
				   -lTFELSystem     -lTFELUtilities \
				   -lTFELException
