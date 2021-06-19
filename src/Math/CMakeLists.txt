set(TFELMath_SOURCES
    MathException.cxx
    LUException.cxx
    QRException.cxx
    Discretization1D.cxx
    StensorConcept.cxx
    TensorConcept.cxx
    RunTimeCheck.cxx
    StridedRandomAccessIterator.cxx
    DifferenceRandomAccessIterator.cxx)

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
    ExternalCyranoFunction.cxx
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
target_include_directories(TFELMath
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(TFELMath
	PUBLIC TFELException)

tfel_library(TFELMathCubicSpline ${TFELMathCubicSpline_SOURCES})
target_include_directories(TFELMath
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(TFELMathCubicSpline
	PUBLIC TFELMath
    PUBLIC TFELException)

tfel_library(TFELMathKriging ${TFELMathKriging_SOURCES})
target_include_directories(TFELMath
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(TFELMathKriging
    PUBLIC TFELMath
    PUBLIC TFELException)

tfel_library(TFELMathParser ${TFELMathParser_SOURCES})
add_dependencies(TFELMathParser TFELMath)
target_include_directories(TFELMath
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(TFELMathParser
	PUBLIC TFELMathKriging
    PUBLIC TFELMath
	PUBLIC TFELException
	PUBLIC TFELUnicodeSupport)
