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

macro(test_crossed2delta test_arg)
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "crossed2delta${test_arg}-${conf}.mtest")
      get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      configure_file(crossed2delta${test_arg}.mtest.in
	${file} @ONLY)
      add_test(NAME crossed2delta${test_arg}_${conf}_mtest
	CONFIGURATIONS ${conf}
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
      set_tests_properties(crossed2delta${test_arg}_${conf}_mtest
	PROPERTIES DEPENDS MFrontCastemBehaviours)
      set_tests_properties(crossed2delta${test_arg}_${conf}_mtest
	PROPERTIES DEPENDS MFrontMaterialProperties-castem)
      set_tests_properties(crossed2delta${test_arg}_${conf}_mtest
	PROPERTIES DEPENDS mtest)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	DESTINATION "share/mfront/tests/behaviours/crossed2delta"
	CONFIGURATIONS ${conf}
	COMPONENT mtest)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    configure_file(crossed2delta${test_arg}.mtest.in
      crossed2delta${test_arg}.mtest @ONLY)
    add_test(NAME crossed2delta${test_arg}_mtest
      COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false crossed2delta${test_arg}.mtest)
    set_tests_properties(crossed2delta${test_arg}_mtest
      PROPERTIES DEPENDS MFrontCastemBehaviours)
    set_tests_properties(crossed2delta${test_arg}_mtest
      PROPERTIES DEPENDS MFrontMaterialProperties-castem)
    set_tests_properties(crossed2delta${test_arg}_mtest
      PROPERTIES DEPENDS mtest)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/crossed2delta${test_arg}.mtest
      DESTINATION "share/mfront/tests/behaviours/crossed2delta"
      COMPONENT mtest)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_crossed2delta)

test_crossed2delta(chaboche)
test_crossed2delta(ddif2-2)
test_crossed2delta(ddif2-3)
test_crossed2delta(ddif2-4)
test_crossed2delta(ddif2)
test_crossed2delta(druckerprager)
test_crossed2delta(ellipticcreep)
test_crossed2delta(finitestrainsinglecrystal-001)
test_crossed2delta(finitestrainsinglecrystal-012-2)
test_crossed2delta(finitestrainsinglecrystal-012)
test_crossed2delta(finitestrainsinglecrystal-159)
test_crossed2delta(finitestrainsinglecrystal2-001)
test_crossed2delta(finitestrainsinglecrystal_numericaljacobian-001)
test_crossed2delta(finitestrainsinglecrystal_numericaljacobian-012-2)
test_crossed2delta(finitestrainsinglecrystal_numericaljacobian-012)
test_crossed2delta(finitestrainsinglecrystal_numericaljacobian-159)
test_crossed2delta(hayhurst)
test_crossed2delta(implicithayhurst)
test_crossed2delta(implicitmonocrystal)
test_crossed2delta(implicitnorton3)
test_crossed2delta(implicitnorton-planestress2)
test_crossed2delta(lorentz)
test_crossed2delta(monocrystal_dd_cc_irradiation)
test_crossed2delta(monocrystal_dd_cc_irradiation_numericaljacobian)
test_crossed2delta(monocrystal_dd_cfc_irradiation)
test_crossed2delta(monocrystal_dd_cfc)
test_crossed2delta(monocrystal_dd_cfc_numericaljacobian)
test_crossed2delta(monocrystal)
test_crossed2delta(norton2)
test_crossed2delta(norton-euler)
test_crossed2delta(norton)
test_crossed2delta(norton-rk2)
test_crossed2delta(nortonrk2)
test_crossed2delta(nortonrk3)
test_crossed2delta(norton-rk4)
test_crossed2delta(nortonrk4)
test_crossed2delta(plasisocin)
test_crossed2delta(plasticity)
test_crossed2delta(strainhardeningcreep)
test_crossed2delta(stressrelaxation)
test_crossed2delta(T91-satohtest)
test_crossed2delta(T91-viscositytest)
test_crossed2delta(viscoplasisocin)

# test_crossed2delta(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
# test_crossed2delta(monocrystal_dd_cc)
# test_crossed2delta(monocrystal_dd_cc_numericaljacobian)
