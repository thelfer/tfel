configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles-30.txt"
    PolyCrystalsAngles-30.txt @ONLY)

function(test_fanderson test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  set(_EXPECT_ADDITIONAL_ENVIRONMENT )
  set(_ADDITIONAL_ENVIRONMENT )
  set(_EXPECT_REFERENCE_FILE)
  set(_REFERENCE_FILE )
  foreach(_ARG ${ARGN})
    if(_EXPECT_ADDITIONAL_ENVIRONMENT)
      if(_ADDITIONAL_ENVIRONMENT)
	message(FATAL_ERROR "test_fanderson: environment already set")
      endif(_ADDITIONAL_ENVIRONMENT)
      set(_ADDITIONAL_ENVIRONMENT "${_ARG}")
      set(_EXPECT_ADDITIONAL_ENVIRONMENT )
    elseif(_EXPECT_REFERENCE_FILE)
      if(_REFERENCE_FILE)
	message(FATAL_ERROR "test_fanderson: reference file already set")
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
	message(FATAL_ERROR "test_fanderson: unsupported option '${_ARG}'")
      endif()
    endif()
  endforeach(_ARG ${ARGN})
  if(_EXPECT_ADDITIONAL_ENVIRONMENT)
    message(FATAL_ERROR "test_fanderson: expected environment variable")
  endif()
  if(_EXPECT_REFERENCE_FILE)
    message(FATAL_ERROR "test_fanderson: expected reference file")
  endif()
  if(_REFERENCE_FILE)
    set(_REFERENCE_FILE "${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/${_REFERENCE_FILE}")
  endif(_REFERENCE_FILE)
  set(test "fanderson${test_arg}_mtest")
  set(file "${CMAKE_CURRENT_SOURCE_DIR}/fanderson${test_arg}.mtest")
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
      DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/fanderson"
      COMPONENT mtest)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront/tests/behaviours/fanderson"
      COMPONENT mtest)
  endif(TFEL_APPEND_SUFFIX)
endfunction(test_fanderson)

