include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

set(mfront_tests_SOURCES
  MaterialPropertiesBoundsCheck
  ExternalStateVariablesBoundsCheck
  Elasticity
  Elasticity2
  Elasticity3
  OrthotropicElastic
  OrthotropicElastic2
  OrthotropicElastic5
  OrthotropicElastic6
  Norton
  ImplicitNorton
  ImplicitNorton3
  OrthotropicCreep
  EllipticCreep
  ComputeStressFreeExpansion
  IsotropicLinearSwelling
  IsotropicVolumeSwelling
  OrthotropicSwelling
  OrthotropicSwelling2
  AxialGrowth
  AxialGrowth2
  AxialGrowth3
  )
mfront_dependencies(MFrontCyranoBehaviours
  YoungModulusTest_1
  YoungModulusTest_2
  YoungModulusTest_3
  PoissonRatioTest_12
  PoissonRatioTest_23
  PoissonRatioTest_13
  ShearModulusTest_12
  ShearModulusTest_23
  ShearModulusTest_13
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)

mfront_behaviour_check_library(MFrontCyranoBehaviours cyrano ${mfront_tests_SOURCES})
target_link_libraries(MFrontCyranoBehaviours
  CyranoInterface
  MFrontProfiling
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontCyranoBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

macro(test_cyrano test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "test_cyrano: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  foreach(rm ${IEEE754_ROUNDING_MODES})
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(conf ${CMAKE_CONFIGURATION_TYPES})
	set(test "cyrano${test_arg}_${rm}_${conf}_mtest")
	set(file "cyrano${test_arg}-${rm}-${conf}.mtest")
	get_property(MFrontCyranoBehavioursBuildPath TARGET MFrontCyranoBehaviours PROPERTY LOCATION_${conf})
	configure_file(cyrano${test_arg}.mtest.in
	  ${file} @ONLY)
	if(_NO_XML_OUTPUT)
	  add_test(NAME ${test}
	    CONFIGURATIONS ${conf}
	    COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=false --result-file-output=false ${file})
	else(_NO_XML_OUTPUT)
	  add_test(NAME ${test}
	    CONFIGURATIONS ${conf}
	    COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false ${file})
	endif(_NO_XML_OUTPUT)
	if(_WILL_FAIL)
	  set_tests_properties(${test} PROPERTIES WILL_FAIL true)
	endif(_WILL_FAIL)
	if((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS MFrontCyranoBehaviours mtest
	    PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
	elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test} PROPERTY DEPENDS MFrontCyranBehaviours mtest)
	endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	if(TFEL_APPEND_VERSION)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront-${TFEL_VERSION}/tests/behaviours/cyrano"
	    COMPONENT mtest)
	else(TFEL_APPEND_VERSION)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront/tests/behaviours/cyrano"
	    COMPONENT mtest)
	endif(TFEL_APPEND_VERSION)
      endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
    else(CMAKE_CONFIGURATION_TYPES)
      set(test "cyrano${test_arg}_${rm}_mtest")
      set(file "cyrano${test_arg}-${rm}.mtest")
      get_property(MFrontCyranoBehavioursBuildPath TARGET MFrontCyranoBehaviours PROPERTY LOCATION_${conf})
      configure_file(cyrano${test_arg}.mtest.in ${file} @ONLY)
      if(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=false --result-file-output=false ${file})
      else(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false ${file})
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(${test} PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      set_tests_properties(${test} PROPERTIES DEPENDS MFrontCyranoBehaviours)
      set_tests_properties(${test}
	PROPERTIES DEPENDS mtest)
      if(TFEL_APPEND_VERSION)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_VERSION}/tests/behaviours/cyrano"
	  COMPONENT mtest)
      else(TFEL_APPEND_VERSION)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/cyrano"
	  COMPONENT mtest)
      endif(TFEL_APPEND_VERSION)
    endif(CMAKE_CONFIGURATION_TYPES)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endmacro(test_cyrano)

test_cyrano(materialpropertiesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_cyrano(materialpropertiesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_cyrano(materialpropertiesboundscheck3)
test_cyrano(materialpropertiesboundscheck4)
test_cyrano(materialpropertiesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_cyrano(materialpropertiesboundscheck6)
test_cyrano(materialpropertiesboundscheck7)
test_cyrano(materialpropertiesboundscheck8 
  NO_XML_OUTPUT WILL_FAIL)
foreach(rm ${IEEE754_ROUNDING_MODES})
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set_tests_properties(cyranomaterialpropertiesboundscheck6_${rm}_${conf}_mtest
	PROPERTIES ENVIRONMENT "CYRANO_OUT_OF_BOUNDS_POLICY=NONE")
      set_tests_properties(cyranomaterialpropertiesboundscheck8_${rm}_${conf}_mtest
	PROPERTIES ENVIRONMENT "CYRANO_OUT_OF_BOUNDS_POLICY=STRICT")
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(cyranomaterialpropertiesboundscheck6_${rm}_mtest
      PROPERTIES ENVIRONMENT "CYRANO_OUT_OF_BOUNDS_POLICY=NONE")
    set_tests_properties(cyranomaterialpropertiesboundscheck8_${rm}_mtest
      PROPERTIES ENVIRONMENT "CYRANO_OUT_OF_BOUNDS_POLICY=STRICT")
  endif(CMAKE_CONFIGURATION_TYPES)
endforeach(rm ${IEEE754_ROUNDING_MODES})

test_cyrano(externalstatevariablesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_cyrano(externalstatevariablesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_cyrano(externalstatevariablesboundscheck3)
test_cyrano(externalstatevariablesboundscheck4)
test_cyrano(externalstatevariablesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)

test_cyrano(elasticity)
test_cyrano(elasticity10)
test_cyrano(elasticity18)
test_cyrano(elasticity19)
test_cyrano(elasticity20)
test_cyrano(elasticity21)
test_cyrano(orthotropicelasticity5-1)
test_cyrano(orthotropicelasticity5-2)
test_cyrano(orthotropicelasticity5-3)
test_cyrano(orthotropicelasticity6-1)
test_cyrano(orthotropicelasticity6-2)
test_cyrano(orthotropicelasticity6-3)
test_cyrano(norton)
test_cyrano(norton2)
test_cyrano(orthotropiccreep)
test_cyrano(ellipticcreep)
test_cyrano(implicitnorton)
test_cyrano(implicitnorton2)
test_cyrano(implicitnorton3)
test_cyrano(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-1-1D)
test_cyrano(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-2-1D)
test_cyrano(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-3-1D)
test_cyrano(elasticity_LogarithmicStrain1DWrapper-thermalstrain)
test_cyrano(elasticity_LogarithmicStrain1DWrapper-thermalstrain2)
test_cyrano(norton_LogarithmicStrain1DWrapper-uniaxialloading-1-1D)
test_cyrano(norton_LogarithmicStrain1DWrapper-uniaxialloading-2-1D)
test_cyrano(norton_LogarithmicStrain1DWrapper-uniaxialloading-3-1D)
test_cyrano(computestressfreeexpansion)
test_cyrano(isotropiclinearswelling)
test_cyrano(isotropicvolumeswelling)
test_cyrano(axialgrowth)
test_cyrano(axialgrowth3)
test_cyrano(axialgrowth4)
test_cyrano(orthotropicswelling)
test_cyrano(orthotropicswelling2)
