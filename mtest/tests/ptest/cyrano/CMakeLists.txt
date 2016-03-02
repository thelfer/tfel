macro(cyranoptest test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "cyranoptest: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "cyrano${test_arg}-${conf}.ptest")
      get_property(MFrontCyranoBehavioursBuildPath TARGET MFrontCyranoBehaviours PROPERTY LOCATION_${conf})
      get_property(MFrontMaterialPropertiesCyranoBuildPath TARGET MFrontMaterialProperties-cyrano PROPERTY LOCATION_${conf})
      configure_file(cyrano${test_arg}.ptest.in
	${file} @ONLY)
      if(_NO_XML_OUTPUT)
	add_test(NAME cyrano${test_arg}_${conf}_ptest
	  COMMAND ptest --verbose=level0 --xml-output=false --result-file-output=false ${file}
	  CONFIGURATIONS ${conf})
      else(_NO_XML_OUTPUT)
	add_test(NAME cyrano${test_arg}_${conf}_ptest
	  COMMAND ptest --verbose=level0 --xml-output=true --result-file-output=false ${file}
	  CONFIGURATIONS ${conf})
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(cyrano${test_arg}_${conf}_ptest
	  PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST cyrano${test_arg}_${conf}_ptest
          PROPERTY DEPENDS "MFrontCyranoBehaviours MFrontMaterialProperties-cyrano ptest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST cyrano${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCyranoBehaviours MFrontMaterialProperties-cyrano mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	DESTINATION "share/mfront/tests/behaviours/cyrano"
	CONFIGURATIONS ${conf}
	COMPONENT mtest)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontCyranoBehavioursBuildPath TARGET MFrontCyranoBehaviours PROPERTY LOCATION_${conf})
    configure_file(cyrano${test_arg}.ptest.in
      cyrano${test_arg}.ptest @ONLY)
    if(_NO_XML_OUTPUT)
      add_test(NAME cyrano${test_arg}_ptest
	COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false cyrano${test_arg}.ptest)
    else(_NO_XML_OUTPUT)
      add_test(NAME cyrano${test_arg}_ptest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false cyrano${test_arg}.ptest)
    endif(_NO_XML_OUTPUT)
    if(_WILL_FAIL)
      set_tests_properties(cyrano${test_arg}_ptest
	PROPERTIES WILL_FAIL true)
    endif(_WILL_FAIL)
    set_property(TEST cyrano${test_arg}_ptest
      PROPERTY DEPENDS "MFrontCyranoBehaviours MFrontMaterialProperties-castem ptest")
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cyrano${test_arg}.ptest
      DESTINATION "share/mfront/tests/behaviours/cyrano"
      COMPONENT mtest)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(cyranoptest)

cyranoptest(elasticity-linear)
cyranoptest(elasticity-quadratic)
cyranoptest(isotropic-elastic-linear)
cyranoptest(isotropic-elastic-quadratic)
cyranoptest(isotropic-elastic2-linear)
cyranoptest(isotropic-elastic2-quadratic)
cyranoptest(isotropic-thermoelastic-linear)
cyranoptest(isotropic-thermoelastic-quadratic)
#cyranoptest(nortonpipe1D-linear)
#cyranoptest(nortonpipe1D-quadratic)
#cyranoptest(norton2pipe1D-linear)
#cyranoptest(norton2pipe1D-quadratic)
cyranoptest(orthotropic-elastic-linear)
cyranoptest(orthotropic-elastic-quadratic)
cyranoptest(orthotropic-elastic2-linear)
cyranoptest(orthotropic-elastic2-quadratic)
cyranoptest(orthotropic-thermoelastic-linear)
cyranoptest(orthotropic-thermoelastic-quadratic)
cyranoptest(orthotropic-thermoelastic2-linear)
cyranoptest(orthotropic-thermoelastic2-quadratic)
cyranoptest(tightpipe-linear)
cyranoptest(tightpipe-quadratic)
cyranoptest(tightpipe-gd-linear)
cyranoptest(tightpipe-gd-quadratic)