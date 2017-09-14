include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
  "${PROJECT_BINARY_DIR}/src/Material"
  "${PROJECT_BINARY_DIR}/src/Math"
  "${PROJECT_BINARY_DIR}/src/Utilities"
  "${PROJECT_BINARY_DIR}/src/Exception")

set(mfront_tests_SOURCES
  MaterialPropertiesBoundsCheck
  Elasticity
  OrthotropicElastic
  JohnsonCook_s
  JohnsonCook_ssr
  JohnsonCook_ssrt
  RusinekKlepaczko_ssrt)
mfront_dependencies(MFrontAnsysBehaviours
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)

mfront_behaviour_check_library(MFrontAnsysBehaviours  ansys ${mfront_tests_SOURCES})
target_link_libraries(MFrontAnsysBehaviours
  AnsysInterface
  MFrontProfiling
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontAnsysBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

macro(test_ansys test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "test_ansys: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  foreach(rm ${IEEE754_ROUNDING_MODES})
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(conf ${CMAKE_CONFIGURATION_TYPES})
	set(file "ansys${test_arg}-${rm}-${conf}.mtest")
	set(test "ansys${test_arg}_${rm}_${conf}_mtest")
	get_property(MFrontAnsysBehavioursBuildPath TARGET MFrontAnsysBehaviours PROPERTY LOCATION_${conf})
	if(HAVE_CASTEM)
	  get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
	endif(HAVE_CASTEM)
	configure_file(ansys${test_arg}.mtest.in ${file} @ONLY)
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
            PROPERTY DEPENDS MFrontAnsysBehaviours MFrontMaterialProperties-ansys mtest
	    PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
	elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS MFrontAnsysBehaviours MFrontMaterialProperties-castem mtest)
	endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	if(TFEL_APPEND_VERSION)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront-${TFEL_VERSION}/tests/behaviours/ansys"
	    COMPONENT mtest)
	else(TFEL_APPEND_VERSION)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront/tests/behaviours/ansys"
	    COMPONENT mtest)
	endif(TFEL_APPEND_VERSION)
      endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
    else(CMAKE_CONFIGURATION_TYPES)
      set(file "ansys${test_arg}-${rm}.mtest")
      set(test "ansys${test_arg}_${rm}_mtest")
      get_property(MFrontAnsysBehavioursBuildPath TARGET MFrontAnsysBehaviours PROPERTY LOCATION)
      if(HAVE_CASTEM)
	get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)
      endif(HAVE_CASTEM)
      configure_file(ansys${test_arg}.mtest.in ${file} @ONLY)
      if(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=false --result-file-output=false ${file})
      else(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false ${file})
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(${test} PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      set_tests_properties(${test} PROPERTIES DEPENDS MFrontAnsysBehaviours)
      set_tests_properties(${test} PROPERTIES DEPENDS mtest)
      if(TFEL_APPEND_VERSION)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_VERSION}/tests/behaviours/ansys"
	  COMPONENT mtest)
      else(TFEL_APPEND_VERSION)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/ansys"
	  COMPONENT mtest)
      endif(TFEL_APPEND_VERSION)
    endif(CMAKE_CONFIGURATION_TYPES)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endmacro(test_ansys)

test_ansys(materialpropertiesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_ansys(materialpropertiesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_ansys(materialpropertiesboundscheck3)
test_ansys(materialpropertiesboundscheck4)
test_ansys(materialpropertiesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_ansys(materialpropertiesboundscheck6)

test_ansys(elasticity)
test_ansys(elasticity2)
if(HAVE_CASTEM)
  test_ansys(elasticity3)
endif(HAVE_CASTEM)
test_ansys(elasticity4)
test_ansys(elasticity7)
test_ansys(elasticity15)
test_ansys(elasticity16)

test_ansys(elasticity10)
test_ansys(elasticity11)
test_ansys(elasticity12)
test_ansys(elasticity13)
test_ansys(elasticity14)
test_ansys(elasticity24)
test_ansys(elasticity25)
test_ansys(elasticity26)
test_ansys(elasticity27)
test_ansys(elasticity33)
test_ansys(elasticity5)
test_ansys(elasticity6)
test_ansys(elasticity8)
test_ansys(elasticity9)
