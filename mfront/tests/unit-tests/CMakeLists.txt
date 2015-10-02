include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/mfront/include")
include_directories("${PROJECT_SOURCE_DIR}/mtest/include")
link_directories("${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Glossary"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
		 "${PROJECT_BINARY_DIR}/src/Tests"
		 "${PROJECT_BINARY_DIR}/mfront/src"
		 "${PROJECT_BINARY_DIR}/mtest/src")

# a simple macro
macro(test_mfront test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
      PROPERTY DEPENDS "MFrontCastemBehaviours"
  	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_link_libraries(${test_arg}
    TFELMFront TFELMaterial TFELMath
    TFELSystem  TFELGlossary
    TFELUtilities TFELException TFELTests)
endmacro(test_mfront)

macro(test_mfront2 test_arg)
  configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/unit-tests/${test_arg}.cxx.in"
    "${PROJECT_BINARY_DIR}/mfront/tests/unit-tests/${test_arg}.cxx" @ONLY)
  add_executable(${test_arg} EXCLUDE_FROM_ALL
    "${PROJECT_BINARY_DIR}/mfront/tests/unit-tests/${test_arg}.cxx")
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
      PROPERTY DEPENDS "MFrontCastemBehaviours"
  	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_link_libraries(${test_arg}
    TFELMFront TFELMaterial TFELMath
    TFELSystem  TFELGlossary
    TFELUtilities TFELException TFELTests)
endmacro(test_mfront2)

macro(test_mfront3 test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg}
    TFELMFront TFELMaterial TFELMath
    TFELSystem  TFELGlossary
    TFELUtilities TFELException TFELTests)
endmacro(test_mfront3)

macro(test_mtest test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg}
    TFELMTest TFELMaterial TFELMath
    TFELSystem  TFELGlossary
    TFELUtilities TFELException TFELTests)
endmacro(test_mtest)

test_mfront(DSLTest)
test_mfront2(DSLTest2)
test_mfront(BehaviourDataTest)
test_mfront(BehaviourDescriptionTest)
test_mfront(BehaviourBrickProviderTest)
test_mfront2(BehaviourBrickRequirementManagerTest)
test_mfront3(TargetsDescriptionTest)

if(CMAKE_CONFIGURATION_TYPES)
  foreach(conf ${CMAKE_CONFIGURATION_TYPES})
    if(HAVE_CASTEM)
      get_property(MFrontCastemBehavioursBuildPath
	TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    endif(HAVE_CASTEM)
    if(HAVE_ASTER)
      get_property(MFrontAsterBehavioursBuildPath
	TARGET MFrontAsterBehaviours PROPERTY LOCATION_${conf})
    endif(HAVE_ASTER)
    if(HAVE_CYRANO)
      get_property(MFrontCyranoBehavioursBuildPath
	TARGET MFrontCyranoBehaviours PROPERTY LOCATION_${conf})
    endif(HAVE_CYRANO)
    configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/unit-tests/MTestParserTest.cxx.in"
      ${PROJECT_BINARY_DIR}/mfront/tests/unit-tests/MTestParserTest_${conf}.cxx @ONLY)
    add_executable(MTestParserTest_${conf} EXCLUDE_FROM_ALL
      ${PROJECT_BINARY_DIR}/mfront/tests/unit-tests/MTestParserTest_${conf}.cxx)
    add_test(NAME MTestParserTest_${conf}
      COMMAND MTestParserTest_${conf}
      CONFIGURATIONS ${conf})
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST MTestParserTest_${conf}
    	PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    if(HAVE_CASTEM)
      set_property(TEST MTestParserTest_${conf} APPEND PROPERTY DEPENDS MFrontCastemBehaviours)
    endif(HAVE_CASTEM)
    if(HAVE_ASTER)
      set_property(TEST MTestParserTest_${conf} APPEND PROPERTY DEPENDS MFrontAsterBehaviours)
    endif(HAVE_ASTER)
    if(HAVE_CYRANO)
      set_property(TEST MTestParserTest_${conf} APPEND PROPERTY DEPENDS MFrontCyranoBehaviours)
    endif(HAVE_CYRANO)   
    target_link_libraries(MTestParserTest_${conf} TFELMTest
      MFrontLogStream TFELGlossary TFELSystem TFELTests TFELException)
    add_dependencies(check MTestParserTest_${conf})
  endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
else(CMAKE_CONFIGURATION_TYPES)
  if(HAVE_CASTEM)
    get_property(MFrontCastemBehavioursBuildPath
      TARGET MFrontCastemBehaviours PROPERTY LOCATION)
  endif(HAVE_CASTEM)
  if(HAVE_ASTER)
    get_property(MFrontAsterBehavioursBuildPath
      TARGET MFrontAsterBehaviours PROPERTY LOCATION)
  endif(HAVE_ASTER)
  if(HAVE_CYRANO)
    get_property(MFrontCyranoBehavioursBuildPath
      TARGET MFrontCyranoBehaviours PROPERTY LOCATION)
  endif(HAVE_CYRANO)
  configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/unit-tests/MTestParserTest.cxx.in"
    ${PROJECT_BINARY_DIR}/mfront/tests/unit-tests/MTestParserTest.cxx @ONLY)
  add_executable(MTestParserTest EXCLUDE_FROM_ALL
    ${PROJECT_BINARY_DIR}/mfront/tests/unit-tests/MTestParserTest.cxx)
  add_test(NAME MTestParserTest COMMAND MTestParserTest)
  target_link_libraries(MTestParserTest TFELMTest
    MFrontLogStream TFELGlossary TFELSystem TFELTests TFELException)
  if(HAVE_CASTEM)
    set_property(TEST MTestParserTest APPEND PROPERTY DEPENDS MFrontCastemBehaviours)
  endif(HAVE_CASTEM)
  if(HAVE_ASTER)
    set_property(TEST MTestParserTest APPEND PROPERTY DEPENDS MFrontAsterBehaviours)
  endif(HAVE_ASTER)
  if(HAVE_CYRANO)
    set_property(TEST MTestParserTest APPEND PROPERTY DEPENDS MFrontCyranoBehaviours)
  endif(HAVE_CYRANO)
  add_dependencies(check MTestParserTest)
endif(CMAKE_CONFIGURATION_TYPES)
