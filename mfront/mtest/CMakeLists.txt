include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/mfront/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
		 "${PROJECT_BINARY_DIR}/mfront/src")

set(mtest_SOURCES mtest.cxx
    MTestEvolution.cxx
    MTestBehaviour.cxx
    MTestUmatBehaviourBase.cxx
    MTestConstraint.cxx
    MTestImposedStrain.cxx
    MTestImposedStress.cxx)
set(mtest_LDADD
    TFELMath
    TFELSystem
    TFELUtilities
    TFELException)

if(HAVE_CASTEM)
set(mtest_SOURCES
    MTestUmatBehaviour.cxx
    ${mtest_SOURCES})
set(mtest_LDADD
    UMATInterface
    ${mtest_LDADD})
endif(HAVE_CASTEM)

if(HAVE_ASTER)
set(mtest_SOURCES
    MTestAsterBehaviour.cxx
    ${mtest_SOURCES})
set(mtest_LDADD
    AsterInterface
    ${mtest_LDADD})
endif(HAVE_ASTER)

add_executable(mtest ${mtest_SOURCES})
target_link_libraries(mtest ${mtest_LDADD})
  


