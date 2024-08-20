set(TFELMaterial_SOURCES
    DDIF2Base.cxx
    BoundsCheck.cxx
    MaterialException.cxx
    ModellingHypothesis.cxx
    FiniteStrainBehaviourTangentOperator.cxx
    SlipSystemsDescription.cxx
    LogarithmicStrainHandler.cxx)

tfel_library(TFELMaterial ${TFELMaterial_SOURCES})
target_include_directories(TFELMaterial
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(TFELMaterial
	PUBLIC TFELNUMODIS
	PUBLIC TFELMath
	PUBLIC TFELUtilities
	PUBLIC TFELException)
if(enable-static)
    target_include_directories(TFELMaterial-static
        PUBLIC 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
    target_link_libraries(TFELMaterial-static
        PUBLIC TFELNUMODIS-static
        PUBLIC TFELMath-static
        PUBLIC TFELUtilities-static
        PUBLIC TFELException-static)
endif(enable-static)
