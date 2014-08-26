add_subdirectory(interfaces)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")

macro(add_mfront_generated_source lib interface ext file)
  set(mfront_file   "${CMAKE_CURRENT_SOURCE_DIR}/${file}.mfront")
  if(WIN32)
    set(mfront_executable "${PROJECT_BINARY_DIR}/mfront/src/mfront.exe")
  else(WIN32)
    set(mfront_executable "${PROJECT_BINARY_DIR}/mfront/src/mfront")
  endif(WIN32)
  if("${ext}" STREQUAL "")
    set(mfront_output "src/${file}.cxx")
  else("${ext}" STREQUAL "")
    set(mfront_output "src/${file}-${ext}.cxx")
  endif("${ext}" STREQUAL "")
  add_custom_command(
    OUTPUT  "${mfront_output}"
    COMMAND "${mfront_executable}"
    ARGS    "--interface=${interface}" "${mfront_file}"
    DEPENDS "${PROJECT_BINARY_DIR}/mfront/src/mfront"
    DEPENDS "${mfront_file}"
    COMMENT "mfront source")
  set(${lib}_SOURCES ${mfront_output} ${${lib}_SOURCES})
endmacro(add_mfront_generated_source)

macro(mfront_check_library lib interface ext)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "mfront_library : no source specified")
  endif(${ARGC} LESS 1)
  foreach(source ${ARGN})
    if("${ext}" STREQUAL "")
      add_mfront_generated_source(${lib} ${interface} "" ${source})
    else("${ext}" STREQUAL "")
      add_mfront_generated_source(${lib} ${interface} ${ext} ${source})
    endif("${ext}" STREQUAL "")
  endforeach(source)
  add_library(${lib} SHARED EXCLUDE_FROM_ALL
    ${${lib}_SOURCES})
  add_dependencies(check ${lib})
endmacro(mfront_check_library)

set(mfront_SOURCES
    ThermalExpansionCoefficientTest
    ThermalExpansionCoefficientTest2
    ThermalExpansionCoefficientTest_1
    ThermalExpansionCoefficientTest_2
    ThermalExpansionCoefficientTest_3
    VanadiumAlloy_YoungModulus_SRMA
    VanadiumAlloy_PoissonRatio_SRMA
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
    T91AusteniticSteel_ThermalExpansion_ROUX2007
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
    T91MartensiticSteel_YoungModulus_ROUX2007)

mfront_check_library(MFrontMaterialProperties c "" ${mfront_SOURCES})

if(HAVE_CASTEM)
  mfront_check_library(MFrontMaterialProperties-castem castem "castem" ${mfront_SOURCES})
  if(WIN32)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
      set_target_properties(MFrontMaterialProperties-castem PROPERTIES LINK_FLAGS "-Wl,--kill-at")
    endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  endif(WIN32)
endif(HAVE_CASTEM)

if(HAVE_FORTRAN)
  mfront_check_library(MFrontMaterialProperties-fortran fortran "fortran" ${mfront_SOURCES})
  if(WIN32)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
      set_target_properties(MFrontMaterialProperties-fortran PROPERTIES LINK_FLAGS "-Wl,--kill-at")
    endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  endif(WIN32)
endif(HAVE_FORTRAN)

add_executable(test-c EXCLUDE_FROM_ALL test-c.c)
add_test(NAME test-c COMMAND test-c)
add_dependencies(check test-c)
add_dependencies(test-c MFrontMaterialProperties)
target_link_libraries(test-c MFrontMaterialProperties)

if(HAVE_FORTRAN)
  IF(GNU_FORTRAN_COMPILER)
    add_executable(test-f EXCLUDE_FROM_ALL test-f.c compute.f)
    add_test(NAME test-f COMMAND test-f)
    add_dependencies(check test-f)
    add_dependencies(test-f MFrontMaterialProperties-fortran)
    target_link_libraries(test-f MFrontMaterialProperties-fortran)
  endif(GNU_FORTRAN_COMPILER)
endif(HAVE_FORTRAN)