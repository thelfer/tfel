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
  FiniteRotationSmallStrainElasticity
  SaintVenantKirchhoffElasticity
  SaintVenantKirchhoffElasticity2
  SaintVenantKirchhoffElasticity3
  LogarithmicStrainElasticity
  LogarithmicStrainElasticity2
  LogarithmicStrainNortonCreep
  OrthotropicSwelling
  OrthotropicSwelling2
  AxialGrowth
  AbaqusAxialGrowth
  AbaqusAxialGrowth2
  AbaqusOrthotropicSwelling
  AbaqusOrthotropicSwelling2
  ImplicitSimoMieheElastoPlasticity)

mfront_behaviour_check_library(MFrontAbaqusExplicitBehaviours
  abaqusexplicit ${mfront_tests_SOURCES})
target_link_libraries(MFrontAbaqusExplicitBehaviours
  AbaqusInterface
  MFrontProfiling
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontAbaqusExplicitBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

macro(test_abaqus_explicit test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "test_abaqus_explicit: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  foreach(rm ${IEEE754_ROUNDING_MODES})
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(conf ${CMAKE_CONFIGURATION_TYPES})
	set(file "abaqusexplicit${test_arg}-${rm}-${conf}.mtest")
	set(test "abaqusexplicit${test_arg}_${rm}_${conf}_mtest")
	get_property(MFrontAbaqusExplicitBehavioursBuildPath TARGET MFrontAbaqusExplicitBehaviours PROPERTY LOCATION_${conf})
	if(HAVE_CASTEM)
	  get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
	endif(HAVE_CASTEM)
	configure_file(abaqusexplicit${test_arg}.mtest.in	${file} @ONLY)
	if(_NO_XML_OUTPUT)
	  add_test(NAME ${test} CONFIGURATIONS ${conf}
	    COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false ${file})
	else(_NO_XML_OUTPUT)
	  add_test(NAME ${test} CONFIGURATIONS ${conf}
	    COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
	endif(_NO_XML_OUTPUT)
	if(_WILL_FAIL)
	  set_tests_properties(${test}
	    PROPERTIES WILL_FAIL true)
	endif(_WILL_FAIL)
	if((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS MFrontAbaqusExplicitBehaviours MFrontMaterialProperties-europlexus mtest
	    PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
	elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS MFrontAbaqusExplicitBehaviours MFrontMaterialProperties-castem mtest)
	endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/mfront/tests/behaviours/europlexus"
	  COMPONENT mtest)
      endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
    else(CMAKE_CONFIGURATION_TYPES)
      set(file "abaqusexplicit${test_arg}-${rm}.mtest")
      set(test "abaqusexplicit${test_arg}_${rm}_mtest")
      get_property(MFrontAbaqusExplicitBehavioursBuildPath TARGET MFrontAbaqusExplicitBehaviours PROPERTY LOCATION)
      if(HAVE_CASTEM)
	get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)
      endif(HAVE_CASTEM)
      configure_file(abaqusexplicit${test_arg}.mtest.in ${file} @ONLY)
      if(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false ${file})
      else(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(${test} PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      set_tests_properties(${test} PROPERTIES DEPENDS MFrontAbaqusExplicitBehaviours)
      set_tests_properties(${test} PROPERTIES DEPENDS mtest)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	DESTINATION "share/mfront/tests/behaviours/europlexus"
	COMPONENT mtest)
    endif(CMAKE_CONFIGURATION_TYPES)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endmacro(test_abaqus_explicit)

test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-sig11-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-sig22-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-sig33-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-1-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-2-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-3-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-shear-1-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-shear-2-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-shear-1-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-shear-2-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-shear-3-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-shear-4-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-shear-5-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity-shear-6-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-uniaxialloading-1-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-uniaxialloading-2-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-uniaxialloading-3-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-shear-1-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-shear-2-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-uniaxialloading-1-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-uniaxialloading-2-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-uniaxialloading-3-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-shear-1-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-shear-2-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-shear-3-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-shear-4-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-shear-5-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity2-shear-6-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-uniaxialloading-1-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-uniaxialloading-2-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-uniaxialloading-3-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-shear-1-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-shear-2-2D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-uniaxialloading-1-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-uniaxialloading-2-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-uniaxialloading-3-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-shear-1-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-shear-2-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-shear-3-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-shear-4-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-shear-5-3D)
test_abaqus_explicit(saintvenantkirchhoffelasticity3-shear-6-3D)

test_abaqus_explicit(finiterotationsmallstrainelasticity-uniaxialloading-1-2D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-uniaxialloading-2-2D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-uniaxialloading-3-2D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-shear-1-2D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-shear-2-2D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-uniaxialloading-1-3D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-uniaxialloading-2-3D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-uniaxialloading-3-3D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-shear-1-3D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-shear-2-3D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-shear-3-3D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-shear-4-3D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-shear-5-3D)
test_abaqus_explicit(finiterotationsmallstrainelasticity-shear-6-3D)

test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_abaqus_explicit(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D)
test_abaqus_explicit(orthotropicswelling)
test_abaqus_explicit(orthotropicswelling2)
test_abaqus_explicit(axialgrowth)
test_abaqus_explicit(implicitsimomieheelastoplasticity)
