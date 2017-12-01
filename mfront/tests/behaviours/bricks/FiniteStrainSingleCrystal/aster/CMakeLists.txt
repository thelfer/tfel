include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

if(HAVE_ASTER)
  mfront_behaviour_brick_check_library(MFrontAsterFiniteStrainSingleCrystalBrickBehaviours
    FiniteStrainSingleCrystal aster
    FiniteStrainSingleCrystal
    FiniteStrainSingleCrystal_NumericalJacobian
    )
  target_link_libraries(MFrontAsterFiniteStrainSingleCrystalBrickBehaviours
    AsterInterface MFrontProfiling TFELMaterial
    TFELMath TFELUtilities TFELException)
endif(HAVE_ASTER)

macro(astertest_finitestrainsinglecrystalbrick test_arg behaviour)
  if(HAVE_ASTER)
    set(_REFERENCE_FILE )
    if(NOT (${ARGC} EQUAL 0))
      set(_REFERENCE_FILE "${ARGN}")
    endif()
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
    if(NOT _REFERENCE_FILE)
      add_test(NAME brick${test_arg}${behaviour}_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
	--@library@="$<TARGET_FILE:MFrontAsterFiniteStrainSingleCrystalBrickBehaviours>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}.xml" ${file})
    else(NOT _REFERENCE_FILE)
      add_test(NAME brick${test_arg}_${behaviour}_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
	--@library@="$<TARGET_FILE:MFrontAsterFiniteStrainSingleCrystalBrickBehaviours>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}.xml"
	--@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}" ${file})
    endif(NOT _REFERENCE_FILE)
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST brick${test_arg}_${behaviour}_mtest
	PROPERTY DEPENDS "MFrontAsterBehaviours MFrontMaterialProperties-aster mtest"
	PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
    elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST brick${test_arg}${behaviour}_mtest
	PROPERTY DEPENDS "MFrontAsterBehaviours MFrontMaterialProperties-aster mtest")
      set_property(TEST brick${test_arg}${behaviour}_consistenttangentoperator_mtest
	PROPERTY DEPENDS "MFrontAsterBehaviours MFrontMaterialProperties-aster mtest")
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/bricks/FiniteStrainSingleCrystal/aster/"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront/tests/behaviours/bricks/FiniteStrainSingleCrystal/aster/"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endif(HAVE_ASTER)
endmacro(astertest_finitestrainsinglecrystalbrick)

astertest_finitestrainsinglecrystalbrick(finitestrainsinglecrystal-012
  asterfinitestrainsinglecrystal finitestrainsinglecrystal-012.ref)
astertest_finitestrainsinglecrystalbrick(finitestrainsinglecrystal-159
  asterfinitestrainsinglecrystal finitestrainsinglecrystal-159.ref)
astertest_finitestrainsinglecrystalbrick(finitestrainsinglecrystal-012
  asterfinitestrainsinglecrystal_numericaljacobian finitestrainsinglecrystal-012.ref)
astertest_finitestrainsinglecrystalbrick(finitestrainsinglecrystal-159
  asterfinitestrainsinglecrystal_numericaljacobian finitestrainsinglecrystal-159.ref)
