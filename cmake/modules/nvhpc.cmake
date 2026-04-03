set(OPTIMISATION_FLAGS "-O2 -DNDEBUG ${OPTIMISATION_FLAGS}")
set(OPTIMISATION_FLAGS "-DTFEL_NO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
  tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS_MARCH "fast")
endif((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions("-g")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if(enable-fast-math)
  tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS  "ffinite-math-only")
else(enable-fast-math)
  tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS2 "ffinite-math-only")
endif(enable-fast-math)
