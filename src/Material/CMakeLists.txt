include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

set(TFELMaterial_SOURCES
    MaterialException.cxx
    ModellingHypothesis.cxx
    DDIF2Base.cxx)

tfel_library(TFELMaterial ${TFELMaterial_SOURCES})
target_link_libraries(TFELMaterial
                      TFELMath
                      TFELUtilities
                      TFELException)

tfel_library(TFELMaterialSingleCristals
  MetallicCFCSlidingSystems.cxx
  MetallicCFCGenericSlidingSystemsInteractionMatrix.cxx)
target_link_libraries(TFELMaterial
                      TFELMath
                      TFELUtilities
                      TFELException)
