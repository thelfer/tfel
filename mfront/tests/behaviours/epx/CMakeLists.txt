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
  SaintVenantKirchhoffElasticity)

# mfront_behaviour_check_library(MFrontEuroplexusBehaviours  epx ${mfront_tests_SOURCES})
# target_link_libraries(MFrontEuroplexusBehaviours
#   EuroplexusInterface
#   MFrontProfiling
#   TFELMaterial
#   TFELMath
#   TFELUtilities
#   TFELException)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontEuroplexusBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

macro(test_epx test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "test_epx: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "epx{test_arg}-${conf}.mtest")
      get_property(MFrontEuroplexusBehavioursBuildPath TARGET MFrontEuroplexusBehaviours PROPERTY LOCATION_${conf})
      if(HAVE_CASTEM)
	get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      endif(HAVE_CASTEM)
      configure_file(epx{test_arg}.mtest.in
	${file} @ONLY)
      if(_NO_XML_OUTPUT)
	add_test(NAME epx{test_arg}_${conf}_mtest
	  CONFIGURATIONS ${conf}
	  COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false ${file})
      else(_NO_XML_OUTPUT)
	add_test(NAME epx{test_arg}_${conf}_mtest
	  CONFIGURATIONS ${conf}
	  COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(epx{test_arg}_${conf}_mtest
	  PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST epx{test_arg}_${conf}_mtest
          PROPERTY DEPENDS MFrontEuroplexusBehaviours MFrontMaterialProperties-europlexus mtest
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST epx{test_arg}_${conf}_mtest
          PROPERTY DEPENDS MFrontEuroplexusBehaviours MFrontMaterialProperties-castem mtest)
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	DESTINATION "share/mfront/tests/behaviours/europlexus"
	COMPONENT mtest)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontEuroplexusBehavioursBuildPath TARGET MFrontEuroplexusBehaviours PROPERTY LOCATION)
    if(HAVE_CASTEM)
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)
    endif(HAVE_CASTEM)
    configure_file(epx{test_arg}.mtest.in
      epx{test_arg}.mtest @ONLY)
    if(_NO_XML_OUTPUT)
      add_test(NAME epx{test_arg}_mtest
	COMMAND mtest --verbose=level0 --xml-output=false --result-file-output=false epx{test_arg}.mtest)
    else(_NO_XML_OUTPUT)
      add_test(NAME epx{test_arg}_mtest
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false epx{test_arg}.mtest)
    endif(_NO_XML_OUTPUT)
    if(_WILL_FAIL)
      set_tests_properties(epx{test_arg}_mtest
	PROPERTIES WILL_FAIL true)
    endif(_WILL_FAIL)
    set_tests_properties(epx{test_arg}_mtest
      PROPERTIES DEPENDS MFrontEuroplexusBehaviours)
    set_tests_properties(epx{test_arg}_mtest
      PROPERTIES DEPENDS mtest)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/epx{test_arg}.mtest
      DESTINATION "share/mfront/tests/behaviours/europlexus"
      COMPONENT mtest)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_epx)
