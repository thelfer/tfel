include_directories(
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
configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles-30.txt"
    PolyCrystalsAngles-30.txt @ONLY)

set(mfront_tests_SOURCES
  MaterialPropertiesBoundsCheck
  ExternalStateVariablesBoundsCheck
  Elasticity
  Elasticity2
  Elasticity3
  OrthotropicElastic
  OrthotropicElastic2
  OrthotropicCreep
  OrthotropicCreep_rk42
  OrthotropicCreep_rkCastem
  ImplicitOrthotropicCreep
  Plasticity
  DruckerPrager
  DruckerPragerEcroLin
  DruckerPragerEcroLinDefault
  Norton NortonRK DDIF2
  Norton_Euler Norton_rk2
  Norton_rk4   Norton_rk42
  Norton_rkCastem
  ImplicitNorton
  ImplicitNorton2
  ImplicitNorton_Broyden
  ImplicitNorton_Broyden2
  ImplicitNorton_LevenbergMarquardt
  EllipticCreep
  StrainHardeningCreep
  Mazars
  Lorentz
  Lorentz2
  PlasIsoCin
  ViscoPlasIsoCin
  Chaboche
  Chaboche2
  ViscoChaboche
  ViscoMemoNrad
  ImplicitMonoCrystal
  MonoCrystal
  MonoCrystal_DD_CFC
  MonoCrystal_DD_CFC_NumericalJacobian
  MonoCrystal_DD_CFC_Irradiation
  MonoCrystal_DD_CC
  MonoCrystal_DD_CC_LevenbergMarquardt
  MonoCrystal_DD_CC_NumericalJacobian
  MonoCrystal_DD_CC_Irradiation
  MonoCrystal_DD_CC_Irradiation_NumericalJacobian
  PolyCrystals
  PolyCrystals_DD_CC
  PolyCrystals_DD_CFC
  Gurson
  GursonTvergaardNeedlemanPlasticFlow_NumericalJacobian
  GursonTvergaardNeedlemanViscoPlasticFlow_NumericalJacobian
  Hayhurst
  ImplicitHayhurst
  Burger
  AgeingBurger
  AnistropicLemaitreViscoplasticBehaviour
  SaintVenantKirchhoffElasticity
  SaintVenantKirchhoffElasticity2
  SaintVenantKirchhoffElasticity3
  FiniteStrainSingleCrystal
  FiniteStrainSingleCrystal_NumericalJacobian)
mfront_dependencies(MFrontAbaqusBehaviours
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)

mfront_behaviour_check_library(MFrontAbaqusBehaviours  abaqus ${mfront_tests_SOURCES})
target_link_libraries(MFrontAbaqusBehaviours
  AbaqusInterface
  MFrontProfiling
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontAbaqusBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

macro(test_abaqus test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "test_abaqus: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "abaqus${test_arg}-${conf}.mtest")
      get_property(MFrontAbaqusBehavioursBuildPath TARGET MFrontAbaqusBehaviours PROPERTY LOCATION_${conf})
      if(HAVE_CASTEM)
	get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      endif(HAVE_CASTEM)
      configure_file(abaqus${test_arg}.mtest.in
	${file} @ONLY)
      if(_NO_XML_OUTPUT)
	add_test(NAME abaqus${test_arg}_${conf}_mtest
	  CONFIGURATIONS ${conf}
	  COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false ${file})
      else(_NO_XML_OUTPUT)
	add_test(NAME abaqus${test_arg}_${conf}_mtest
	  CONFIGURATIONS ${conf}
	  COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(abaqus${test_arg}_${conf}_mtest
	  PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST abaqus${test_arg}_${conf}_mtest
          PROPERTY DEPENDS MFrontAbaqusBehaviours MFrontMaterialProperties-abaqus mtest
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST abaqus${test_arg}_${conf}_mtest
          PROPERTY DEPENDS MFrontAbaqusBehaviours MFrontMaterialProperties-castem mtest)
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	DESTINATION "share/mfront/tests/behaviours/abaqus"
	COMPONENT mtest)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontAbaqusBehavioursBuildPath TARGET MFrontAbaqusBehaviours PROPERTY LOCATION)
    if(HAVE_CASTEM)
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)
    endif(HAVE_CASTEM)
    configure_file(abaqus${test_arg}.mtest.in
      abaqus${test_arg}.mtest @ONLY)
    if(_NO_XML_OUTPUT)
      add_test(NAME abaqus${test_arg}_mtest
	COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false abaqus${test_arg}.mtest)
    else(_NO_XML_OUTPUT)
      add_test(NAME abaqus${test_arg}_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false abaqus${test_arg}.mtest)
    endif(_NO_XML_OUTPUT)
    if(_WILL_FAIL)
      set_tests_properties(abaqus${test_arg}_mtest
	PROPERTIES WILL_FAIL true)
    endif(_WILL_FAIL)
    set_tests_properties(abaqus${test_arg}_mtest
      PROPERTIES DEPENDS MFrontAbaqusBehaviours)
    set_tests_properties(abaqus${test_arg}_mtest
      PROPERTIES DEPENDS mtest)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/abaqus${test_arg}.mtest
      DESTINATION "share/mfront/tests/behaviours/abaqus"
      COMPONENT mtest)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_abaqus)

