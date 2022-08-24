set(mfront_tests_SOURCES
  TensorialExternalStateVariableTest
  InitializationFailureTest
  SaintVenantKirchhoffElasticity
  SaintVenantKirchhoffElasticity2
  SaintVenantKirchhoffElasticity3
  FiniteRotationSmallStrainElasticity
  LogarithmicStrainElasticity
  LogarithmicStrainElasticity2
  LogarithmicStrainNortonCreep
  OrthotropicSwelling
  OrthotropicSwelling2
  AxialGrowth)

mfront_behaviour_check_library(MFrontEuroplexusBehaviours
  epx ${mfront_tests_SOURCES})
target_link_libraries(MFrontEuroplexusBehaviours
  PRIVATE EuroplexusInterface)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontEuroplexusBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

macro(test_epx test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "test_epx: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  foreach(rm ${IEEE754_ROUNDING_MODES})
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(conf ${CMAKE_CONFIGURATION_TYPES})
	set(file "epx${test_arg}-${rm}-${conf}.mtest")
	set(test "epx${test_arg}_${rm}_${conf}_mtest")
	get_property(MFrontEuroplexusBehavioursBuildPath TARGET MFrontEuroplexusBehaviours PROPERTY LOCATION_${conf})
	if(HAVE_CASTEM)
	  get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
	endif(HAVE_CASTEM)
	configure_file(epx${test_arg}.mtest.in ${file} @ONLY)
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
	  set_tests_properties(${test}  PROPERTIES WILL_FAIL true)
	endif(_WILL_FAIL)
	if((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS MFrontEuroplexusBehaviours MFrontMaterialProperties-europlexus mtest
	    PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
	elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}  PROPERTY DEPENDS MFrontEuroplexusBehaviours MFrontMaterialProperties-castem mtest)
	endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	if(TFEL_APPEND_SUFFIX)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/europlexus"
	    COMPONENT mtest)
	else(TFEL_APPEND_SUFFIX)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront/tests/behaviours/europlexus"
	    COMPONENT mtest)
	endif(TFEL_APPEND_SUFFIX)
      endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
    else(CMAKE_CONFIGURATION_TYPES)
      set(file  epx${test_arg}-${rm}.mtest)
      set(test  epx${test_arg}_${rm}_mtest)
      get_property(MFrontEuroplexusBehavioursBuildPath TARGET MFrontEuroplexusBehaviours PROPERTY LOCATION)
      if(HAVE_CASTEM)
	get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)
      endif(HAVE_CASTEM)
      configure_file(epx${test_arg}.mtest.in ${file} @ONLY)
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
      set_tests_properties(${test} PROPERTIES DEPENDS MFrontEuroplexusBehaviours)
      set_tests_properties(${test} PROPERTIES DEPENDS mtest)
      if(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/europlexus"
	  COMPONENT mtest)
      else(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/europlexus"
	  COMPONENT mtest)
      endif(TFEL_APPEND_SUFFIX)
    endif(CMAKE_CONFIGURATION_TYPES)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endmacro(test_epx)

test_epx(initializationfailuretest
  NO_XML_OUTPUT WILL_FAIL)

