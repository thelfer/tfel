include_directories(
  "${CASTEMHOME}/include"
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)

macro(test_alternate2delta test_arg)
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "alternate2delta${test_arg}-${conf}.mtest")
      get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      configure_file(alternate2delta${test_arg}.mtest.in
	${file} @ONLY)
      add_test(NAME alternate2delta${test_arg}_${conf}_mtest
	CONFIGURATIONS ${conf}
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST alternate2delta${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST alternate2delta${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      if(TFEL_APPEND_VERSION)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_VERSION}/tests/behaviours/alternate2delta"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      else(TFEL_APPEND_VERSION)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/alternate2delta"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      endif(TFEL_APPEND_VERSION)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    configure_file(alternate2delta${test_arg}.mtest.in
      alternate2delta${test_arg}.mtest @ONLY)
    add_test(NAME alternate2delta${test_arg}_mtest
      COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false alternate2delta${test_arg}.mtest)
    set_tests_properties(alternate2delta${test_arg}_mtest
      PROPERTIES DEPENDS MFrontCastemBehaviours)
    set_tests_properties(alternate2delta${test_arg}_mtest
      PROPERTIES DEPENDS MFrontMaterialProperties-castem)
    set_tests_properties(alternate2delta${test_arg}_mtest
      PROPERTIES DEPENDS mtest)
    if(TFEL_APPEND_VERSION)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/alternate2delta${test_arg}.mtest
	DESTINATION "share/doc/mfront-${TFEL_VERSION}/tests/behaviours/alternate2delta"
	COMPONENT mtest)
    else(TFEL_APPEND_VERSION)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/alternate2delta${test_arg}.mtest
	DESTINATION "share/doc/mfront/tests/behaviours/alternate2delta"
	COMPONENT mtest)
    endif(TFEL_APPEND_VERSION)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_alternate2delta)

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
