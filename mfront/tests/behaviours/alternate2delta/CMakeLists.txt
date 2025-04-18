configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)

function(test_alternate2delta test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  set(_EXPECT_ADDITIONAL_ENVIRONMENT )
  set(_ADDITIONAL_ENVIRONMENT )
  set(_EXPECT_REFERENCE_FILE)
  set(_REFERENCE_FILE )
  foreach(_ARG ${ARGN})
    if(_EXPECT_ADDITIONAL_ENVIRONMENT)
      if(_ADDITIONAL_ENVIRONMENT)
	message(FATAL_ERROR "test_alternate2delta: environment already set")
      endif(_ADDITIONAL_ENVIRONMENT)
      set(_ADDITIONAL_ENVIRONMENT "${_ARG}")
      set(_EXPECT_ADDITIONAL_ENVIRONMENT )
    elseif(_EXPECT_REFERENCE_FILE)
      if(_REFERENCE_FILE)
	message(FATAL_ERROR "test_alternate2delta: reference file already set")
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
	message(FATAL_ERROR "test_alternate2delta: unsupported option '${_ARG}'")
      endif()
    endif()
  endforeach(_ARG ${ARGN})
  if(_EXPECT_ADDITIONAL_ENVIRONMENT)
    message(FATAL_ERROR "test_alternate2delta: expected environment variable")
  endif()
  if(_EXPECT_REFERENCE_FILE)
    message(FATAL_ERROR "test_alternate2delta: expected reference file")
  endif()
  if(_REFERENCE_FILE)
    set(_REFERENCE_FILE "${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/${_REFERENCE_FILE}")
  endif(_REFERENCE_FILE)
  set(test "alternate2delta${test_arg}_mtest")
  set(file "${CMAKE_CURRENT_SOURCE_DIR}/alternate2delta${test_arg}.mtest")
  set(_mtest_args)
  list(APPEND _mtest_args --verbose=level0)
  list(APPEND _mtest_args --result-file-output=false)
  list(APPEND _mtest_args --@interface@=umat)
  list(APPEND _mtest_args --@library@="$<TARGET_FILE:MFrontCastemBehaviours>")
  list(APPEND _mtest_args --@top_srcdir@=${PROJECT_SOURCE_DIR})
  list(APPEND _mtest_args --@mplibrary@="$<TARGET_FILE:MFrontMaterialProperties-castem>")
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
      DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/alternate2delta"
      COMPONENT mtest)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront/tests/behaviours/alternate2delta"
      COMPONENT mtest)
  endif(TFEL_APPEND_SUFFIX)
endfunction(test_alternate2delta)

test_alternate2delta(chaboche2)
test_alternate2delta(chaboche)
test_alternate2delta(compressibleneohookeanbehaviour)
test_alternate2delta(ddif2-2)
test_alternate2delta(ddif2-3)
test_alternate2delta(ddif2-4)
test_alternate2delta(ddif2)
test_alternate2delta(druckerprager2)
test_alternate2delta(druckerprager)
test_alternate2delta(elasticity_malls-uniaxialloading-1-1D)
test_alternate2delta(elasticity_malls-uniaxialloading-1-2D)
test_alternate2delta(elasticity_malls-uniaxialloading-1-3D)
test_alternate2delta(elasticity_malls-uniaxialloading-2-1D)
test_alternate2delta(elasticity_malls-uniaxialloading-2-2D)
test_alternate2delta(elasticity_malls-uniaxialloading-2-3D)
test_alternate2delta(elasticity_malls-uniaxialloading-3-1D)
test_alternate2delta(elasticity_malls-uniaxialloading-3-2D)
test_alternate2delta(elasticity_malls-uniaxialloading-3-3D)
test_alternate2delta(ellipticcreep)
test_alternate2delta(finitestrainsinglecrystal-001)
test_alternate2delta(finitestrainsinglecrystal-012-2)
test_alternate2delta(finitestrainsinglecrystal-012)
test_alternate2delta(finitestrainsinglecrystal-159)
test_alternate2delta(finitestrainsinglecrystal2-001)
test_alternate2delta(finitestrainsinglecrystal_numericaljacobian-001)
test_alternate2delta(finitestrainsinglecrystal_numericaljacobian-012-2)
test_alternate2delta(finitestrainsinglecrystal_numericaljacobian-012)
test_alternate2delta(finitestrainsinglecrystal_numericaljacobian-159)
test_alternate2delta(hayhurst)
test_alternate2delta(implicithayhurst)
test_alternate2delta(implicitmonocrystal)
test_alternate2delta(implicitnorton3)
test_alternate2delta(implicitnorton-planestress2)
test_alternate2delta(implicitnorton-planestress)
test_alternate2delta(implicitsimomieheelastoplasticity)
test_alternate2delta(incompressiblemooneyrivlinbehaviour)
test_alternate2delta(incompressibleplanestressgornetdesmoratbehaviourbiaxialtesting)
test_alternate2delta(incompressibleplanestressgornetdesmoratbehavioursheartesting)
test_alternate2delta(incompressibleplanestressgornetdesmoratbehaviouruniaxialtesting)
test_alternate2delta(incompressibleplanestressmooneyrivlinbehaviouruniaxialtesting)
test_alternate2delta(lorentz)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-1D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-1D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-1D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-1D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-planestress)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-1D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-1D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-planestress)
test_alternate2delta(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-1D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-1D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-1D)
test_alternate2delta(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D)
test_alternate2delta(monocrystal_dd_cc_irradiation)
test_alternate2delta(monocrystal_dd_cc_irradiation_numericaljacobian)
test_alternate2delta(monocrystal_dd_cfc_irradiation)
test_alternate2delta(monocrystal_dd_cfc)
test_alternate2delta(monocrystal_dd_cfc_numericaljacobian)
test_alternate2delta(monocrystal)
test_alternate2delta(norton2)
test_alternate2delta(norton-euler)
test_alternate2delta(norton)
test_alternate2delta(norton-rk2)
test_alternate2delta(nortonrk2)
test_alternate2delta(nortonrk3)
test_alternate2delta(norton-rk4)
test_alternate2delta(nortonrk4)
test_alternate2delta(plasisocin)
test_alternate2delta(plasticity)
test_alternate2delta(polycrystals_dd_cfc)
test_alternate2delta(strainhardeningcreep)
test_alternate2delta(stressrelaxation)
test_alternate2delta(T91-satohtest)
test_alternate2delta(T91-viscositytest)
test_alternate2delta(viscochaboche)
test_alternate2delta(viscoplasisocin)

# test_alternate2delta(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
# test_alternate2delta(monocrystal_dd_cc)
# test_alternate2delta(monocrystal_dd_cc_numericaljacobian)
