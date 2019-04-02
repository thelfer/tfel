include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
  "${PROJECT_BINARY_DIR}/src/Glossary"
  "${PROJECT_BINARY_DIR}/src/Material"
  "${PROJECT_BINARY_DIR}/src/Math"
  "${PROJECT_BINARY_DIR}/src/Utilities"
  "${PROJECT_BINARY_DIR}/src/System"
  "${PROJECT_BINARY_DIR}/src/Tests"
  "${PROJECT_BINARY_DIR}/src/Exception")
  
mfront_behaviour_check_library(MFrontDianaFEABehaviours dianafea
  Elasticity
  Plasticity
  HosfordPerfectPlasticity
  DruckerPrager
  DruckerPragerEcroLin
  DruckerPragerEcroLinDefault
  Norton
  ImplicitNorton
  Mazars
  ComputeThermalExpansion
  )

target_link_libraries(MFrontDianaFEABehaviours
  DianaFEAInterface
  MFrontProfiling
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontDianaFEABehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

function(test_dianafea test_arg)
  if(HAVE_CASTEM)
    set(_REFERENCE_FILE )
    foreach(_ARG ${ARGN})
      if ( ${_ARG} MATCHES WILL_FAIL)
	set ( _WILL_FAIL ON)
      else()
        set(_REFERENCE_FILE "${_ARG}")
      endif()
    endforeach(_ARG ${ARGN})
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
    foreach(rm ${IEEE754_ROUNDING_MODES})
      if(NOT _REFERENCE_FILE)
        add_test(NAME dianafea-${test_arg}_${rm}_mtest
  	  COMMAND mtest --rounding-direction-mode=${rm}
          --verbose=level0 --xml-output=true
          --result-file-output=false
          --@library@="$<TARGET_FILE:MFrontDianaFEABehaviours>"
	  --@mplibrary@="$<TARGET_FILE:MFrontMaterialProperties-castem>"
          --@xml_output@="${test_arg}-${rm}.xml" ${file})
      else(NOT _REFERENCE_FILE)
        set(rfile "${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}")
        add_test(NAME dianafea-${test_arg}_${rm}_mtest
          COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
          --@library@="$<TARGET_FILE:MFrontDianaFEABehaviours>"
	  --@mplibrary@="$<TARGET_FILE:MFrontMaterialProperties-castem>"
          --@xml_output@="${test_arg}-${rm}.xml"
          --@reference_file@="${rfile}" ${file})
      endif(NOT _REFERENCE_FILE)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST dianafea-${test_arg}_${rm}_mtest
          PROPERTY DEPENDS "MFrontDianaFEABehaviours MFrontMaterialProperties-castem mtest"
          PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELPhysicalConstants>\;$ENV{PATH}")
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
  endif(HAVE_CASTEM)
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
