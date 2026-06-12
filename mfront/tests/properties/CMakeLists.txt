add_subdirectory(interfaces)

function(add_mfront_generated_source lib interface ext file)
  set(mfront_file   "${CMAKE_CURRENT_SOURCE_DIR}/${file}.mfront")
  set(mfront_executable "$<TARGET_FILE:mfront>")
  if("${interface}" STREQUAL "c")
    set(mfront_output "src/${file}.cxx")
  else("${interface}" STREQUAL "c")
    set(mfront_output "src/${file}-${ext}.cxx")
  endif("${interface}" STREQUAL "c")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(mfront_flags "--debug")
  else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(mfront_flags "")
  endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    add_custom_command(
      OUTPUT  "${mfront_output}"
      COMMAND "set"
      ARGS "PATH=$<TARGET_FILE_DIR:TFELMFront>;\
$<TARGET_FILE_DIR:MFrontLogStream>;\
$<TARGET_FILE_DIR:TFELMaterial>;\
$<TARGET_FILE_DIR:TFELNUMODIS>;\
$<TARGET_FILE_DIR:TFELMathParser>;\
$<TARGET_FILE_DIR:TFELGlossary>;\
$<TARGET_FILE_DIR:TFELSystem>;\
$<TARGET_FILE_DIR:TFELUtilities>;\
$<TARGET_FILE_DIR:TFELException>;\
$<TARGET_FILE_DIR:TFELConfig>;\
$<TARGET_FILE_DIR:TFELUnicodeSupport>;\
%PATH%"
      COMMAND "${mfront_executable}"
      ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties"
      ARGS    "${mfront_flags}" "--interface=${interface}" "${mfront_file}"
      DEPENDS mfront
      DEPENDS "${mfront_file}"
      COMMENT "mfront source ${mfront_file}")
    else((CMAKE_HOST_WIN32) AND (NOT MSYS))
    add_custom_command(
      OUTPUT  "${mfront_output}"
      COMMAND "${mfront_executable}"
      ARGS    "--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties"
      ARGS    "${mfront_flags}" "--interface=${interface}" "${mfront_file}"
      DEPENDS mfront
      DEPENDS "${mfront_file}"
      COMMENT "mfront source ${mfront_file}")
    if(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
      file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generation-test")
      add_test(NAME mfront-${file}-${interface}
	COMMAND ${mfront_executable} ${mfront_flags}
	--interface=${interface}
	--search-path=${PROJECT_SOURCE_DIR}/mfront/tests/properties
	${mfront_file}
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generation-test")
    endif(CMAKE_VERSION AND (${CMAKE_VERSION} GREATER "2.8.2"))
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  set(${lib}_SOURCES ${mfront_output} ${${lib}_SOURCES} PARENT_SCOPE)
endfunction(add_mfront_generated_source)

function(mfront_check_library lib interface ext)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "mfront_library : no source specified")
  endif(${ARGC} LESS 1)
  foreach(source ${ARGN})
    add_mfront_generated_source(${lib} ${interface} "${ext}" ${source})
  endforeach(source)
  add_library(${lib} EXCLUDE_FROM_ALL
    ${${lib}_SOURCES}
    ${${lib}_ADDITIONAL_SOURCES})
  target_link_libraries(${lib} PRIVATE TFELException)
  set_target_properties(${lib} PROPERTIES
      COMPILE_FLAGS "-DMFRONT_COMPILING")
  add_dependencies(check ${lib})
endfunction(mfront_check_library)

function(mfront_check_module lib interface ext)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "mfront_library : no source specified")
  endif(${ARGC} LESS 1)
  foreach(source ${ARGN})
    add_mfront_generated_source(${lib} ${interface} "${ext}" ${source})
  endforeach(source)
  add_library(${lib} MODULE EXCLUDE_FROM_ALL
    ${${lib}_SOURCES}
    ${${lib}_ADDITIONAL_SOURCES})
  target_link_libraries(${lib} PRIVATE TFELException)
  set_target_properties(${lib} PROPERTIES
      COMPILE_FLAGS "-DMFRONT_COMPILING")
  add_dependencies(check ${lib})
endfunction(mfront_check_module)

set(mfront_SOURCES
  ErrnoHandlingCheck
  YoungModulusTest
  PoissonRatioTest
  YoungModulusTest_1
  YoungModulusTest_2
  YoungModulusTest_3
  PoissonRatioTest_12
  PoissonRatioTest_23
  PoissonRatioTest_13
  ShearModulusTest_12
  ShearModulusTest_23
  ShearModulusTest_13
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3
  VanadiumAlloy_YoungModulus_SRMA
  VanadiumAlloy_PoissonRatio_SRMA
  VanadiumAlloy_ShearModulus_SRMA
  VanadiumAlloy_SpecificHeat_SRMA
  VanadiumAlloy_ThermalExpansion_SRMA
  VanadiumAlloy_ThermalConductivity_SRMA
  Inconel600_YoungModulus
  T91AusteniticSteel_ThermalExpansion_ROUX2007
  T91AusteniticSteel_b_ROUX2007
  T91AusteniticSteel_F_ROUX2007
  T91AusteniticSteel_Kvisc_ROUX2007
  T91AusteniticSteel_Nexp_ROUX2007
  T91AusteniticSteel_Rinf_ROUX2007
  T91AusteniticSteel_YieldStress_ROUX2007
  T91AusteniticSteel_YoungModulus_ROUX2007
  T91MartensiticSteel_b_ROUX2007
  T91MartensiticSteel_C1_ROUX2007
  T91MartensiticSteel_C2_ROUX2007
  T91MartensiticSteel_F_ROUX2007
  T91MartensiticSteel_gamma1_ROUX2007
  T91MartensiticSteel_growth_ROUX2007
  T91MartensiticSteel_Kvisc_ROUX2007
  T91MartensiticSteel_Nexp_ROUX2007
  T91MartensiticSteel_Rinf_ROUX2007
  T91MartensiticSteel_ThermalExpansion_ROUX2007
  T91MartensiticSteel_YieldStress_ROUX2007
  T91MartensiticSteel_YoungModulus_ROUX2007
  LinearDataInterpolationTest
  LinearDataInterpolationTest2
  LinearDataInterpolationTest3
  LinearDataInterpolationTest4
  LinearDataInterpolationTest5
  CubicSplineDataInterpolationTest
  CubicSplineDataInterpolationTest2
  CubicSplineDataInterpolationTest3
  CubicSplineDataInterpolationTest4
  CubicSplineDataInterpolationTest5)

