include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

set(TFELMaterial_SOURCES
    DDIF2Base.cxx
    BoundsCheck.cxx
    MaterialException.cxx
    ModellingHypothesis.cxx
    FiniteStrainBehaviourTangentOperator.cxx)

tfel_library(TFELMaterial ${TFELMaterial_SOURCES})
target_link_libraries(TFELMaterial
                      TFELMath
                      TFELUtilities
                      TFELException)
