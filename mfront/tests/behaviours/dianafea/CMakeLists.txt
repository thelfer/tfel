  mfront_behaviour_check_library(MFrontDianaFEABehaviours dianafea
  MaterialPropertiesBoundsCheck
  StateVariablesBoundsCheck
  Elasticity
  Plasticity
  DDIF2
  HosfordPerfectPlasticity
  DruckerPrager
  DruckerPragerEcroLin
  DruckerPragerEcroLinDefault
  Norton
  ImplicitNorton
  Mazars
  ComputeThermalExpansion
  ParameterTest
  ParameterTest2
  )
target_link_libraries(MFrontDianaFEABehaviours
  PRIVATE DianaFEAInterface)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontDianaFEABehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

function(test_dianafea test_arg)
  if(HAVE_DIANAFEA)
    set(_XML_OUTPUT "true")
    set(_REFERENCE_FILE )
    foreach(_ARG ${ARGN})
      if ( ${_ARG} MATCHES NO_XML_OUTPUT )
	set ( _XML_OUTPUT "false")
      elseif ( ${_ARG} MATCHES WILL_FAIL)
	set ( _WILL_FAIL ON)
      else()
        set(_REFERENCE_FILE "${_ARG}")
      endif()
    endforeach(_ARG ${ARGN})
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
    foreach(rm ${IEEE754_ROUNDING_MODES})
      if(NOT _REFERENCE_FILE)
		if(HAVE_CASTEM)
        add_test(NAME dianafea-${test_arg}_${rm}_mtest
		  	  COMMAND mtest --rounding-direction-mode=${rm}
	          --verbose=level0 --xml-output=${_XML_OUTPUT}
	          --result-file-output=false
	          --@library@="$<TARGET_FILE:MFrontDianaFEABehaviours>"
			  --@mplibrary@="$<TARGET_FILE:MFrontMaterialProperties-castem>"
	          --@xml_output@="${test_arg}-${rm}.xml" ${file})
		else(HAVE_CASTEM)
	        add_test(NAME dianafea-${test_arg}_${rm}_mtest
		  	  COMMAND mtest --rounding-direction-mode=${rm}
	          --verbose=level0 --xml-output=${_XML_OUTPUT}
	          --result-file-output=false
	          --@library@="$<TARGET_FILE:MFrontDianaFEABehaviours>"
	          --@xml_output@="${test_arg}-${rm}.xml" ${file})
			endif(HAVE_CASTEM)
      else(NOT _REFERENCE_FILE)
        set(rfile "${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}")
		if(HAVE_CASTEM)
	        add_test(NAME dianafea-${test_arg}_${rm}_mtest
	          COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=${_XML_OUTPUT} --result-file-output=false
	          --@library@="$<TARGET_FILE:MFrontDianaFEABehaviours>"
			  --@mplibrary@="$<TARGET_FILE:MFrontMaterialProperties-castem>"
	          --@xml_output@="${test_arg}-${rm}.xml"
	          --@reference_file@="${rfile}" ${file})
		else(HAVE_CASTEM)
	        add_test(NAME dianafea-${test_arg}_${rm}_mtest
	          COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=${_XML_OUTPUT} --result-file-output=false
	          --@library@="$<TARGET_FILE:MFrontDianaFEABehaviours>"
	          --@xml_output@="${test_arg}-${rm}.xml"
	          --@reference_file@="${rfile}" ${file})
		endif(HAVE_CASTEM)
      endif(NOT _REFERENCE_FILE)
      if(_WILL_FAIL)
	set_tests_properties(dianafea-${test_arg}_${rm}_mtest
	  PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST dianafea-${test_arg}_${rm}_mtest
          PROPERTY DEPENDS "MFrontDianaFEABehaviours MFrontMaterialProperties-castem mtest"
          PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST dianafea-${test_arg}_${rm}_mtest
          PROPERTY DEPENDS "MFrontDianaFEABehaviours MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    endforeach(rm ${IEEE754_ROUNDING_MODES})
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/bricks/StandardElastoViscoPlasticity/castem"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront/tests/behaviours/bricks/StandardElastoViscoPlasticity/castem"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endif(HAVE_DIANAFEA)
endfunction(test_dianafea)

test_dianafea(elasticity)
test_dianafea(plasticity plasticity.ref)
test_dianafea(mazars mazars-aster.ref)
test_dianafea(mazars2 mazars-aster.ref)
test_dianafea(druckerprager druckerprager.ref)
test_dianafea(druckerprager2 druckerprager2.ref)
test_dianafea(norton)
test_dianafea(implicitnorton)
test_dianafea(computethermalexpansion)
test_dianafea(ddif2 ddif2-uniaxialtesting.ref)
test_dianafea(parametertest)
test_dianafea(parametertest2)

test_dianafea(materialpropertiesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_dianafea(materialpropertiesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_dianafea(materialpropertiesboundscheck3)
test_dianafea(materialpropertiesboundscheck4)
test_dianafea(materialpropertiesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_dianafea(materialpropertiesboundscheck6)
test_dianafea(materialpropertiesboundscheck7)
test_dianafea(materialpropertiesboundscheck8 
  NO_XML_OUTPUT WILL_FAIL)

foreach(rm ${IEEE754_ROUNDING_MODES})
    set_property(TEST dianafea-materialpropertiesboundscheck6_${rm}_mtest
      APPEND PROPERTY ENVIRONMENT "DIANAFEA_OUT_OF_BOUNDS_POLICY=NONE")
    set_property(TEST dianafea-materialpropertiesboundscheck7_${rm}_mtest
      APPEND PROPERTY ENVIRONMENT "DIANAFEA_OUT_OF_BOUNDS_POLICY=WARNING")
    set_property(TEST dianafea-materialpropertiesboundscheck8_${rm}_mtest
      APPEND PROPERTY ENVIRONMENT "DIANAFEA_OUT_OF_BOUNDS_POLICY=STRICT")
    set_property(TEST dianafea-materialpropertiesboundscheck_${rm}_mtest
      dianafea-materialpropertiesboundscheck2_${rm}_mtest
      dianafea-materialpropertiesboundscheck5_${rm}_mtest
      dianafea-materialpropertiesboundscheck8_${rm}_mtest
      PROPERTY WILL_FAIL true)
endforeach(rm ${IEEE754_ROUNDING_MODES})
  
test_dianafea(statevariablesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_dianafea(statevariablesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
