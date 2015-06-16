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

get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION)
get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)

configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)

macro(test_crosseddelta2 test_arg)
  configure_file(crosseddelta2${test_arg}.mtest.in
    crosseddelta2${test_arg}.mtest @ONLY)
  add_test(NAME crosseddelta2${test_arg}_mtest
    COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false crosseddelta2${test_arg}.mtest)
  set_tests_properties(crosseddelta2${test_arg}_mtest
    PROPERTIES DEPENDS MFrontCastemBehaviours)
  set_tests_properties(crosseddelta2${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/crosseddelta2${test_arg}.mtest
          DESTINATION "share/mfront/tests/behaviours/crosseddelta2"
	  COMPONENT mtest)
endmacro(test_crosseddelta2)

test_crosseddelta2(ddif2-2)
test_crosseddelta2(ddif2-3)
test_crosseddelta2(ddif2-4)
test_crosseddelta2(ddif2)
test_crosseddelta2(druckerprager)
test_crosseddelta2(ellipticcreep)
test_crosseddelta2(finitestrainsinglecrystal-001)
test_crosseddelta2(finitestrainsinglecrystal-012-2)
test_crosseddelta2(finitestrainsinglecrystal-012)
test_crosseddelta2(finitestrainsinglecrystal-159)
test_crosseddelta2(finitestrainsinglecrystal2-001)
test_crosseddelta2(finitestrainsinglecrystal_numericaljacobian-001)
test_crosseddelta2(finitestrainsinglecrystal_numericaljacobian-012-2)
test_crosseddelta2(finitestrainsinglecrystal_numericaljacobian-012)
test_crosseddelta2(finitestrainsinglecrystal_numericaljacobian-159)
test_crosseddelta2(hayhurst)
test_crosseddelta2(implicithayhurst)
test_crosseddelta2(implicitmonocrystal)
test_crosseddelta2(implicitnorton3)
test_crosseddelta2(implicitnorton-planestress2)
test_crosseddelta2(implicitnorton-planestress)
test_crosseddelta2(lorentz)
test_crosseddelta2(monocrystal_dd_cc_irradiation)
test_crosseddelta2(monocrystal_dd_cc_irradiation_numericaljacobian)
test_crosseddelta2(monocrystal_dd_cfc_irradiation)
test_crosseddelta2(monocrystal_dd_cfc)
test_crosseddelta2(monocrystal_dd_cfc_numericaljacobian)
test_crosseddelta2(monocrystal)
test_crosseddelta2(norton2)
test_crosseddelta2(norton-euler)
test_crosseddelta2(norton)
test_crosseddelta2(norton-rk2)
test_crosseddelta2(nortonrk2)
test_crosseddelta2(nortonrk3)
test_crosseddelta2(norton-rk4)
test_crosseddelta2(nortonrk4)
test_crosseddelta2(plasisocin)
test_crosseddelta2(plasticity)
test_crosseddelta2(polycrystals_dd_cfc)
test_crosseddelta2(strainhardeningcreep)
test_crosseddelta2(stressrelaxation)
test_crosseddelta2(T91-viscositytest)
test_crosseddelta2(viscoplasisocin)

# test_crosseddelta2(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
# test_crosseddelta2(monocrystal_dd_cc)
# test_crosseddelta2(monocrystal_dd_cc_numericaljacobian)
