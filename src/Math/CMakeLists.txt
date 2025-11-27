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
    PowerFunction.cxx
    Negation.cxx
    Number.cxx
    LevenbergMarquardtEvaluatorWrapper.cxx
    LevenbergMarquardtExternalFunctionWrapper.cxx
    Variable.cxx)

tfel_library(TFELMath ${TFELMath_SOURCES})
target_include_directories(TFELMath
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(TFELMath
	PUBLIC TFELException)
if(enable-static)
  target_include_directories(TFELMath-static
      PUBLIC 
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>)
    target_link_libraries(TFELMath-static
        PUBLIC TFELException-static)
endif(enable-static)

tfel_library(TFELMathCubicSpline ${TFELMathCubicSpline_SOURCES})
target_include_directories(TFELMathCubicSpline
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(TFELMathCubicSpline
   PUBLIC tfel::TFELMath
   PUBLIC tfel::TFELException)
if(enable-static)
    target_include_directories(TFELMathCubicSpline-static
      PUBLIC 
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>)
    target_link_libraries(TFELMathCubicSpline-static
        PUBLIC tfel::TFELMath-static
        PUBLIC tfel::TFELException-static)
endif(enable-static)
    
tfel_library(TFELMathKriging ${TFELMathKriging_SOURCES})
target_include_directories(TFELMathKriging
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(TFELMathKriging
    PUBLIC tfel::TFELMath
    PUBLIC tfel::TFELException)
if(enable-static)
    target_include_directories(TFELMathKriging-static
        PUBLIC 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
    target_link_libraries(TFELMathKriging-static
        PUBLIC tfel::TFELMath-static
        PUBLIC tfel::TFELException-static)
endif(enable-static)
    
tfel_library(TFELMathParser ${TFELMathParser_SOURCES})
add_dependencies(TFELMathParser TFELMath)
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  target_compile_definitions(TFELMathParser PUBLIC "TFEL_ARCH64")
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  target_compile_definitions(TFELMathParser PUBLIC "TFEL_ARCH32")
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
target_include_directories(TFELMathParser
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(TFELMathParser
	PUBLIC tfel::TFELMathKriging
        PUBLIC tfel::TFELMath
	PUBLIC tfel::TFELException
	PUBLIC tfel::TFELUnicodeSupport)
if(enable-static)
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
      target_compile_definitions(TFELMathParser-static PUBLIC "TFEL_ARCH64")
    else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
      target_compile_definitions(TFELMathParser-static PUBLIC "TFEL_ARCH32")
    endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    target_include_directories(TFELMathParser-static
      PUBLIC 
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>)
    target_link_libraries(TFELMathParser-static
        PUBLIC tfel::TFELMathKriging-static
        PUBLIC tfel::TFELMath-static
        PUBLIC tfel::TFELException-static
        PUBLIC tfel::TFELUnicodeSupport-static)
endif(enable-static)
