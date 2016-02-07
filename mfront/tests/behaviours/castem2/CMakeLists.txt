if(CASTEMHOME)
  include_directories(
    "${CASTEMHOME}/include"
    "${CASTEMHOME}/include/c")
endif(CASTEMHOME)

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

macro(test_castem2 test_arg)
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "castem2${test_arg}-${conf}.mtest")
      get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
      if(HAVE_CASTEM)
	get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      endif(HAVE_CASTEM)
      configure_file(castem2${test_arg}.mtest.in
	${file} @ONLY)
      add_test(NAME castem2${test_arg}_${conf}_mtest
	CONFIGURATIONS ${conf}
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST castem2${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST castem2${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	DESTINATION "share/mfront/tests/behaviours/castem2"
	CONFIGURATIONS ${conf}
	COMPONENT mtest)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    if(HAVE_CASTEM)
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)
    endif(HAVE_CASTEM)
    configure_file(castem2${test_arg}.mtest.in
      castem2${test_arg}.mtest @ONLY)
    add_test(NAME castem2${test_arg}_mtest
      COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false castem2${test_arg}.mtest)
    set_tests_properties(castem2${test_arg}_mtest
      PROPERTIES DEPENDS MFrontCastemBehaviours)
    set_tests_properties(castem2${test_arg}_mtest
      PROPERTIES DEPENDS mtest)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/castem2${test_arg}.mtest
      DESTINATION "share/mfront/tests/behaviours/castem2"
      COMPONENT mtest)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_castem2)

test_castem2(elasticity)
test_castem2(elasticity2)
if(HAVE_CASTEM)
  test_castem2(elasticity3)
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set_tests_properties(castem2elasticity3_${conf}_mtest
	PROPERTIES DEPENDS MFrontMaterialProperties-castem)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(castem2elasticity3_mtest
      PROPERTIES DEPENDS MFrontMaterialProperties-castem)
  endif(CMAKE_CONFIGURATION_TYPES)
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
test_castem2(monocrystal)
test_castem2(monocrystal-2)
test_castem2(implicitmonocrystal)
test_castem2(implicitmonocrystal-2)
test_castem2(monocrystal_dd_cfc)
test_castem2(monocrystal_dd_cfc_numericaljacobian)
test_castem2(monocrystal_dd_cfc_irradiation)
test_castem2(monocrystal_dd_cc)
test_castem2(monocrystal_dd_cc_numericaljacobian)
test_castem2(monocrystal_dd_cc_irradiation_numericaljacobian)
test_castem2(implicithayhurst)
test_castem2(gursontvergaardneedlemanplasticflow)
test_castem2(gursontvergaardneedlemanviscoplasticflow)
test_castem2(ellipticcreep)
test_castem2(burger)
test_castem2(tssf)
