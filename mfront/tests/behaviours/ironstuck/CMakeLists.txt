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

macro(test_ironstuck test_arg)
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "ironstuck${test_arg}-${conf}.mtest")
      get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      configure_file(ironstuck${test_arg}.mtest.in
	${file} @ONLY)
      add_test(NAME ironstuck${test_arg}_${conf}_mtest
	CONFIGURATIONS ${conf}
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST ironstuck${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST ironstuck${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      if(TFEL_APPEND_VERSION)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_VERSION}/tests/behaviours/ironstuck"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      else(TFEL_APPEND_VERSION)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/ironstuck"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      endif(TFEL_APPEND_VERSION)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    configure_file(ironstuck${test_arg}.mtest.in
      ironstuck${test_arg}.mtest @ONLY)
    add_test(NAME ironstuck${test_arg}_mtest
      COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ironstuck${test_arg}.mtest)
    set_tests_properties(ironstuck${test_arg}_mtest
      PROPERTIES DEPENDS MFrontCastemBehaviours)
    set_tests_properties(ironstuck${test_arg}_mtest
      PROPERTIES DEPENDS MFrontMaterialProperties-castem)
    set_tests_properties(ironstuck${test_arg}_mtest
      PROPERTIES DEPENDS mtest)
    if(TFEL_APPEND_VERSION)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ironstuck${test_arg}.mtest
	DESTINATION "share/doc/mfront-${TFEL_VERSION}/tests/behaviours/ironstuck"
	COMPONENT mtest)
    else(TFEL_APPEND_VERSION)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ironstuck${test_arg}.mtest
	DESTINATION "share/doc/mfront/tests/behaviours/ironstuck"
	COMPONENT mtest)
    endif(TFEL_APPEND_VERSION)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_ironstuck)

test_ironstuck(chaboche)
test_ironstuck(compressibleneohookeanbehaviour)
test_ironstuck(ddif2-2)
test_ironstuck(ddif2-3)
test_ironstuck(ddif2-4)
test_ironstuck(ddif2)
test_ironstuck(druckerprager2)
test_ironstuck(druckerprager)
test_ironstuck(elasticity_malls-uniaxialloading-1-1D)
test_ironstuck(elasticity_malls-uniaxialloading-1-2D)
test_ironstuck(elasticity_malls-uniaxialloading-1-3D)
test_ironstuck(elasticity_malls-uniaxialloading-2-1D)
test_ironstuck(elasticity_malls-uniaxialloading-2-2D)
test_ironstuck(elasticity_malls-uniaxialloading-2-3D)
test_ironstuck(elasticity_malls-uniaxialloading-3-1D)
test_ironstuck(elasticity_malls-uniaxialloading-3-2D)
test_ironstuck(elasticity_malls-uniaxialloading-3-3D)
test_ironstuck(ellipticcreep)
test_ironstuck(finitestrainsinglecrystal-001)
test_ironstuck(finitestrainsinglecrystal-012-2)
test_ironstuck(finitestrainsinglecrystal-012)
test_ironstuck(finitestrainsinglecrystal-159)
test_ironstuck(finitestrainsinglecrystal2-001)
test_ironstuck(finitestrainsinglecrystal_numericaljacobian-001)
test_ironstuck(finitestrainsinglecrystal_numericaljacobian-012-2)
test_ironstuck(finitestrainsinglecrystal_numericaljacobian-012)
test_ironstuck(finitestrainsinglecrystal_numericaljacobian-159)
test_ironstuck(hayhurst)
test_ironstuck(implicithayhurst)
test_ironstuck(implicitmonocrystal)
test_ironstuck(implicitnorton3)
test_ironstuck(implicitnorton-planestress2)
test_ironstuck(implicitnorton-planestress)
test_ironstuck(implicitsimomieheelastoplasticity)
test_ironstuck(incompressiblemooneyrivlinbehaviour)
test_ironstuck(incompressibleplanestressgornetdesmoratbehaviourbiaxialtesting)
test_ironstuck(incompressibleplanestressgornetdesmoratbehavioursheartesting)
test_ironstuck(incompressibleplanestressgornetdesmoratbehaviouruniaxialtesting)
test_ironstuck(incompressibleplanestressmooneyrivlinbehaviouruniaxialtesting)
test_ironstuck(lorentz)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-1D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-1D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-1D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-1D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-planestress)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-1D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-1D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_ironstuck(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-planestress)
test_ironstuck(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-1D)
test_ironstuck(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D)
test_ironstuck(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_ironstuck(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-1D)
test_ironstuck(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D)
test_ironstuck(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_ironstuck(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-1D)
test_ironstuck(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D)
test_ironstuck(monocrystal_dd_cc_irradiation)
test_ironstuck(monocrystal_dd_cc_irradiation_numericaljacobian)
test_ironstuck(monocrystal_dd_cfc_irradiation)
test_ironstuck(monocrystal_dd_cfc)
test_ironstuck(monocrystal_dd_cfc_numericaljacobian)
test_ironstuck(monocrystal)
test_ironstuck(norton2)
test_ironstuck(norton-euler)
test_ironstuck(norton)
test_ironstuck(norton-rk2)
test_ironstuck(nortonrk2)
test_ironstuck(nortonrk3)
test_ironstuck(norton-rk4)
test_ironstuck(nortonrk4)
test_ironstuck(plasisocin)
test_ironstuck(plasticity)
test_ironstuck(polycrystals_dd_cfc)
test_ironstuck(strainhardeningcreep)
test_ironstuck(stressrelaxation)
test_ironstuck(T91-satohtest)
test_ironstuck(T91-viscositytest)
test_ironstuck(viscoplasisocin)