macro(test_abaqus_python test)
  configure_file(abaqus${test}.py.in
    abaqus${test}.py @ONLY)
  add_test(NAME abaqus${test}_py
    COMMAND ${PYTHON_EXECUTABLE} abaqus${test}.py)
  set_tests_properties(abaqus${test}_py
    PROPERTIES DEPENDS MFrontAbaqusBehaviours)
  if(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(abaqus${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python;$<TARGET_FILE_DIR:py_std__stl_vector>;$<TARGET_FILE_DIR:py_tfel_tests>")
  else(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(abaqus${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python")
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_abaqus_python)

# test_abaqus(materialpropertiesboundscheck
#   NO_XML_OUTPUT WILL_FAIL)
# test_abaqus(materialpropertiesboundscheck2 
#   NO_XML_OUTPUT WILL_FAIL)
# test_abaqus(materialpropertiesboundscheck3)
# test_abaqus(materialpropertiesboundscheck4)
# test_abaqus(materialpropertiesboundscheck5 
#   NO_XML_OUTPUT WILL_FAIL)
# test_abaqus(materialpropertiesboundscheck6)

# test_abaqus(externalstatevariablesboundscheck
#   NO_XML_OUTPUT WILL_FAIL)
# test_abaqus(externalstatevariablesboundscheck2 
#   NO_XML_OUTPUT WILL_FAIL)
# test_abaqus(externalstatevariablesboundscheck3)
# test_abaqus(externalstatevariablesboundscheck4)
# test_abaqus(externalstatevariablesboundscheck5 
#   NO_XML_OUTPUT WILL_FAIL)

test_abaqus(elasticity)
test_abaqus(elasticity2)
if(HAVE_CASTEM)
  test_abaqus(elasticity3)
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set_tests_properties(abaquselasticity3_${conf}_mtest
	PROPERTIES DEPENDS MFrontMaterialProperties-castem)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(abaquselasticity3_mtest
      PROPERTIES DEPENDS MFrontMaterialProperties-castem)
  endif(CMAKE_CONFIGURATION_TYPES)
endif(HAVE_CASTEM)
test_abaqus(elasticity4)
test_abaqus(elasticity5)
test_abaqus(elasticity6)
test_abaqus(elasticity7)
test_abaqus(elasticity8)
test_abaqus(elasticity9)
test_abaqus(elasticity10)
test_abaqus(elasticity11)
test_abaqus(elasticity12)
test_abaqus(elasticity13)
test_abaqus(elasticity14)
test_abaqus(elasticity15)
test_abaqus(elasticity16)
test_abaqus(elasticity18)
test_abaqus(elasticity19)
test_abaqus(elasticity20)
test_abaqus(elasticity21)
test_abaqus(elasticity22)
test_abaqus(elasticity23)
test_abaqus(elasticity24)
test_abaqus(elasticity25)
test_abaqus(elasticity26)
test_abaqus(elasticity27)
test_abaqus(elasticity33)
test_abaqus(druckerprager)
test_abaqus(druckerprager2)
test_abaqus(mazars)
# test_abaqus(norton)
# test_abaqus(norton2)
# test_abaqus(stressrelaxation)
# # test_abaqus(norton-euler)
# # test_abaqus(norton-rk2)
# # test_abaqus(norton-rk4)
test_abaqus(implicitnorton)
test_abaqus(implicitnorton2)
test_abaqus(implicitnorton3)
test_abaqus(implicitnorton_broyden)
test_abaqus(implicitnorton_broyden2)
test_abaqus(implicitnorton_levenbergmarquardt)
# test_abaqus(implicitorthotropiccreep)
# test_abaqus(plasticity)
# test_abaqus(ellipticcreep)
# test_abaqus(strainhardeningcreep)
# test_abaqus(plasisocin)
# test_abaqus(viscoplasisocin)
# test_abaqus(chaboche)
# test_abaqus(chaboche2)
# test_abaqus(viscochaboche)
test_abaqus(monocrystal)
test_abaqus(monocrystal-2)
test_abaqus(implicitmonocrystal)
test_abaqus(implicitmonocrystal-2)
test_abaqus(monocrystal_dd_cfc)
test_abaqus(monocrystal_dd_cfc_numericaljacobian)
test_abaqus(monocrystal_dd_cfc_irradiation)
test_abaqus(monocrystal_dd_cc)
test_abaqus(monocrystal_dd_cc_numericaljacobian)
test_abaqus(monocrystal_dd_cc_irradiation)
test_abaqus(monocrystal_dd_cc_irradiation_numericaljacobian)
# test_abaqus(implicithayhurst)
# test_abaqus(burger)
# test_abaqus(ageingburger)
# test_abaqus(gursontvergaardneedlemanplasticflow)
# test_abaqus(gursontvergaardneedlemanviscoplasticflow)
# test_abaqus(anistropiclemaitreviscoplasticbehaviour)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig11-2D)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig22-2D)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig33-2D)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-1-2D)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-2-2D)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-3-2D)
# test_abaqus(saintvenantkirchhoffelasticity-shear-1-2D)
# test_abaqus(saintvenantkirchhoffelasticity-shear-2-2D)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
# test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
# test_abaqus(saintvenantkirchhoffelasticity-shear-1-3D)
# test_abaqus(saintvenantkirchhoffelasticity-shear-2-3D)
# test_abaqus(saintvenantkirchhoffelasticity-shear-3-3D)
# test_abaqus(saintvenantkirchhoffelasticity-shear-4-3D)
# test_abaqus(saintvenantkirchhoffelasticity-shear-5-3D)
# test_abaqus(saintvenantkirchhoffelasticity-shear-6-3D)
# test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-1-2D)
# test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-2-2D)
# test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-3-2D)
# test_abaqus(saintvenantkirchhoffelasticity2-shear-1-2D)
# test_abaqus(saintvenantkirchhoffelasticity2-shear-2-2D)
# test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-1-3D)
# test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-2-3D)
# test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-3-3D)
# test_abaqus(saintvenantkirchhoffelasticity2-shear-1-3D)
# test_abaqus(saintvenantkirchhoffelasticity2-shear-2-3D)
# test_abaqus(saintvenantkirchhoffelasticity2-shear-3-3D)
# test_abaqus(saintvenantkirchhoffelasticity2-shear-4-3D)
# test_abaqus(saintvenantkirchhoffelasticity2-shear-5-3D)
# test_abaqus(saintvenantkirchhoffelasticity2-shear-6-3D)
# test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-1-2D)
# test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-2-2D)
# test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-3-2D)
# test_abaqus(saintvenantkirchhoffelasticity3-shear-1-2D)
# test_abaqus(saintvenantkirchhoffelasticity3-shear-2-2D)
# test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-1-3D)
# test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-2-3D)
# test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-3-3D)
# test_abaqus(saintvenantkirchhoffelasticity3-shear-1-3D)
# test_abaqus(saintvenantkirchhoffelasticity3-shear-2-3D)
# test_abaqus(saintvenantkirchhoffelasticity3-shear-3-3D)
# test_abaqus(saintvenantkirchhoffelasticity3-shear-4-3D)
# test_abaqus(saintvenantkirchhoffelasticity3-shear-5-3D)
# test_abaqus(saintvenantkirchhoffelasticity3-shear-6-3D)
# test_abaqus(finitestrainsinglecrystal-001)
# test_abaqus(finitestrainsinglecrystal-012)
# test_abaqus(finitestrainsinglecrystal-012-2)
# test_abaqus(finitestrainsinglecrystal-159)
# test_abaqus(finitestrainsinglecrystal_numericaljacobian-001)
# test_abaqus(finitestrainsinglecrystal_numericaljacobian-012)
# test_abaqus(finitestrainsinglecrystal_numericaljacobian-012-2)
# test_abaqus(finitestrainsinglecrystal_numericaljacobian-159)