test_fanderson(generalizedmaxwell)
test_fanderson(ddif2)
test_fanderson(ddif2-2)
test_fanderson(ddif2-3)
test_fanderson(ddif2-4)
test_fanderson(lorentz)
test_fanderson(lorentz2)
test_fanderson(mazars)
test_fanderson(mazars2)
test_fanderson(druckerprager)
test_fanderson(druckerprager2)
test_fanderson(norton)
test_fanderson(norton2)
test_fanderson(nortonrk2)
test_fanderson(nortonrk3)
test_fanderson(nortonrk4)
test_fanderson(implicitnorton)
test_fanderson(implicitnorton2)
test_fanderson(implicitnorton3)
test_fanderson(implicitnorton-planestress)
test_fanderson(implicitnorton-planestress2)
test_fanderson(implicitnorton-levenbergmarquardt)
test_fanderson(implicitnorton4-planestress)
test_fanderson(implicitorthotropiccreep2-planestress)
test_fanderson(stressrelaxation)
test_fanderson(norton-euler)
test_fanderson(norton-rk2)
test_fanderson(norton-rk4)
test_fanderson(orthotropiccreep)
test_fanderson(orthotropiccreep-rk42)
test_fanderson(orthotropiccreep-rkCastem)
test_fanderson(strainhardeningcreep)
test_fanderson(plasticity)
test_fanderson(plasisocin)
test_fanderson(viscoplasisocin)
test_fanderson(chaboche)
test_fanderson(chaboche2)
test_fanderson(viscochaboche)
test_fanderson(monocrystal)
test_fanderson(monocrystal-2)
test_fanderson(implicitmonocrystal)
test_fanderson(implicitmonocrystal-2)
test_fanderson(monocrystal_dd_cfc)
test_fanderson(monocrystal_dd_cfc_numericaljacobian)
test_fanderson(monocrystal_dd_cfc_irradiation)
test_fanderson(monocrystal_dd_cc)
test_fanderson(monocrystal_dd_cc_numericaljacobian)
test_fanderson(monocrystal_dd_cc_irradiation)
test_fanderson(monocrystal_dd_cc_irradiation_numericaljacobian)
test_fanderson(polycrystals)
test_fanderson(polycrystals_dd_cc)
test_fanderson(polycrystals_dd_cfc)
test_fanderson(hayhurst)
test_fanderson(implicithayhurst)
test_fanderson(gurson)
test_fanderson(gursontvergaardneedlemanplasticflow)
test_fanderson(gursontvergaardneedlemanviscoplasticflow)
test_fanderson(ellipticcreep)
test_fanderson(burger)
test_fanderson(ageingburger)
test_fanderson(tvergaard)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-sig11-1D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-sig22-1D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-sig33-1D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-sig11-2D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-sig22-2D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-sig33-2D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-1-1D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-2-1D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-3-1D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-1-2D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-2-2D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-3-2D)
test_fanderson(saintvenantkirchhoffelasticity-shear-1-2D)
test_fanderson(saintvenantkirchhoffelasticity-shear-2-2D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_fanderson(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_fanderson(saintvenantkirchhoffelasticity-shear-1-3D)
test_fanderson(saintvenantkirchhoffelasticity-shear-2-3D)
test_fanderson(saintvenantkirchhoffelasticity-shear-3-3D)
test_fanderson(saintvenantkirchhoffelasticity-shear-4-3D)
test_fanderson(saintvenantkirchhoffelasticity-shear-5-3D)
test_fanderson(saintvenantkirchhoffelasticity-shear-6-3D)
test_fanderson(elasticity_frst-uniaxialloading-1-1D)
test_fanderson(elasticity_frst-uniaxialloading-2-1D)
test_fanderson(elasticity_frst-uniaxialloading-3-1D)
test_fanderson(elasticity_frst-uniaxialloading-1-2D)
test_fanderson(elasticity_frst-uniaxialloading-2-2D)
test_fanderson(elasticity_frst-uniaxialloading-3-2D)
test_fanderson(elasticity_frst-shear-1-2D)
test_fanderson(elasticity_frst-shear-2-2D)
test_fanderson(elasticity_frst-uniaxialloading-1-3D)
test_fanderson(elasticity_frst-uniaxialloading-2-3D)
test_fanderson(elasticity_frst-uniaxialloading-3-3D)
test_fanderson(elasticity_frst-shear-1-3D)
test_fanderson(elasticity_frst-shear-2-3D)
test_fanderson(elasticity_frst-shear-3-3D)
test_fanderson(elasticity_frst-shear-4-3D)
test_fanderson(elasticity_frst-shear-5-3D)
test_fanderson(elasticity_frst-shear-6-3D)
test_fanderson(finiterotationsmallstrainelasticity-uniaxialloading-1-1D)
test_fanderson(finiterotationsmallstrainelasticity-uniaxialloading-2-1D)
test_fanderson(finiterotationsmallstrainelasticity-uniaxialloading-3-1D)
test_fanderson(finiterotationsmallstrainelasticity-uniaxialloading-1-2D)
test_fanderson(finiterotationsmallstrainelasticity-uniaxialloading-2-2D)
test_fanderson(finiterotationsmallstrainelasticity-uniaxialloading-3-2D)
test_fanderson(finiterotationsmallstrainelasticity-shear-1-2D)
test_fanderson(finiterotationsmallstrainelasticity-shear-2-2D)
test_fanderson(finiterotationsmallstrainelasticity-uniaxialloading-1-3D)
test_fanderson(finiterotationsmallstrainelasticity-uniaxialloading-2-3D)
test_fanderson(finiterotationsmallstrainelasticity-uniaxialloading-3-3D)
test_fanderson(finiterotationsmallstrainelasticity-shear-1-3D)
test_fanderson(finiterotationsmallstrainelasticity-shear-2-3D)
test_fanderson(finiterotationsmallstrainelasticity-shear-3-3D)
test_fanderson(finiterotationsmallstrainelasticity-shear-4-3D)
test_fanderson(finiterotationsmallstrainelasticity-shear-5-3D)
test_fanderson(finiterotationsmallstrainelasticity-shear-6-3D)
test_fanderson(finiterotationsmallstrainelasticity2-uniaxialloading-1-1D)
test_fanderson(finiterotationsmallstrainelasticity2-uniaxialloading-2-1D)
test_fanderson(finiterotationsmallstrainelasticity2-uniaxialloading-3-1D)
test_fanderson(finiterotationsmallstrainelasticity2-uniaxialloading-1-2D)
test_fanderson(finiterotationsmallstrainelasticity2-uniaxialloading-2-2D)
test_fanderson(finiterotationsmallstrainelasticity2-uniaxialloading-3-2D)
test_fanderson(finiterotationsmallstrainelasticity2-shear-1-2D)
test_fanderson(finiterotationsmallstrainelasticity2-shear-2-2D)
test_fanderson(finiterotationsmallstrainelasticity2-uniaxialloading-1-3D)
test_fanderson(finiterotationsmallstrainelasticity2-uniaxialloading-2-3D)
test_fanderson(finiterotationsmallstrainelasticity2-uniaxialloading-3-3D)
test_fanderson(finiterotationsmallstrainelasticity2-shear-1-3D)
test_fanderson(finiterotationsmallstrainelasticity2-shear-2-3D)
test_fanderson(finiterotationsmallstrainelasticity2-shear-3-3D)
test_fanderson(finiterotationsmallstrainelasticity2-shear-4-3D)
test_fanderson(finiterotationsmallstrainelasticity2-shear-5-3D)
test_fanderson(finiterotationsmallstrainelasticity2-shear-6-3D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-1D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-1D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-1D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-planestress)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-planestress)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-1D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-1D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-1D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D)
test_fanderson(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_fanderson(elasticity_log1D-uniaxialloading-1-1D)
test_fanderson(elasticity_log1D-uniaxialloading-2-1D)
test_fanderson(elasticity_log1D-uniaxialloading-3-1D)
test_fanderson(norton_log1D-uniaxialloading-1-1D)
test_fanderson(norton_log1D-uniaxialloading-2-1D)
test_fanderson(norton_log1D-uniaxialloading-3-1D)
test_fanderson(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-1-1D)
test_fanderson(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-2-1D)
test_fanderson(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-3-1D)
test_fanderson(elasticity_LogarithmicStrain1DWrapper-thermalstrain)
test_fanderson(elasticity_LogarithmicStrain1DWrapper-thermalstrain2)
test_fanderson(norton_LogarithmicStrain1DWrapper-uniaxialloading-1-1D)
test_fanderson(norton_LogarithmicStrain1DWrapper-uniaxialloading-2-1D)
test_fanderson(norton_LogarithmicStrain1DWrapper-uniaxialloading-3-1D)
test_fanderson(elasticity_malls-uniaxialloading-1-1D)
test_fanderson(elasticity_malls-uniaxialloading-1-2D)
test_fanderson(elasticity_malls-uniaxialloading-1-3D)
test_fanderson(elasticity_malls-uniaxialloading-2-1D)
test_fanderson(elasticity_malls-uniaxialloading-2-2D)
test_fanderson(elasticity_malls-uniaxialloading-2-3D)
test_fanderson(elasticity_malls-uniaxialloading-3-1D)
test_fanderson(elasticity_malls-uniaxialloading-3-2D)
test_fanderson(elasticity_malls-uniaxialloading-3-3D)
test_fanderson(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-1D)
test_fanderson(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-1D)
test_fanderson(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-1D)
test_fanderson(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D)
test_fanderson(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D)
test_fanderson(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D)
test_fanderson(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_fanderson(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_fanderson(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
test_fanderson(incompressiblemooneyrivlinbehaviour)
test_fanderson(incompressibleplanestressgornetdesmoratbehavioursheartesting)
test_fanderson(incompressibleplanestressgornetdesmoratbehaviouruniaxialtesting)
test_fanderson(compressibleneohookeanbehaviour)
test_fanderson(incompressibleplanestressmooneyrivlinbehaviouruniaxialtesting)
test_fanderson(incompressibleplanestressgornetdesmoratbehaviourbiaxialtesting)
test_fanderson(implicitsimomieheelastoplasticity)
test_fanderson(T91-satohtest)
test_fanderson(T91-viscositytest)
test_fanderson(finitestrainsinglecrystal-001)
test_fanderson(finitestrainsinglecrystal-012)
test_fanderson(finitestrainsinglecrystal-012-2)
test_fanderson(finitestrainsinglecrystal-159)
test_fanderson(finitestrainsinglecrystal_numericaljacobian-001)
test_fanderson(finitestrainsinglecrystal_numericaljacobian-012)
test_fanderson(finitestrainsinglecrystal_numericaljacobian-012-2)
test_fanderson(finitestrainsinglecrystal_numericaljacobian-159)
test_fanderson(finitestrainsinglecrystal2-001)
test_fanderson(anistropiclemaitreviscoplasticbehaviour)
test_fanderson(tssf)

