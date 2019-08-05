include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
  "${PROJECT_BINARY_DIR}/src/PhysicalConstants"
  "${PROJECT_BINARY_DIR}/src/UnicodeSupport"
  "${PROJECT_BINARY_DIR}/src/Exception")

set(TFELMath_SOURCES
    MathException.cxx
    LUException.cxx
    QRException.cxx
    Discretization1D.cxx
    StensorConcept.cxx
    TensorConcept.cxx
    RunTimeCheck.cxx
    StridedRandomAccessIterator.cxx
    DifferenceRandomAccessIterator.cxx
    StensorComputeEigenValues.cxx)

set(TFELMathCubicSpline_SOURCES
    CubicSpline.cxx)

set(TFELMathKriging_SOURCES
    KrigingErrors.cxx
    KrigingUtilities.cxx
    Kriging1D.cxx
    Kriging2D.cxx
    Kriging3D.cxx
    FactorizedKriging1D1D.cxx
    FactorizedKriging1D2D.cxx
    FactorizedKriging1D3D.cxx)

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
    ConstantExternalFunction.cxx
    EvaluatorBase.cxx
    EvaluatorTExpr.cxx
    EvaluatorFunction.cxx
    Evaluator.cxx
    IntegerEvaluator.cxx
    IntegerEvaluatorTExpr.cxx
    Function.cxx
    Negation.cxx
    Number.cxx
    LevenbergMarquardtEvaluatorWrapper.cxx
    LevenbergMarquardtExternalFunctionWrapper.cxx
    Variable.cxx)

tfel_library(TFELMath ${TFELMath_SOURCES})
target_link_libraries(TFELMath
                      TFELException)

tfel_library(TFELMathCubicSpline ${TFELMathCubicSpline_SOURCES})
target_link_libraries(TFELMathCubicSpline
		      TFELMath
                      TFELException)

tfel_library(TFELMathKriging ${TFELMathKriging_SOURCES})
target_link_libraries(TFELMathKriging
		      TFELMath
                      TFELException)

tfel_library(TFELMathParser ${TFELMathParser_SOURCES})
add_dependencies(TFELMathParser TFELMath)
target_link_libraries(TFELMathParser
		      TFELMathKriging
                      TFELMath
		      TFELException
		      TFELUnicodeSupport
		      TFELPhysicalConstants)
