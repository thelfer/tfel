tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS_MARCH "fast")
if(NOT enable-fast-math)
  tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS  "Kieee")
endif(NOT enable-fast-math)

set(OPTIMISATION_FLAGS "-DTFEL_NO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")
if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
  set(OPTIMISATION_FLAGS "-Minline -Mipa -DNDEBUG ${OPTIMISATION_FLAGS}")
endif((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions("-g")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")


