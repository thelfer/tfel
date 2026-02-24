set(mfront_tests_SOURCES
  InitializationFailureTest
  MaterialPropertiesBoundsCheck
  Elasticity
  OrthotropicElastic
  HosfordPerfectPlasticity
  JohnsonCook_s
  JohnsonCook_ssr
  JohnsonCook_ssrt
  RusinekKlepaczko_ssrt
  SaintVenantKirchhoffElasticity
  SaintVenantKirchhoffElasticity2
  SaintVenantKirchhoffElasticity3
  OrthotropicSaintVenantKirchhoffElasticity
  LogarithmicStrainElasticity
  LogarithmicStrainElasticity2
  LogarithmicStrainElasticity3
  LogarithmicStrainNortonCreep
  ComputeThermalExpansion
  ComputeThermalExpansion2
  ComputeThermalExpansion3
  ParameterTest
  ParameterTest2)
mfront_dependencies(MFrontAnsysBehaviours
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)

mfront_behaviour_check_library(MFrontAnsysBehaviours  ansys ${mfront_tests_SOURCES})
target_link_libraries(MFrontAnsysBehaviours
  PRIVATE AnsysInterface)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontAnsysBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

function(test_ansys test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  set(_EXPECT_ADDITIONAL_ENVIRONMENT )
  set(_ADDITIONAL_ENVIRONMENT )
  set(_EXPECT_REFERENCE_FILE)
  set(_REFERENCE_FILE )
  foreach(_ARG ${ARGN})
    if(_EXPECT_ADDITIONAL_ENVIRONMENT)
      if(_ADDITIONAL_ENVIRONMENT)
	message(FATAL_ERROR "test_ansys: environment already set")
      endif(_ADDITIONAL_ENVIRONMENT)
      set(_ADDITIONAL_ENVIRONMENT "${_ARG}")
      set(_EXPECT_ADDITIONAL_ENVIRONMENT )
    elseif(_EXPECT_REFERENCE_FILE)
      if(_REFERENCE_FILE)
	message(FATAL_ERROR "test_ansys: reference file already set")
      endif(_REFERENCE_FILE)
      set(_REFERENCE_FILE "${_ARG}")
      set(_EXPECT_REFERENCE_FILE )
    else()
      if ( ${_ARG} MATCHES NO_XML_OUTPUT )
	set ( _NO_XML_OUTPUT ON)
      elseif ( ${_ARG} MATCHES WILL_FAIL)
	set ( _WILL_FAIL ON)
      elseif ( ${_ARG} MATCHES ENVIRONMENT)
	set(_EXPECT_ADDITIONAL_ENVIRONMENT ON)
      elseif ( ${_ARG} MATCHES REFERENCE_FILE)
	set(_EXPECT_REFERENCE_FILE ON)
      else()
	message(FATAL_ERROR "test_ansys: unsupported option '${_ARG}'")
      endif()
    endif()
  endforeach(_ARG ${ARGN})
  if(_EXPECT_ADDITIONAL_ENVIRONMENT)
    message(FATAL_ERROR "test_ansys: expected environment variable")
  endif()
  if(_EXPECT_REFERENCE_FILE)
    message(FATAL_ERROR "test_ansys: expected reference file")
  endif()
  if(_REFERENCE_FILE)
    set(_REFERENCE_FILE "${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/${_REFERENCE_FILE}")
  endif(_REFERENCE_FILE)
  foreach(rm ${IEEE754_ROUNDING_MODES})
    set(test "ansys${test_arg}_${rm}_mtest")
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/ansys${test_arg}.mtest")
    set(_mtest_args)
    list(APPEND _mtest_args --rounding-direction-mode=${rm} )
    list(APPEND _mtest_args --verbose=level0)
    list(APPEND _mtest_args --result-file-output=false)
    list(APPEND _mtest_args --@interface@=ansys)
    list(APPEND _mtest_args --@library@="$<TARGET_FILE:MFrontAnsysBehaviours>")
    list(APPEND _mtest_args --@top_srcdir@=${PROJECT_SOURCE_DIR})
    if(HAVE_CASTEM)
      list(APPEND _mtest_args --@mplibrary@="$<TARGET_FILE:MFrontMaterialProperties-castem>")
    endif(HAVE_CASTEM)
    list(APPEND _mtest_args --@reference_file@="${_REFERENCE_FILE}")
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
         PROPERTY ENVIRONMENT ${_ADDITIONAL_ENVIRONMENT}
        "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
    else((CMAKE_HOST_WIN32) AND (NOT MSYS))
      if(_ADDITIONAL_ENVIRONMENT)
        set_property(TEST ${test} PROPERTY ENVIRONMENT ${_ADDITIONAL_ENVIRONMENT})
      endif(_ADDITIONAL_ENVIRONMENT)
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
        DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/ansys"
        COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
        DESTINATION "share/doc/mfront/tests/behaviours/ansys"
        COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endfunction(test_ansys)

test_ansys(initializationfailuretest
  NO_XML_OUTPUT WILL_FAIL)
test_ansys(materialpropertiesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_ansys(materialpropertiesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_ansys(materialpropertiesboundscheck3)
test_ansys(materialpropertiesboundscheck4)
test_ansys(materialpropertiesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_ansys(materialpropertiesboundscheck6)

test_ansys(elasticity)
test_ansys(elasticity2)
if(HAVE_CASTEM)
  test_ansys(elasticity3)
endif(HAVE_CASTEM)
test_ansys(elasticity4)
test_ansys(elasticity7)
test_ansys(elasticity15)
test_ansys(elasticity16)

test_ansys(elasticity10)
test_ansys(elasticity11)
test_ansys(elasticity12)
test_ansys(elasticity13)
test_ansys(elasticity14)
test_ansys(elasticity24)
test_ansys(elasticity25)
test_ansys(elasticity26)
test_ansys(elasticity27)
test_ansys(elasticity33)
test_ansys(elasticity5)
test_ansys(elasticity6)
test_ansys(elasticity8)
test_ansys(elasticity9)

test_ansys(saintvenantkirchhoffelasticity-shear-1-2D)
test_ansys(saintvenantkirchhoffelasticity-shear-1-3D)
test_ansys(saintvenantkirchhoffelasticity-shear-2-2D)
test_ansys(saintvenantkirchhoffelasticity-shear-2-3D)
test_ansys(saintvenantkirchhoffelasticity-shear-3-3D)
test_ansys(saintvenantkirchhoffelasticity-shear-4-3D)
test_ansys(saintvenantkirchhoffelasticity-shear-5-3D)
test_ansys(saintvenantkirchhoffelasticity-shear-6-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-1-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-2-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-3-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig11-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig22-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig33-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)

test_ansys(saintvenantkirchhoffelasticity2-shear-1-2D)
test_ansys(saintvenantkirchhoffelasticity2-shear-1-3D)
test_ansys(saintvenantkirchhoffelasticity2-shear-2-2D)
test_ansys(saintvenantkirchhoffelasticity2-shear-2-3D)
test_ansys(saintvenantkirchhoffelasticity2-shear-3-3D)
test_ansys(saintvenantkirchhoffelasticity2-shear-4-3D)
test_ansys(saintvenantkirchhoffelasticity2-shear-5-3D)
test_ansys(saintvenantkirchhoffelasticity2-shear-6-3D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-1-2D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-1-3D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-2-2D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-2-3D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-3-2D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-3-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-1-2D)
test_ansys(saintvenantkirchhoffelasticity3-shear-1-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-2-2D)
test_ansys(saintvenantkirchhoffelasticity3-shear-2-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-3-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-4-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-5-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-6-3D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-1-2D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-1-3D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-2-2D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-2-3D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-3-2D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-3-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-1-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-2-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-3-3D)
test_ansys(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_ansys(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_ansys(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-2D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FXY-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FXZ-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FYX-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FYZ-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FZX-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FZY-3D)
test_ansys(parametertest)
test_ansys(parametertest2)