mfront_dependencies(MFrontMaterialProperties-generic
  MaterialLawDSLOptionsTest
  VanadiumAlloy_YoungModulus_SRMA
  VanadiumAlloy_PoissonRatio_SRMA)
mfront_check_module(MFrontMaterialProperties-generic generic "generic" ${mfront_SOURCES})
target_include_directories(MFrontMaterialProperties-generic
  PRIVATE "${PROJECT_SOURCE_DIR}/include"
  PRIVATE "${PROJECT_SOURCE_DIR}/mfront/include"
  PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")

mfront_dependencies(MFrontMaterialProperties
  MaterialLawDSLOptionsTest
  VanadiumAlloy_YoungModulus_SRMA
  VanadiumAlloy_PoissonRatio_SRMA)
mfront_check_library(MFrontMaterialProperties c "" ${mfront_SOURCES})
target_include_directories(MFrontMaterialProperties
  PRIVATE "${PROJECT_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")

if(HAVE_CASTEM)
  mfront_dependencies(MFrontMaterialProperties-castem
    MaterialLawDSLOptionsTest
    VanadiumAlloy_YoungModulus_SRMA
    VanadiumAlloy_PoissonRatio_SRMA)
  mfront_check_module(MFrontMaterialProperties-castem castem "castem" ${mfront_SOURCES})
  target_include_directories(MFrontMaterialProperties-castem
    PRIVATE "${PROJECT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
  if(WIN32)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
      set_target_properties(MFrontMaterialProperties-castem PROPERTIES LINK_FLAGS "-Wl,--kill-at")
    endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  endif(WIN32)
endif(HAVE_CASTEM)

mfront_dependencies(MFrontMaterialProperties-cxx
  MaterialLawDSLOptionsTest
  VanadiumAlloy_YoungModulus_SRMA
  VanadiumAlloy_PoissonRatio_SRMA)
mfront_check_library(MFrontMaterialProperties-cxx cxx cxx ${mfront_SOURCES})
target_include_directories(MFrontMaterialProperties-cxx
  PRIVATE "${PROJECT_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontMaterialProperties-cxx PROPERTIES LINK_FLAGS "-Wl,--kill-at")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

if(HAVE_FORTRAN)
  mfront_dependencies(MFrontMaterialProperties-fortran
    MaterialLawDSLOptionsTest
    VanadiumAlloy_YoungModulus_SRMA
    VanadiumAlloy_PoissonRatio_SRMA)
  mfront_check_library(MFrontMaterialProperties-fortran fortran "fortran" ${mfront_SOURCES})
  target_include_directories(MFrontMaterialProperties-fortran
    PRIVATE "${PROJECT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
  if(WIN32)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
      set_target_properties(MFrontMaterialProperties-fortran PROPERTIES LINK_FLAGS "-Wl,--kill-at")
    endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  endif(WIN32)
endif(HAVE_FORTRAN)

add_executable(test-c EXCLUDE_FROM_ALL test-c.c)
target_include_directories(test-c
  PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(test-c MFrontMaterialProperties)
add_test(NAME test-c COMMAND test-c)
add_dependencies(check test-c)
add_dependencies(test-c MFrontMaterialProperties)

add_executable(test-cxx EXCLUDE_FROM_ALL test-cxx.cxx)
target_include_directories(test-cxx
  PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(test-cxx MFrontMaterialProperties-cxx
  TFELTests)
add_test(NAME test-cxx COMMAND test-cxx)
add_dependencies(check test-cxx)
add_dependencies(test-cxx MFrontMaterialProperties-cxx)
if((CMAKE_HOST_WIN32) AND (NOT MSYS))
  set_property(TEST test-cxx
               PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
  target_link_libraries(test-cxx "-lgcov")
endif(CMAKE_BUILD_TYPE STREQUAL "Coverage")

if(HAVE_FORTRAN)
  add_executable(test-f EXCLUDE_FROM_ALL test-f.c compute.f)
  add_test(NAME test-f COMMAND test-f)
  add_dependencies(check test-f)
  add_dependencies(test-f MFrontMaterialProperties-fortran)
  target_link_libraries(test-f MFrontMaterialProperties-fortran)
  IF(GNU_FORTRAN_COMPILER)
    if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
      target_link_libraries(test-f "-lgcov")
    endif(CMAKE_BUILD_TYPE STREQUAL "Coverage")
  endif(GNU_FORTRAN_COMPILER)
endif(HAVE_FORTRAN)
