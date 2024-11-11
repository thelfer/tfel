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
	PUBLIC tfel::TFELNUMODIS
	PUBLIC tfel::TFELMath
	PUBLIC tfel::TFELUtilities
	PUBLIC tfel::TFELException)
if(enable-static)
    target_include_directories(TFELMaterial-static
        PUBLIC 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
    target_link_libraries(TFELMaterial-static
        PUBLIC tfel::TFELNUMODIS-static
        PUBLIC tfel::TFELMath-static
        PUBLIC tfel::TFELUtilities-static
        PUBLIC tfel::TFELException-static)
endif(enable-static)
