set(mfront_tests_SOURCES
  MaterialPropertiesBoundsCheck
  ExternalStateVariablesBoundsCheck
  Elasticity
  Elasticity2
  Elasticity3
  JohnsonCook_s
  JohnsonCook_ssr
  JohnsonCook_ssrt
  RusinekKlepaczko_ssrt
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
  ComputeThermalExpansion
  ComputeThermalExpansion2
  ComputeThermalExpansion3
  ParameterTest
  ParameterTest2
  ElasticMaterialPropertiesBoundsCheckTest
  TemperatureUpdateOnSubSteppingCheck
  LogarithmicStrainElasticity
  LogarithmicStrainElasticity4
  LogarithmicStrainStandardElasticity
  LogarithmicStrainStandardElasticity2
  LogarithmicStrainStandardElasticity3
  LogarithmicStrainStandardElasticity4
  LogarithmicStrainOrthotropicElasticity
  LogarithmicStrainNortonCreep
  LogarithmicStrainNortonCreep2
  GenerateMTestFileOnFailure
  ProfilerTest
  ProfilerTest3)

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
  ThermalExpansionCoefficientTest_3
  YoungModulusBoundsCheckTest)

mfront_behaviour_check_library(MFrontCyranoBehaviours cyrano ${mfront_tests_SOURCES})
target_link_libraries(MFrontCyranoBehaviours
  PRIVATE CyranoInterface)

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
  set(file "${CMAKE_CURRENT_SOURCE_DIR}/cyrano${test_arg}.mtest")
  foreach(rm ${IEEE754_ROUNDING_MODES})
    set(test "cyrano${test_arg}_${rm}_mtest")
    set(_mtest_args)
    list(APPEND _mtest_args --rounding-direction-mode=${rm} )
    list(APPEND _mtest_args --verbose=level0)
    list(APPEND _mtest_args --result-file-output=false)
    list(APPEND _mtest_args --@library@="$<TARGET_FILE:MFrontCyranoBehaviours>")
    list(APPEND _mtest_args --@top_srcdir@=${PROJECT_SOURCE_DIR})
    if(_NO_XML_OUTPUT)
      list(APPEND _mtest_args --xml-output=false)
    else(_NO_XML_OUTPUT)
      list(APPEND _mtest_args --xml-output=true)
    endif(_NO_XML_OUTPUT)
    add_test(NAME ${test} COMMAND mtest ${_mtest_args} ${file})
    if(_WILL_FAIL)
      set_tests_properties(${test} PROPERTIES WILL_FAIL true)
    endif(_WILL_FAIL)
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
       set_property(TEST ${test}
                    PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  endforeach(rm ${IEEE754_ROUNDING_MODES})
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/cyrano"
      COMPONENT mtest)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront/tests/behaviours/cyrano"
      COMPONENT mtest)
  endif(TFEL_APPEND_SUFFIX)
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
  set_tests_properties(cyranomaterialpropertiesboundscheck6_${rm}_mtest
    PROPERTIES ENVIRONMENT "CYRANO_OUT_OF_BOUNDS_POLICY=NONE")
  set_tests_properties(cyranomaterialpropertiesboundscheck8_${rm}_mtest
    PROPERTIES ENVIRONMENT "CYRANO_OUT_OF_BOUNDS_POLICY=STRICT")
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
test_cyrano(parametertest)
test_cyrano(parametertest2)

test_cyrano(elasticity_LogarithmicStrain-uniaxialloading-1-1D)
test_cyrano(elasticity_LogarithmicStrain-uniaxialloading-2-1D)
test_cyrano(elasticity_LogarithmicStrain-uniaxialloading-3-1D)
test_cyrano(elasticity_LogarithmicStrain-thermalstrain)
test_cyrano(elasticity_LogarithmicStrain-thermalstrain2)
test_cyrano(norton_LogarithmicStrain-uniaxialloading-1-1D)
test_cyrano(norton_LogarithmicStrain-uniaxialloading-2-1D)
test_cyrano(norton_LogarithmicStrain-uniaxialloading-3-1D)

test_cyrano(elasticmaterialpropertiesboundschecktest
  NO_XML_OUTPUT WILL_FAIL)
test_cyrano(temperatureupdateonsubsteppingcheck)

test_cyrano(logarithmicstrainstandardelasticity)
test_cyrano(logarithmicstrainstandardelasticity2)
test_cyrano(logarithmicstrainstandardelasticity3)
test_cyrano(logarithmicstrainstandardelasticity4)
