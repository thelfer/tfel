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
  Elasticity
  Elasticity2
  Elasticity3
  OrthotropicElastic
  OrthotropicElastic2
  JohnsonCook_s
  JohnsonCook_ssr
  JohnsonCook_ssrt
  RusinekKlepaczko_ssrt
  LogarithmicStrainElasticity
  LogarithmicStrainElasticity2
  LogarithmicStrainNortonCreep
  SaintVenantKirchhoffElasticity
  FiniteStrainSingleCrystal)
mfront_dependencies(MFrontCalculiXBehaviours
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)

mfront_behaviour_check_library(MFrontCalculiXBehaviours calculix ${mfront_tests_SOURCES})
target_link_libraries(MFrontCalculiXBehaviours
  CalculiXInterface
  MFrontProfiling
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontCalculiXBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

macro(test_calculix test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "test_calculix: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  foreach(rm ${IEEE754_ROUNDING_MODES})
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(conf ${CMAKE_CONFIGURATION_TYPES})
	set(file "calculix${test_arg}-${conf}.mtest")
	set(test "calculix${test_arg}_${rm}_${conf}_mtest")
	get_property(MFrontCalculiXBehavioursBuildPath TARGET MFrontCalculiXBehaviours PROPERTY LOCATION_${conf})
	if(HAVE_CASTEM)
	  get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
	endif(HAVE_CASTEM)
	configure_file(calculix${test_arg}.mtest.in ${file} @ONLY)
	if(_NO_XML_OUTPUT)
	  add_test(NAME ${test} CONFIGURATIONS ${conf}
	    COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=false --result-file-output=false ${file})
	else(_NO_XML_OUTPUT)
	  add_test(NAME ${test} CONFIGURATIONS ${conf}
	    COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false ${file})
	endif(_NO_XML_OUTPUT)
	if(_WILL_FAIL)
	  set_tests_properties(${test} PROPERTIES WILL_FAIL true)
	endif(_WILL_FAIL)
	if((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS MFrontCalculiXBehaviours MFrontMaterialProperties-calculix mtest
	    PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
	elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS MFrontCalculiXBehaviours MFrontMaterialProperties-castem mtest)
	endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	if(TFEL_APPEND_SUFFIX)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/calculix"
	    COMPONENT mtest)
	else(TFEL_APPEND_SUFFIX)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront/tests/behaviours/calculix"
	    COMPONENT mtest)
	endif(TFEL_APPEND_SUFFIX)
      endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
    else(CMAKE_CONFIGURATION_TYPES)
      set(file "calculix${test_arg}.mtest")
      set(test "calculix${test_arg}_${rm}_mtest")
      get_property(MFrontCalculiXBehavioursBuildPath TARGET MFrontCalculiXBehaviours PROPERTY LOCATION)
      if(HAVE_CASTEM)
	get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)
      endif(HAVE_CASTEM)
      configure_file(calculix${test_arg}.mtest.in	${file} @ONLY)
      if(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest  --rounding-direction-mode=${rm} --verbose=level0 --xml-output=false --result-file-output=false ${file})
      else(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest  --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false ${file})
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(${test} PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      set_tests_properties(${test} PROPERTIES DEPENDS MFrontCalculiXBehaviours)
      set_tests_properties(${test} PROPERTIES DEPENDS mtest)
      if(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/calculix"
	  COMPONENT mtest)
      else(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/calculix"
	  COMPONENT mtest)
      endif(TFEL_APPEND_SUFFIX)
    endif(CMAKE_CONFIGURATION_TYPES)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endmacro(test_calculix)

test_calculix(elasticity)
test_calculix(elasticity2)
test_calculix(elasticity3)
test_calculix(elasticity4)
test_calculix(elasticity5)
test_calculix(elasticity6)
test_calculix(elasticity7)
test_calculix(elasticity8)
test_calculix(elasticity9)
test_calculix(elasticity10)
test_calculix(elasticity11)
test_calculix(elasticity14)
test_calculix(elasticity15)
test_calculix(elasticity18)
test_calculix(elasticity19)
test_calculix(elasticity20)
test_calculix(elasticity21)
test_calculix(elasticity22)
test_calculix(elasticity23)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-1-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-2-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-3-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-4-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-5-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-6-3D)

test_calculix(finitestrainsinglecrystal-001)
test_calculix(finitestrainsinglecrystal-012-2)
test_calculix(finitestrainsinglecrystal-012)
test_calculix(finitestrainsinglecrystal-159)

test_calculix(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_calculix(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_calculix(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_calculix(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_calculix(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_calculix(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_calculix(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_calculix(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_calculix(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
