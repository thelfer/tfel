mfront_behaviour_brick_check_library(MFrontGenericIsotropicDSLsTestsBehaviours
   IsotropicDSLs generic
   Plasticity1
   Plasticity2
   Plasticity3
   Plasticity4
   Plasticity5
   Plasticity6
   Plasticity7
   MultipleFlows1
   MultipleFlows2
   MultipleFlows3)

macro(generictestisotropic_dsls test_arg behaviour)
    set(_REFERENCE_FILE )
    if(NOT (${ARGC} EQUAL 0))
      set(_REFERENCE_FILE "${ARGN}")
    endif()
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
    foreach(rm ${IEEE754_ROUNDING_MODES})
      if(NOT _REFERENCE_FILE)
	add_test(NAME isotropic_dsls_${test_arg}${behaviour}_${rm}_mtest
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@library@="$<TARGET_FILE:MFrontGenericIsotropicDSLsTestsBehaviours>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml" ${file})
      else(NOT _REFERENCE_FILE)
	add_test(NAME isotropic_dsls_${test_arg}${behaviour}_${rm}_mtest

	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false
	  --@library@="$<TARGET_FILE:MFrontGenericIsotropicDSLsTestsBehaviours>" --@behaviour@="${behaviour}"
	  --@xml_output@="${test_arg}-${behaviour}-${rm}.xml"
	  --@reference_file@="${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}" ${file})
      endif(NOT _REFERENCE_FILE)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST isotropic_dsls_${test_arg}${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontGenericIsotropicDSLsTestsBehaviours mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	set_property(TEST isotropic_dsls_${test_arg}${behaviour}_${rm}_mtest
	  PROPERTY DEPENDS "MFrontGenericIsotropicDSLsTestsBehaviours mtest")
	set_property(TEST isotropic_dsls_${test_arg}${behaviour}_consistenttangentoperator_${rm}_mtest
	  PROPERTY DEPENDS "MFrontGenericIsotropicDSLsTestsBehaviours mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    endforeach(rm ${IEEE754_ROUNDING_MODES})
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/IsotropicDSLs/generic"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
	DESTINATION "share/doc/mfront/tests/behaviours/IsotropicDSLs/generic"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
endmacro(generictestisotropic_dsls)

generictestisotropic_dsls(plasticity  Plasticity6 plasticity.ref)
generictestisotropic_dsls(plasticity2 Plasticity7 plasticity.ref)

