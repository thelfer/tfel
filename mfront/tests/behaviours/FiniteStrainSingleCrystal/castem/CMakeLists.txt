if(HAVE_CASTEM)
  mfront_behaviour_brick_check_library(MFrontCastemFiniteStrainSingleCrystalBrickBehaviours
    FiniteStrainSingleCrystal umat
    FiniteStrainSingleCrystal
    FiniteStrainSingleCrystal_NumericalJacobian
    )
  target_link_libraries(MFrontCastemFiniteStrainSingleCrystalBrickBehaviours
    PRIVATE CastemInterface)
endif(HAVE_CASTEM)

macro(castemtest_finitestrainsinglecrystalbrick test_arg behaviour)
  if(HAVE_CASTEM)
    set(_REFERENCE_FILE )
    if(NOT (${ARGC} EQUAL 0))
      set(_REFERENCE_FILE "${ARGN}")
    endif()
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
    if(NOT _REFERENCE_FILE)
      add_test(NAME brick${test_arg}_${behaviour}_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
    --@AccelerationAlgorithm='Cast3M'
	--@library@="$<TARGET_FILE:MFrontCastemFiniteStrainSingleCrystalBrickBehaviours>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}.xml" ${file})
      add_test(NAME brick${test_arg}_${behaviour}_consistenttangentoperator_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
	--@StiffnessMatrixType='ConsistentTangentOperator'
	--@library@="$<TARGET_FILE:MFrontCastemFiniteStrainSingleCrystalBrickBehaviours>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}-consistenttangentoperator.xml" ${file})
    else(NOT _REFERENCE_FILE)
      add_test(NAME brick${test_arg}_${behaviour}_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
    --@AccelerationAlgorithm='Cast3M'
	--@library@="$<TARGET_FILE:MFrontCastemFiniteStrainSingleCrystalBrickBehaviours>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}.xml"
	--@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}" ${file})
      add_test(NAME brick${test_arg}_${behaviour}_consistenttangentoperator_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
	--@StiffnessMatrixType='ConsistentTangentOperator'
	--@library@="$<TARGET_FILE:MFrontCastemFiniteStrainSingleCrystalBrickBehaviours>" --@behaviour@="${behaviour}"
	--@xml_output@="${test_arg}-${behaviour}-consistenttangentoperator.xml"
	--@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}" ${file})
    endif(NOT _REFERENCE_FILE)
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST brick${test_arg}_${behaviour}_mtest
	PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
      set_property(TEST brick${test_arg}_${behaviour}_consistenttangentoperator_mtest
	PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
    elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST brick${test_arg}_${behaviour}_mtest
	PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
      set_property(TEST brick${test_arg}_${behaviour}_consistenttangentoperator_mtest
	PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/FiniteStrainSingleCrystal/castem/"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront/tests/behaviours/FiniteStrainSingleCrystal/castem/"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endif(HAVE_CASTEM)
endmacro(castemtest_finitestrainsinglecrystalbrick)

# castemtest_finitestrainsinglecrystalbrick()
