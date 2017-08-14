if(HAVE_FORTRAN)
  macro(test_castemumat test_arg)
    set(_NO_XML_OUTPUT )
    set(_WILL_FAIL )
    foreach(_ARG ${ARGN})
      if ( ${_ARG} MATCHES NO_XML_OUTPUT )
	set ( _NO_XML_OUTPUT ON)
      elseif ( ${_ARG} MATCHES WILL_FAIL)
	set ( _WILL_FAIL ON)
      else()
	message(FATAL_ERROR "test_castemumat: unsupported option '${_ARG}'")
      endif()
    endforeach(_ARG ${ARGN})
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(conf ${CMAKE_CONFIGURATION_TYPES})
	set(file "${CMAKE_CURRENT_SOURCE_DIR}/castemumat${test_arg}.mtest")
	get_property(CastemUmatLibraryPath TARGET CastemUmat PROPERTY LOCATION_${conf})
	if(_NO_XML_OUTPUT)
	  add_test(NAME castemumat${test_arg}_${conf}_mtest
	    COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false --@CASTEMUMATLIBRARY@="${CastemUmatLibraryPath}" ${file}
	    CONFIGURATIONS ${conf})
	else(_NO_XML_OUTPUT)
	  add_test(NAME castemumat${test_arg}_${conf}_mtest
	    COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false --@CASTEMUMATLIBRARY@="${CastemUmatLibraryPath}" ${file}
	    CONFIGURATIONS ${conf})
	endif(_NO_XML_OUTPUT)
	if(_WILL_FAIL)
	  set_tests_properties(castemumat${test_arg}_${conf}_mtest
	    PROPERTIES WILL_FAIL true)
	endif(_WILL_FAIL)
	if((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST castemumat${test_arg}_${conf}_mtest
            PROPERTY DEPENDS "CastemUmat mtest"
	    PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
	elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST castemumat${test_arg}_${conf}_mtest
            PROPERTY DEPENDS "CastemUmat mtest")
	endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	if(TFEL_APPEND_VERSION)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mtest-${TFEL_VERSION}/tests/mtest/behaviours/castem"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
	else(TFEL_APPEND_VERSION)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mtest-${TFEL_VERSION}/tests/mtest/behaviours/castem"
	    CONFIGURATIONS ${conf}
	    COMPONENT mtest)
	endif(TFEL_APPEND_VERSION)
      endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
    else(CMAKE_CONFIGURATION_TYPES)
      get_property(CastemUmatLibraryPath TARGET CastemUmat PROPERTY LOCATION_${conf})
      if(_NO_XML_OUTPUT)
	add_test(NAME castemumat${test_arg}_mtest
	  COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false --@CASTEMUMATLIBRARY@="${CastemUmatLibraryPath}" ${CMAKE_CURRENT_SOURCE_DIR}/castemumat${test_arg}.mtest)
      else(_NO_XML_OUTPUT)
	add_test(NAME castemumat${test_arg}_mtest
	  COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false --@CASTEMUMATLIBRARY@="${CastemUmatLibraryPath}" ${CMAKE_CURRENT_SOURCE_DIR}/castemumat${test_arg}.mtest)
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(castemumat${test_arg}_mtest
	  PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      set_property(TEST castemumat${test_arg}_mtest
	PROPERTY DEPENDS "CastemUmat mtest")
      if(TFEL_APPEND_VERSION)
      install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/castemumat${test_arg}.mtest
	  DESTINATION "share/doc/mtest-${TFEL_VERSION}/tests/mtest/behaviours/castem"
	COMPONENT mtest)
      else(TFEL_APPEND_VERSION)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/castemumat${test_arg}.mtest
	  DESTINATION "share/doc/mtest/tests/mtest/behaviours/castem"
	  COMPONENT mtest)
      endif(TFEL_APPEND_VERSION)
    endif(CMAKE_CONFIGURATION_TYPES)
  endmacro(test_castemumat)
  add_library(CastemUmat SHARED umat.f mooney.f neohoo.f)
  test_castemumat(elastic)
  test_castemumat(incompressiblemooneyrivlin)
  test_castemumat(compressibleneohookeanbehaviour)
endif(HAVE_FORTRAN)