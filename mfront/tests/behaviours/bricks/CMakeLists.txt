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
  IsotropicStandardElasticity
  IsotropicStandardElasticity2
  IsotropicStandardElasticity3
  IsotropicStandardElasticity4
  IsotropicStandardElasticity5 
  IsotropicStandardElasticity6
  IsotropicStandardElasticity7
  IsotropicStandardElasticity8
  IsotropicStandardElasticity9
  IsotropicStandardElasticity10
  IsotropicStandardElasticity11
  IsotropicStandardElasticity12
  IsotropicStandardElasticity13
  OrthotropicStandardElasticity
  OrthotropicStandardElasticity2
  OrthotropicStandardElasticity3
 )

mfront_dependencies(MFrontCastemBehaviourBricks
  YoungModulusTest
  YoungModulusTest_1
  YoungModulusTest_2
  YoungModulusTest_3
  PoissonRatioTest_12
  PoissonRatioTest_23
  PoissonRatioTest_13
  ShearModulusTest_12
  ShearModulusTest_23
  ShearModulusTest_13
  Inconel600_YoungModulus)

mfront_behaviour_check_library(MFrontCastemBehaviourBricks umat ${mfront_tests_SOURCES})

target_link_libraries(MFrontCastemBehaviourBricks
  CastemInterface
  MFrontProfiling
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

macro(test_brick test_arg behaviour)
  set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
  add_test(NAME brick${test_arg}${behaviour}_mtest
    COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false
    --@library@="/home/th202608/codes/tfel/tfel-3.0.x/src/build/mfront/tests/behaviours/bricks/libMFrontCastemBehaviourBricks.so" --@behaviour@="${behaviour}" --@xml_output@="${test_arg}-${behaviour}.xml" ${file})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST castem${test_arg}_${conf}_mtest
      PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
      PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
  elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST castem${test_arg}_${conf}_mtest
      PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  install(FILES ${file}
    DESTINATION "share/mfront/tests/behaviours/bricks"
    COMPONENT mtest)
endmacro(test_brick)

test_brick(isotropicstandardelasticity umatisotropicstandardelasticity )
test_brick(isotropicstandardelasticity umatisotropicstandardelasticity2)
test_brick(isotropicstandardelasticity umatisotropicstandardelasticity3)
test_brick(isotropicstandardelasticity umatisotropicstandardelasticity4)
test_brick(isotropicstandardelasticity umatisotropicstandardelasticity5)
test_brick(isotropicstandardelasticity umatisotropicstandardelasticity6)
test_brick(isotropicstandardelasticity umatisotropicstandardelasticity7)
test_brick(isotropicstandardelasticity umatisotropicstandardelasticity8)
test_brick(isotropicstandardelasticity umatisotropicstandardelasticity9)
test_brick(isotropicstandardelasticity umatisotropicstandardelasticity10)

test_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity )
test_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity2)
test_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity3)
test_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity4)
test_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity5)
test_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity6)
test_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity7)
test_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity8)
test_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity9)
test_brick(isotropicstandardelasticity2 umatisotropicstandardelasticity10)

test_brick(isotropicstandardelasticity3 umatisotropicstandardelasticity11)

test_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity )
test_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity2)
test_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity3)
test_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity4)
test_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity5)
test_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity6)
test_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity7)
test_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity8)
test_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity9)
test_brick(isotropicstandardelasticity4 umatisotropicstandardelasticity10)

test_brick(isotropicstandardelasticity5 umatisotropicstandardelasticity12)
test_brick(isotropicstandardelasticity5 umatisotropicstandardelasticity13)
test_brick(isotropicstandardelasticity6 umatisotropicstandardelasticity12)
test_brick(isotropicstandardelasticity6 umatisotropicstandardelasticity13)
test_brick(isotropicstandardelasticity7 umatisotropicstandardelasticity12)
test_brick(isotropicstandardelasticity7 umatisotropicstandardelasticity13)

test_brick(orthotropicstandardelasticity umatorthotropicstandardelasticity )
test_brick(orthotropicstandardelasticity umatorthotropicstandardelasticity2)
test_brick(orthotropicstandardelasticity umatorthotropicstandardelasticity3)

