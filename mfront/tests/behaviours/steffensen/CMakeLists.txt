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

macro(test_steffensen test_arg)
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "steffensen${test_arg}-${conf}.mtest")
      get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      configure_file(steffensen${test_arg}.mtest.in
	${file} @ONLY)
      add_test(NAME steffensen${test_arg}_${conf}_mtest
	CONFIGURATIONS ${conf}
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST steffensen${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST steffensen${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      if(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/steffensen"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      else(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/steffensen"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      endif(TFEL_APPEND_SUFFIX)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    configure_file(steffensen${test_arg}.mtest.in
      steffensen${test_arg}.mtest @ONLY)
    add_test(NAME steffensen${test_arg}_mtest
      COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false steffensen${test_arg}.mtest)
    set_tests_properties(steffensen${test_arg}_mtest
      PROPERTIES DEPENDS MFrontCastemBehaviours)
    set_tests_properties(steffensen${test_arg}_mtest
      PROPERTIES DEPENDS MFrontMaterialProperties-castem)
    set_tests_properties(steffensen${test_arg}_mtest
      PROPERTIES DEPENDS mtest)
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/steffensen${test_arg}.mtest
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/steffensen"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/steffensen${test_arg}.mtest
	DESTINATION "share/doc/mfront/tests/behaviours/steffensen"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_steffensen)

test_steffensen(ddif2-2)
test_steffensen(ddif2-3)
test_steffensen(ddif2-4)
test_steffensen(ddif2)
test_steffensen(druckerprager2)
test_steffensen(druckerprager)
test_steffensen(elasticity_malls-uniaxialloading-1-1D)
test_steffensen(elasticity_malls-uniaxialloading-1-2D)
test_steffensen(elasticity_malls-uniaxialloading-1-3D)
test_steffensen(elasticity_malls-uniaxialloading-2-1D)
test_steffensen(elasticity_malls-uniaxialloading-2-2D)
test_steffensen(elasticity_malls-uniaxialloading-2-3D)
test_steffensen(elasticity_malls-uniaxialloading-3-1D)
test_steffensen(elasticity_malls-uniaxialloading-3-2D)
test_steffensen(elasticity_malls-uniaxialloading-3-3D)
test_steffensen(ellipticcreep)
test_steffensen(finitestrainsinglecrystal-001)
test_steffensen(finitestrainsinglecrystal-012-2)
test_steffensen(finitestrainsinglecrystal-012)
test_steffensen(finitestrainsinglecrystal-159)
test_steffensen(finitestrainsinglecrystal2-001)
test_steffensen(finitestrainsinglecrystal_numericaljacobian-001)
test_steffensen(finitestrainsinglecrystal_numericaljacobian-012-2)
test_steffensen(finitestrainsinglecrystal_numericaljacobian-012)
test_steffensen(finitestrainsinglecrystal_numericaljacobian-159)
test_steffensen(hayhurst)
test_steffensen(implicithayhurst)
test_steffensen(implicitmonocrystal)
test_steffensen(implicitnorton3)
test_steffensen(implicitnorton-planestress2)
test_steffensen(implicitnorton-planestress)
test_steffensen(implicitsimomieheelastoplasticity)
test_steffensen(incompressiblemooneyrivlinbehaviour)
test_steffensen(incompressibleplanestressgornetdesmoratbehaviourbiaxialtesting)
test_steffensen(incompressibleplanestressgornetdesmoratbehavioursheartesting)
test_steffensen(incompressibleplanestressgornetdesmoratbehaviouruniaxialtesting)
test_steffensen(incompressibleplanestressmooneyrivlinbehaviouruniaxialtesting)
test_steffensen(lorentz)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-1D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-1D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-1D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-1D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-planestress)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-1D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-1D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_steffensen(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-planestress)
test_steffensen(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-1D)
test_steffensen(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D)
test_steffensen(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-1D)
test_steffensen(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D)
test_steffensen(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-1D)
test_steffensen(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D)
test_steffensen(monocrystal_dd_cc_irradiation)
test_steffensen(monocrystal_dd_cc_irradiation_numericaljacobian)
test_steffensen(monocrystal_dd_cfc_irradiation)
test_steffensen(monocrystal_dd_cfc)
test_steffensen(monocrystal_dd_cfc_numericaljacobian)
test_steffensen(monocrystal)
test_steffensen(norton2)
test_steffensen(norton-euler)
test_steffensen(norton-rk2)
test_steffensen(nortonrk2)
test_steffensen(nortonrk3)
test_steffensen(norton-rk4)
test_steffensen(nortonrk4)
test_steffensen(plasisocin)
test_steffensen(plasticity)
test_steffensen(strainhardeningcreep)
test_steffensen(stressrelaxation)
test_steffensen(T91-viscositytest)
test_steffensen(viscoplasisocin)
