include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

if(HAVE_GSL)
include_directories(${GSL_INCLUDES})
endif(HAVE_GSL)

if(HAVE_OCTAVE)
include_directories(${OCTAVE_INCLUDES})
endif(HAVE_OCTAVE)

set(TFELMath_SOURCES
    MathException.cxx
    LUException.cxx
    BasicOperations.cxx
    Id.cxx
    Lambda.cxx
    Power.cxx
    OpDiff.cxx
    CompositeTag.cxx
    ArrayTag.cxx
    LambdaTag.cxx
    St2toSt2Tag.cxx
    VectorTag.cxx
    FunctionTag.cxx
    MatrixTag.cxx
    StensorTag.cxx
    StandardFunctions.cxx
    KrigingErrors.cxx)

set(TFELMathParser_SOURCES
    ExternalCFunction.cxx
    ExternalCastemFunction.cxx
    ExternalFunctionExpr.cxx
    ExternalFunctionExpr2.cxx
    KrigedFunction.cxx
    DifferentiatedFunctionExpr.cxx
    Expr.cxx
    BinaryFunction.cxx
    BinaryOperator.cxx
    LogicalExpr.cxx
    ConditionalExpr.cxx
    ExternalFunction.cxx
    EvaluatorBase.cxx
    EvaluatorTExpr.cxx
    EvaluatorFunction.cxx
    Evaluator.cxx
    Function.cxx
    Negation.cxx
    Number.cxx
    LevenbergMarquardtEvaluatorWrapper.cxx
    LevenbergMarquardtExternalFunctionWrapper.cxx
    Variable.cxx)

# if HAVE_GSL
# libTFELMathParser_la_SOURCES += EvaluatorGSLWrapper.cxx
# endif
# 
# if HAVE_OCTAVE
# libTFELMathParser_la_SOURCES += ExternalOctaveFunction.cxx
# endif 

# if HAVE_GSL
# libTFELMathParser_la_LIBADD += @GSLLIBS@
# endif

# if HAVE_OCTAVE
# libTFELMathParser_la_LIBADD += @OCTAVELIBS@
# endif

set(TFELMathInterpreter_SOURCES
    InterpreterCommon.cxx)

tfel_library(TFELMath ${TFELMath_SOURCES})
target_link_libraries(TFELMath
                      TFELException)

tfel_library(TFELMathParser ${TFELMathParser_SOURCES})
add_dependencies(TFELMathParser TFELMath)
target_link_libraries(TFELMathParser
                      TFELMath TFELException)

tfel_library(TFELMathInterpreter ${TFELMathInterpreter_SOURCES})
add_dependencies(TFELMathInterpreter
                 TFELMathParser TFELMath)
target_link_libraries(TFELMathInterpreter
                      TFELMathParser TFELMath TFELException)

add_executable(tfel_eval tfel-eval.cxx)
set_target_properties(tfel_eval PROPERTIES LINK_FLAGS " -rdynamic -Wl,--export-dynamic")
add_dependencies(tfel_eval
                 TFELMathInterpreter
		 TFELMathParser
		 TFELMath)
target_link_libraries(tfel_eval
                      TFELMathInterpreter
                      TFELMathParser
		      TFELMath
                      TFELSystem
		      TFELUtilities
		      TFELException
		      -ldl)

# if HAVE_GSL
# tfel_eval_LDADD += @GSLLIBS@
# endif

# if HAVE_OCTAVE
# tfel_eval_LDADD += @OCTAVELIBS@
# endif

# if HAVE_READLINE
# tfel_eval_LDADD += @READLINE_LIBS@
# endif