test_epx(saintvenantkirchhoffelasticity-uniaxialloading-sig11-2D)
test_epx(saintvenantkirchhoffelasticity-uniaxialloading-sig22-2D)
test_epx(saintvenantkirchhoffelasticity-uniaxialloading-sig33-2D)
test_epx(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_epx(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_epx(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)
test_epx(saintvenantkirchhoffelasticity-uniaxialloading-1-2D)
test_epx(saintvenantkirchhoffelasticity-uniaxialloading-2-2D)
test_epx(saintvenantkirchhoffelasticity-uniaxialloading-3-2D)
test_epx(saintvenantkirchhoffelasticity-shear-1-2D)
test_epx(saintvenantkirchhoffelasticity-shear-2-2D)
test_epx(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_epx(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_epx(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_epx(saintvenantkirchhoffelasticity-shear-1-3D)
test_epx(saintvenantkirchhoffelasticity-shear-2-3D)
test_epx(saintvenantkirchhoffelasticity-shear-3-3D)
test_epx(saintvenantkirchhoffelasticity-shear-4-3D)
test_epx(saintvenantkirchhoffelasticity-shear-5-3D)
test_epx(saintvenantkirchhoffelasticity-shear-6-3D)
test_epx(saintvenantkirchhoffelasticity2-uniaxialloading-1-2D)
test_epx(saintvenantkirchhoffelasticity2-uniaxialloading-2-2D)
test_epx(saintvenantkirchhoffelasticity2-uniaxialloading-3-2D)
test_epx(saintvenantkirchhoffelasticity2-shear-1-2D)
test_epx(saintvenantkirchhoffelasticity2-shear-2-2D)
test_epx(saintvenantkirchhoffelasticity2-uniaxialloading-1-3D)
test_epx(saintvenantkirchhoffelasticity2-uniaxialloading-2-3D)
test_epx(saintvenantkirchhoffelasticity2-uniaxialloading-3-3D)
test_epx(saintvenantkirchhoffelasticity2-shear-1-3D)
test_epx(saintvenantkirchhoffelasticity2-shear-2-3D)
test_epx(saintvenantkirchhoffelasticity2-shear-3-3D)
test_epx(saintvenantkirchhoffelasticity2-shear-4-3D)
test_epx(saintvenantkirchhoffelasticity2-shear-5-3D)
test_epx(saintvenantkirchhoffelasticity2-shear-6-3D)
test_epx(saintvenantkirchhoffelasticity3-uniaxialloading-1-2D)
test_epx(saintvenantkirchhoffelasticity3-uniaxialloading-2-2D)
test_epx(saintvenantkirchhoffelasticity3-uniaxialloading-3-2D)
test_epx(saintvenantkirchhoffelasticity3-shear-1-2D)
test_epx(saintvenantkirchhoffelasticity3-shear-2-2D)
test_epx(saintvenantkirchhoffelasticity3-uniaxialloading-1-3D)
test_epx(saintvenantkirchhoffelasticity3-uniaxialloading-2-3D)
test_epx(saintvenantkirchhoffelasticity3-uniaxialloading-3-3D)
test_epx(saintvenantkirchhoffelasticity3-shear-1-3D)
test_epx(saintvenantkirchhoffelasticity3-shear-2-3D)
test_epx(saintvenantkirchhoffelasticity3-shear-3-3D)
test_epx(saintvenantkirchhoffelasticity3-shear-4-3D)
test_epx(saintvenantkirchhoffelasticity3-shear-5-3D)
test_epx(saintvenantkirchhoffelasticity3-shear-6-3D)

test_epx(finiterotationsmallstrainelasticity-uniaxialloading-1-2D)
test_epx(finiterotationsmallstrainelasticity-uniaxialloading-2-2D)
test_epx(finiterotationsmallstrainelasticity-uniaxialloading-3-2D)
test_epx(finiterotationsmallstrainelasticity-shear-1-2D)
test_epx(finiterotationsmallstrainelasticity-shear-2-2D)
test_epx(finiterotationsmallstrainelasticity-uniaxialloading-1-3D)
test_epx(finiterotationsmallstrainelasticity-uniaxialloading-2-3D)
test_epx(finiterotationsmallstrainelasticity-uniaxialloading-3-3D)
test_epx(finiterotationsmallstrainelasticity-shear-1-3D)
test_epx(finiterotationsmallstrainelasticity-shear-2-3D)
test_epx(finiterotationsmallstrainelasticity-shear-3-3D)
test_epx(finiterotationsmallstrainelasticity-shear-4-3D)
test_epx(finiterotationsmallstrainelasticity-shear-5-3D)
test_epx(finiterotationsmallstrainelasticity-shear-6-3D)

test_epx(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D)
test_epx(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_epx(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D)
test_epx(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_epx(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D)
test_epx(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_epx(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D)
test_epx(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_epx(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D)
test_epx(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_epx(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D)
test_epx(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_epx(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D)
test_epx(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_epx(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D)
test_epx(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_epx(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D)
test_epx(orthotropicswelling)
test_epx(orthotropicswelling2)
test_epx(axialgrowth)
test_epx(tensorialexternalstatevariabletest)
