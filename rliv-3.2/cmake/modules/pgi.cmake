tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS_MARCH "fast")
if(NOT enable-fast-math)
  tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS  "Kieee")
endif(NOT enable-fast-math)

set(COMPILER_CXXFLAGS "${COMPILER_CXXFLAGS} -std=c++11")
set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")
if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
  set(OPTIMISATION_FLAGS "-Minline -Mipa -DNDEBUG ${OPTIMISATION_FLAGS}")
endif((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions("-g")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")


