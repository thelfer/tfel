configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)

macro(test_crosseddelta2 test_arg)
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "crosseddelta2${test_arg}-${conf}.mtest")
      get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      configure_file(crosseddelta2${test_arg}.mtest.in
	${file} @ONLY)
      add_test(NAME crosseddelta2${test_arg}_${conf}_mtest
	CONFIGURATIONS ${conf}
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST crosseddelta2${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST crosseddelta2${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      if(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/crosseddelta2"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      else(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/crosseddelta2"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      endif(TFEL_APPEND_SUFFIX)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    configure_file(crosseddelta2${test_arg}.mtest.in
      crosseddelta2${test_arg}.mtest @ONLY)
    add_test(NAME crosseddelta2${test_arg}_mtest
      COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false crosseddelta2${test_arg}.mtest)
    set_tests_properties(crosseddelta2${test_arg}_mtest
      PROPERTIES DEPENDS MFrontCastemBehaviours)
    set_tests_properties(crosseddelta2${test_arg}_mtest
      PROPERTIES DEPENDS MFrontMaterialProperties-castem)
    set_tests_properties(crosseddelta2${test_arg}_mtest
      PROPERTIES DEPENDS mtest)
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/crosseddelta2${test_arg}.mtest
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/crosseddelta2"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/crosseddelta2${test_arg}.mtest
	DESTINATION "share/doc/mfront/tests/behaviours/crosseddelta2"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_crosseddelta2)

test_crosseddelta2(ddif2-2)
test_crosseddelta2(ddif2-3)
test_crosseddelta2(ddif2-4)
test_crosseddelta2(ddif2)
test_crosseddelta2(druckerprager)
test_crosseddelta2(ellipticcreep)
test_crosseddelta2(finitestrainsinglecrystal-001)
test_crosseddelta2(finitestrainsinglecrystal-012-2)
test_crosseddelta2(finitestrainsinglecrystal-012)
test_crosseddelta2(finitestrainsinglecrystal-159)
test_crosseddelta2(finitestrainsinglecrystal2-001)
test_crosseddelta2(finitestrainsinglecrystal_numericaljacobian-001)
test_crosseddelta2(finitestrainsinglecrystal_numericaljacobian-012-2)
test_crosseddelta2(finitestrainsinglecrystal_numericaljacobian-012)
test_crosseddelta2(finitestrainsinglecrystal_numericaljacobian-159)
test_crosseddelta2(hayhurst)
test_crosseddelta2(implicithayhurst)
test_crosseddelta2(implicitmonocrystal)
test_crosseddelta2(implicitnorton3)
test_crosseddelta2(implicitnorton-planestress2)
test_crosseddelta2(implicitnorton-planestress)
test_crosseddelta2(lorentz)
test_crosseddelta2(monocrystal_dd_cc_irradiation)
test_crosseddelta2(monocrystal_dd_cc_irradiation_numericaljacobian)
test_crosseddelta2(monocrystal_dd_cfc_irradiation)
test_crosseddelta2(monocrystal_dd_cfc)
test_crosseddelta2(monocrystal_dd_cfc_numericaljacobian)
test_crosseddelta2(monocrystal)
test_crosseddelta2(norton2)
test_crosseddelta2(norton-euler)
test_crosseddelta2(norton)
test_crosseddelta2(norton-rk2)
test_crosseddelta2(nortonrk2)
test_crosseddelta2(nortonrk3)
test_crosseddelta2(norton-rk4)
test_crosseddelta2(nortonrk4)
test_crosseddelta2(plasisocin)
test_crosseddelta2(plasticity)
test_crosseddelta2(polycrystals_dd_cfc)
test_crosseddelta2(strainhardeningcreep)
test_crosseddelta2(stressrelaxation)
test_crosseddelta2(T91-viscositytest)
test_crosseddelta2(viscoplasisocin)

# test_crosseddelta2(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
# test_crosseddelta2(monocrystal_dd_cc)
# test_crosseddelta2(monocrystal_dd_cc_numericaljacobian)
