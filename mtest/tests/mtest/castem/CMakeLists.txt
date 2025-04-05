if(HAVE_FORTRAN)
  function(test_castemumat test_arg)
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
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/castemumat${test_arg}.mtest")
    if(_NO_XML_OUTPUT)
      add_test(NAME castemumat${test_arg}_mtest
	       COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false
                             --@CASTEMUMATLIBRARY@="$<TARGET_FILE:CastemUmat>"
                             ${CMAKE_CURRENT_SOURCE_DIR}/castemumat${test_arg}.mtest)
    else(_NO_XML_OUTPUT)
	add_test(NAME castemumat${test_arg}_mtest
	  COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
                        --@CASTEMUMATLIBRARY@="$<TARGET_FILE:CastemUmat>"
                        ${CMAKE_CURRENT_SOURCE_DIR}/castemumat${test_arg}.mtest)
    endif(_NO_XML_OUTPUT)
    if(_WILL_FAIL)
      set_tests_properties(castemumat${test_arg}_mtest
	                   PROPERTIES WILL_FAIL true)
    endif(_WILL_FAIL)
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/castemumat${test_arg}.mtest
	      DESTINATION "share/doc/mtest-${TFEL_SUFFIX}/tests/mtest/behaviours/castem"
	      COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/castemumat${test_arg}.mtest
	      DESTINATION "share/doc/mtest/tests/mtest/behaviours/castem"
	      COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endfunction(test_castemumat)
  add_library(CastemUmat SHARED umat.f mooney.f neohoo.f)
  test_castemumat(elastic)
  test_castemumat(incompressiblemooneyrivlin)
  test_castemumat(compressibleneohookeanbehaviour)
endif(HAVE_FORTRAN)
