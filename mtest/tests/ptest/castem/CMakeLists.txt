macro(castemptest test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "castemptest: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "castem${test_arg}-${conf}.ptest")
      get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      configure_file(castem${test_arg}.ptest.in
	${file} @ONLY)
      if(_NO_XML_OUTPUT)
	add_test(NAME castem${test_arg}_${conf}_ptest
	  COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false ${file}
	  CONFIGURATIONS ${conf})
      else(_NO_XML_OUTPUT)
	add_test(NAME castem${test_arg}_${conf}_ptest
	  COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file}
	  CONFIGURATIONS ${conf})
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(castem${test_arg}_${conf}_ptest
	  PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST castem${test_arg}_${conf}_ptest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem ptest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST castem${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	DESTINATION "share/mfront/tests/behaviours/castem"
	CONFIGURATIONS ${conf}
	COMPONENT mtest)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
    configure_file(castem${test_arg}.ptest.in
      castem${test_arg}.ptest @ONLY)
    if(_NO_XML_OUTPUT)
      add_test(NAME castem${test_arg}_ptest
	COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false castem${test_arg}.ptest)
    else(_NO_XML_OUTPUT)
      add_test(NAME castem${test_arg}_ptest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false castem${test_arg}.ptest)
    endif(_NO_XML_OUTPUT)
    if(_WILL_FAIL)
      set_tests_properties(castem${test_arg}_ptest
	PROPERTIES WILL_FAIL true)
    endif(_WILL_FAIL)
    set_property(TEST castem${test_arg}_ptest
      PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem ptest")
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/castem${test_arg}.ptest
      DESTINATION "share/mfront/tests/behaviours/castem"
      COMPONENT mtest)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(castemptest)

castemptest(elasticity-linear)
castemptest(elasticity-quadratic)
castemptest(isotropic-elastic-linear)
castemptest(isotropic-elastic-quadratic)
castemptest(isotropic-elastic2-linear)
castemptest(isotropic-elastic2-quadratic)
castemptest(isotropic-thermoelastic-linear)
castemptest(isotropic-thermoelastic-quadratic)
#castemptest(nortonpipe1D-linear)
#castemptest(nortonpipe1D-quadratic)
#castemptest(norton2pipe1D-linear)
#castemptest(norton2pipe1D-quadratic)
castemptest(orthotropic-elastic-linear)
castemptest(orthotropic-elastic-quadratic)
castemptest(orthotropic-elastic2-linear)
castemptest(orthotropic-elastic2-quadratic)
castemptest(orthotropic-thermoelastic-linear)
castemptest(orthotropic-thermoelastic-quadratic)
castemptest(orthotropic-thermoelastic2-linear)
castemptest(orthotropic-thermoelastic2-quadratic)
castemptest(tightpipe-linear)
castemptest(tightpipe-quadratic)
castemptest(tightpipe-gd-linear)
castemptest(tightpipe-gd-quadratic)
