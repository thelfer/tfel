include_directories("${PROJECT_SOURCE_DIR}/include",
	            "${CMAKE_CURRENT_BINARY_DIR}/include")

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
    VanadiumAlloy_YoungModulus_SRMA
    VanadiumAlloy_PoissonRatio_SRMA
    VanadiumAlloy_SpecificHeat_SRMA
    VanadiumAlloy_ThermalExpansion_SRMA
    VanadiumAlloy_ThermalConductivity_SRMA)

mfront_check_library(MFrontMaterialProperties c "" ${mfront_SOURCES})

if(HAVE_CASTEM)
  mfront_check_library(MFrontMaterialProperties-castem castem "castem" ${mfront_SOURCES})
endif(HAVE_CASTEM)

if(HAVE_FORTRAN)
  mfront_check_library(MFrontMaterialProperties-fortran fortran "fortran" ${mfront_SOURCES})
endif(HAVE_FORTRAN)

add_executable(test-c EXCLUDE_FROM_ALL test-c.c)
add_test(test-c test-c)
add_dependencies(check test-c)
add_dependencies(test-c MFrontMaterialProperties)
target_link_libraries(test-c MFrontMaterialProperties)

if(HAVE_FORTRAN)
  add_executable(test-f EXCLUDE_FROM_ALL test-f.c compute.f)
  add_test(test-f test-f)
  add_dependencies(check test-f)
  add_dependencies(test-f MFrontMaterialProperties-fortran)
  target_link_libraries(test-f MFrontMaterialProperties-fortran)
endif(HAVE_FORTRAN)