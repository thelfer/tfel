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

macro(test_alternatesecant test_arg)
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "alternatesecant${test_arg}-${conf}.mtest")
      get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      configure_file(alternatesecant${test_arg}.mtest.in
	${file} @ONLY)
      add_test(NAME alternatesecant${test_arg}_${conf}_mtest
	CONFIGURATIONS ${conf}
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST alternatesecant${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST alternatesecant${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      if(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/alternatesecant"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      else(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/alternatesecant"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      endif(TFEL_APPEND_SUFFIX)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    configure_file(alternatesecant${test_arg}.mtest.in
      alternatesecant${test_arg}.mtest @ONLY)
    add_test(NAME alternatesecant${test_arg}_mtest
      COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false alternatesecant${test_arg}.mtest)
    set_tests_properties(alternatesecant${test_arg}_mtest
      PROPERTIES DEPENDS MFrontCastemBehaviours)
    set_tests_properties(alternatesecant${test_arg}_mtest
      PROPERTIES DEPENDS MFrontMaterialProperties-castem)
    set_tests_properties(alternatesecant${test_arg}_mtest
      PROPERTIES DEPENDS mtest)
      if(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/alternatesecant${test_arg}.mtest
	  DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/alternatesecant"
	  COMPONENT mtest)
      else(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/alternatesecant${test_arg}.mtest
	  DESTINATION "share/doc/mfront/tests/behaviours/alternatesecant"
	  COMPONENT mtest)
      endif(TFEL_APPEND_SUFFIX)
    endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_alternatesecant)

test_alternatesecant(chaboche2)
test_alternatesecant(chaboche)
test_alternatesecant(compressibleneohookeanbehaviour)
test_alternatesecant(ddif2-2)
test_alternatesecant(ddif2-3)
test_alternatesecant(ddif2-4)
test_alternatesecant(ddif2)
test_alternatesecant(druckerprager2)
test_alternatesecant(druckerprager)
test_alternatesecant(elasticity_malls-uniaxialloading-1-1D)
test_alternatesecant(elasticity_malls-uniaxialloading-1-2D)
test_alternatesecant(elasticity_malls-uniaxialloading-1-3D)
test_alternatesecant(elasticity_malls-uniaxialloading-2-1D)
test_alternatesecant(elasticity_malls-uniaxialloading-2-2D)
test_alternatesecant(elasticity_malls-uniaxialloading-2-3D)
test_alternatesecant(elasticity_malls-uniaxialloading-3-1D)
test_alternatesecant(elasticity_malls-uniaxialloading-3-2D)
test_alternatesecant(elasticity_malls-uniaxialloading-3-3D)
test_alternatesecant(ellipticcreep)
test_alternatesecant(finitestrainsinglecrystal-001)
test_alternatesecant(finitestrainsinglecrystal-012-2)
test_alternatesecant(finitestrainsinglecrystal-012)
test_alternatesecant(finitestrainsinglecrystal-159)
test_alternatesecant(finitestrainsinglecrystal2-001)
test_alternatesecant(finitestrainsinglecrystal_numericaljacobian-001)
test_alternatesecant(finitestrainsinglecrystal_numericaljacobian-012-2)
test_alternatesecant(finitestrainsinglecrystal_numericaljacobian-012)
test_alternatesecant(finitestrainsinglecrystal_numericaljacobian-159)
test_alternatesecant(hayhurst)
test_alternatesecant(implicithayhurst)
test_alternatesecant(implicitmonocrystal)
test_alternatesecant(implicitnorton3)
test_alternatesecant(implicitnorton-planestress2)
test_alternatesecant(implicitnorton-planestress)
test_alternatesecant(implicitsimomieheelastoplasticity)
test_alternatesecant(incompressiblemooneyrivlinbehaviour)
test_alternatesecant(incompressibleplanestressgornetdesmoratbehaviourbiaxialtesting)
test_alternatesecant(incompressibleplanestressgornetdesmoratbehavioursheartesting)
test_alternatesecant(incompressibleplanestressgornetdesmoratbehaviouruniaxialtesting)
test_alternatesecant(incompressibleplanestressmooneyrivlinbehaviouruniaxialtesting)
test_alternatesecant(lorentz)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-1D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-1D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-1D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-1D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-planestress)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-1D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-1D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-planestress)
test_alternatesecant(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-1D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-1D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-1D)
test_alternatesecant(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D)
test_alternatesecant(monocrystal_dd_cc_irradiation)
test_alternatesecant(monocrystal_dd_cc_irradiation_numericaljacobian)
test_alternatesecant(monocrystal_dd_cfc_irradiation)
test_alternatesecant(monocrystal_dd_cfc)
test_alternatesecant(monocrystal_dd_cfc_numericaljacobian)
test_alternatesecant(monocrystal)
test_alternatesecant(norton2)
test_alternatesecant(norton-euler)
test_alternatesecant(norton)
test_alternatesecant(norton-rk2)
test_alternatesecant(nortonrk2)
test_alternatesecant(nortonrk3)
test_alternatesecant(norton-rk4)
test_alternatesecant(nortonrk4)
test_alternatesecant(plasisocin)
test_alternatesecant(plasticity)
test_alternatesecant(polycrystals_dd_cfc)
test_alternatesecant(strainhardeningcreep)
test_alternatesecant(stressrelaxation)
test_alternatesecant(T91-satohtest)
test_alternatesecant(T91-viscositytest)
test_alternatesecant(viscochaboche)
test_alternatesecant(viscoplasisocin)

# test_alternatesecant(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
# test_alternatesecant(monocrystal_dd_cc)
# test_alternatesecant(monocrystal_dd_cc_numericaljacobian)
