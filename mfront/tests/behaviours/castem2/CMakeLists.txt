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

macro(test_castem2 test_arg)
  configure_file(castem2${test_arg}.mtest.in
    castem2${test_arg}.mtest @ONLY)
  add_test(NAME castem2${test_arg}_mtest
    COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false castem2${test_arg}.mtest)
  set_tests_properties(castem2${test_arg}_mtest
    PROPERTIES DEPENDS MFrontCastemBehaviours)
  set_tests_properties(castem2${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
  add_custom_target(castem2${test_arg}.mtest-install
    ${CMAKE_COMMAND} -E copy castem2${test_arg}.mtest
    ${CMAKE_INSTALL_PREFIX}/share/mfront/tests/behaviours/castem2/castem2${test_arg}.mtest)
  add_dependencies(tests-install
    castem2${test_arg}.mtest-install)
endmacro(test_castem2)

test_castem2(elasticity)
test_castem2(elasticity2)
if(HAVE_CASTEM)
  test_castem2(elasticity3)
  set_tests_properties(castem2elasticity3_mtest
    PROPERTIES DEPENDS MFrontMaterialProperties-castem)
endif(HAVE_CASTEM)
test_castem2(elasticity4)
test_castem2(elasticity5)
test_castem2(elasticity6)
test_castem2(elasticity7)
test_castem2(elasticity8)
test_castem2(elasticity9)
test_castem2(elasticity10)
test_castem2(elasticity11)
test_castem2(elasticity12)
test_castem2(elasticity13)
test_castem2(elasticity14)
test_castem2(elasticity15)
test_castem2(elasticity16)
test_castem2(druckerprager)
test_castem2(druckerprager2)
test_castem2(norton)
test_castem2(norton2)
test_castem2(stressrelaxation)
test_castem2(strainhardeningcreep)
test_castem2(plasticity)
test_castem2(plasisocin)
test_castem2(viscoplasisocin)
test_castem2(chaboche)
test_castem2(chaboche2)
test_castem2(monocristal)
test_castem2(monocristal-2)
test_castem2(implicitmonocristal)
test_castem2(implicitmonocristal-2)
test_castem2(monocristal_dd_cfc)
test_castem2(monocristal_dd_cfc_numericaljacobian)
test_castem2(monocristal_dd_cfc_irradiation)
test_castem2(monocristal_dd_cc)
test_castem2(monocristal_dd_cc_numericaljacobian)
test_castem2(monocristal_dd_cc_irradiation_numericaljacobian)
test_castem2(implicithayhurst)
test_castem2(gursontvergaardneedlemanplasticflow)
test_castem2(gursontvergaardneedlemanviscoplasticflow)
test_castem2(ellipticcreep)
test_castem2(burger)
