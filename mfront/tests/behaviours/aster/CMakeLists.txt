configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles-30.txt"
    PolyCrystalsAngles-30.txt @ONLY)

set(mfront_tests_SOURCES
  MaterialPropertiesBoundsCheck
  ExternalStateVariablesBoundsCheck
  Tvergaard
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
  HosfordPerfectPlasticity
  JohnsonCook_s
  JohnsonCook_ssr
  JohnsonCook_ssrt
  RusinekKlepaczko_ssrt
  Norton NortonRK DDIF2
  Norton_Euler Norton_rk2
  Norton_rk4   Norton_rk42
  Norton_rkCastem
  ImplicitNorton
  ImplicitNorton2
  ImplicitNorton_Broyden
  ImplicitNorton_Broyden2
  ImplicitNorton_LevenbergMarquardt
  ImplicitNorton_NumericallyComputedJacobianBlocks
  EllipticCreep
  StrainHardeningCreep
  Mazars
  UnilateralMazars
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
  BerveillerZaouiPolyCrystal
  BerveillerZaouiPolyCrystal_DD_CC
  BerveillerZaouiPolyCrystal_DD_CFC
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
  SaintVenantKirchhoffElasticityTotalLagrangian
  FiniteStrainSingleCrystal
  FiniteStrainSingleCrystal_NumericalJacobian
  ComputeStressFreeExpansion
  IsotropicLinearSwelling
  IsotropicVolumeSwelling
  OrthotropicSwelling
  OrthotropicSwelling2
  AxialGrowth
  ComputeThermalExpansion
  ComputeThermalExpansion2
  ComputeThermalExpansion3
  ParameterTest
  ParameterTest2
  GenerateMTestFileOnFailure)
mfront_dependencies(MFrontAsterBehaviours
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)

mfront_behaviour_check_library(MFrontAsterBehaviours aster
  ${mfront_tests_SOURCES})
target_link_libraries(MFrontAsterBehaviours
  PRIVATE AsterInterface)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontAsterBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)


macro(test_aster test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "test_aster: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  foreach(rm ${IEEE754_ROUNDING_MODES})
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(conf ${CMAKE_CONFIGURATION_TYPES})
	set(test "aster${test_arg}_${rm}_${conf}_mtest")
	set(file "aster${test_arg}-${rm}-${conf}.mtest")
	get_property(MFrontAsterBehavioursBuildPath TARGET MFrontAsterBehaviours PROPERTY LOCATION_${conf})
	if(HAVE_CASTEM)
	  get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
	endif(HAVE_CASTEM)
	configure_file(aster${test_arg}.mtest.in
	  ${file} @ONLY)
	if(_NO_XML_OUTPUT)
	  add_test(NAME ${test}
	    CONFIGURATIONS ${conf}
	    COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=false --result-file-output=false ${file})
	else(_NO_XML_OUTPUT)
	  add_test(NAME ${test}
	    CONFIGURATIONS ${conf}
	    COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false ${file})
	endif(_NO_XML_OUTPUT)
	if(_WILL_FAIL)
	  set_tests_properties(${test}
	    PROPERTIES WILL_FAIL true)
	endif(_WILL_FAIL)
	if((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS MFrontAsterBehaviours MFrontMaterialProperties-aster mtest
	    PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
	elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS MFrontAsterBehaviours MFrontMaterialProperties-castem mtest)
	endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	if(TFEL_APPEND_SUFFIX)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/aster"
	    COMPONENT mtest)
	else(TFEL_APPEND_SUFFIX)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront/tests/behaviours/aster"
	    COMPONENT mtest)
	endif(TFEL_APPEND_SUFFIX)
      endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
    else(CMAKE_CONFIGURATION_TYPES)
      set(test "aster${test_arg}_${rm}_mtest")
      set(file "aster${test_arg}-${rm}.mtest")
      get_property(MFrontAsterBehavioursBuildPath TARGET MFrontAsterBehaviours PROPERTY LOCATION)
      if(HAVE_CASTEM)
	get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)
      endif(HAVE_CASTEM)
      configure_file(aster${test_arg}.mtest.in ${file} @ONLY)
      if(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest  --rounding-direction-mode=${rm} --verbose=level0 --xml-output=false --result-file-output=false ${file})
      else(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest  --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false ${file})
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(${test}
	  PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      set_tests_properties(${test}
	PROPERTIES DEPENDS MFrontAsterBehaviours)
      set_tests_properties(${test}
	PROPERTIES DEPENDS mtest)
      if(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/aster"
	  COMPONENT mtest)
      else(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/aster"
	  COMPONENT mtest)
      endif(TFEL_APPEND_SUFFIX)
    endif(CMAKE_CONFIGURATION_TYPES)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endmacro(test_aster)

macro(test_aster_python test)
  configure_file(aster${test}.py.in
    aster${test}.py @ONLY)
  add_test(NAME aster${test}_py
    COMMAND ${PYTHON_EXECUTABLE} aster${test}.py)
  set_tests_properties(aster${test}_py
    PROPERTIES DEPENDS MFrontAsterBehaviours)
  if(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(aster${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python;$<TARGET_FILE_DIR:py_std__stl_vector>;$<TARGET_FILE_DIR:py_tfel_tests>")
  else(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(aster${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python")
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_aster_python)

test_aster(materialpropertiesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_aster(materialpropertiesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_aster(materialpropertiesboundscheck3)
test_aster(materialpropertiesboundscheck4)
test_aster(materialpropertiesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_aster(materialpropertiesboundscheck6)

test_aster(externalstatevariablesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_aster(externalstatevariablesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_aster(externalstatevariablesboundscheck3)
test_aster(externalstatevariablesboundscheck4)
test_aster(externalstatevariablesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)

test_aster(elasticity)
test_aster(elasticity2)
if(HAVE_CASTEM)
  test_aster(elasticity3)
  foreach(rm ${IEEE754_ROUNDING_MODES})
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(conf ${CMAKE_CONFIGURATION_TYPES})
	set_tests_properties(asterelasticity3_${rm}_${conf}_mtest
	  PROPERTIES DEPENDS MFrontMaterialProperties-castem)
      endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
    else(CMAKE_CONFIGURATION_TYPES)
      set_tests_properties(asterelasticity3_${rm}_mtest
	PROPERTIES DEPENDS MFrontMaterialProperties-castem)
    endif(CMAKE_CONFIGURATION_TYPES)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endif(HAVE_CASTEM)
test_aster(elasticity4)
test_aster(elasticity5)
test_aster(elasticity6)
test_aster(elasticity7)
test_aster(elasticity8)
test_aster(elasticity9)
test_aster(elasticity10)
test_aster(elasticity11)
test_aster(elasticity12)
test_aster(elasticity13)
test_aster(elasticity14)
test_aster(elasticity15)
test_aster(elasticity16)
test_aster(elasticity18)
test_aster(elasticity19)
test_aster(elasticity20)
test_aster(elasticity21)
test_aster(elasticity22)
test_aster(elasticity23)
test_aster(elasticity33)
test_aster(druckerprager)
test_aster(druckerprager2)
test_aster(mazars)
test_aster(mazars2)
test_aster(unilateralmazars)
test_aster(lorentz)
test_aster(lorentz2)
test_aster(norton)
test_aster(norton2)
test_aster(stressrelaxation)
# test_aster(norton-euler)
# test_aster(norton-rk2)
# test_aster(norton-rk4)
test_aster(orthotropiccreep)
test_aster(orthotropiccreep-rk42)
test_aster(orthotropiccreep-rkCastem)
test_aster(implicitnorton)
test_aster(implicitnorton2)
test_aster(implicitnorton3)
test_aster(implicitnorton_broyden)
test_aster(implicitnorton_broyden2)
test_aster(implicitorthotropiccreep)
test_aster(implicitnorton_levenbergmarquardt)
test_aster(implicitnorton_numericallycomputedjacobianblocks)
test_aster(plasticity)
test_aster(ellipticcreep)
test_aster(strainhardeningcreep)
test_aster(plasisocin)
test_aster(viscoplasisocin)
test_aster(chaboche)
test_aster(chaboche2)
test_aster(viscochaboche)
test_aster(monocrystal)
test_aster(monocrystal-2)
test_aster(implicitmonocrystal)
test_aster(implicitmonocrystal-2)
test_aster(monocrystal_dd_cfc)
test_aster(monocrystal_dd_cfc_numericaljacobian)
test_aster(monocrystal_dd_cfc_irradiation)
test_aster(monocrystal_dd_cc)
test_aster(monocrystal_dd_cc_numericaljacobian)
test_aster(monocrystal_dd_cc_irradiation)
test_aster(monocrystal_dd_cc_irradiation_numericaljacobian)
test_aster(polycrystals)
test_aster(polycrystals_dd_cc)
test_aster(polycrystals_dd_cfc)
test_aster(implicithayhurst)
test_aster(burger)
test_aster(ageingburger)
test_aster(gursontvergaardneedlemanplasticflow)
test_aster(gursontvergaardneedlemanviscoplasticflow)
test_aster(anistropiclemaitreviscoplasticbehaviour)
test_aster(tvergaard)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig11-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig22-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig33-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-1-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-2-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-3-2D)
test_aster(saintvenantkirchhoffelasticity-shear-1-2D)
test_aster(saintvenantkirchhoffelasticity-shear-2-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_aster(saintvenantkirchhoffelasticity-shear-1-3D)
test_aster(saintvenantkirchhoffelasticity-shear-2-3D)
test_aster(saintvenantkirchhoffelasticity-shear-3-3D)
test_aster(saintvenantkirchhoffelasticity-shear-4-3D)
test_aster(saintvenantkirchhoffelasticity-shear-5-3D)
test_aster(saintvenantkirchhoffelasticity-shear-6-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig11-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig22-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig33-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig11-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig22-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig33-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-1-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-2-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-3-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-1-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-2-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-1-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-2-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-3-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-1-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-2-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-3-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-4-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-5-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-6-3D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-1-2D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-2-2D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-3-2D)
test_aster(saintvenantkirchhoffelasticity2-shear-1-2D)
test_aster(saintvenantkirchhoffelasticity2-shear-2-2D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-1-3D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-2-3D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-3-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-1-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-2-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-3-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-4-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-5-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-6-3D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-1-2D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-2-2D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-3-2D)
test_aster(saintvenantkirchhoffelasticity3-shear-1-2D)
test_aster(saintvenantkirchhoffelasticity3-shear-2-2D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-1-3D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-2-3D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-3-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-1-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-2-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-3-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-4-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-5-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-6-3D)
test_aster(finitestrainsinglecrystal-001)
test_aster(finitestrainsinglecrystal-012)
test_aster(finitestrainsinglecrystal-012-2)
test_aster(finitestrainsinglecrystal-159)
test_aster(finitestrainsinglecrystal_numericaljacobian-001)
test_aster(finitestrainsinglecrystal_numericaljacobian-012)
test_aster(finitestrainsinglecrystal_numericaljacobian-012-2)
test_aster(finitestrainsinglecrystal_numericaljacobian-159)
test_aster(computestressfreeexpansion)
test_aster(isotropiclinearswelling)
test_aster(isotropicvolumeswelling)
test_aster(orthotropicswelling)
test_aster(orthotropicswelling2)
test_aster(orthotropicswelling3)
test_aster(axialgrowth)
test_aster(axialgrowth2)
test_aster(rusinekklepaczko_ssrt)
test_aster(johnsoncook_ssrt)
test_aster(parametertest)
test_aster(parametertest2)

if(enable-python-bindings)
  test_aster_python(xx)
  test_aster_python(viscochaboche-memo-nrad)
endif(enable-python-bindings)
