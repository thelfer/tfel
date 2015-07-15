tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Weverything")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-padded")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-documentation")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-exit-time-destructors")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-global-constructors")
tfel_enable_cxx_compiler_flag2(COMPILER_WARNINGS "Wno-c++98-compat" "Wno_c__98_compat_AVAILABLE")

tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS_MARCH "march=native")
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS2 "ffast-math")

tfel_enable_cxx_compiler_flag(VISIBILITY_FLAGS "fvisibility=hidden")
tfel_enable_cxx_compiler_flag(VISIBILITY_FLAGS "fvisibility-inlines-hidden")

set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
  set(OPTIMISATION_FLAGS "-O2 -DNDEBUG ${OPTIMISATION_FLAGS}")
endif((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions("-g")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if(HAVE_FORTRAN)
  # we associate clang with the gnu fortran compiler
  include(cmake/modules/gnu-fortran-compiler.cmake)
endif(HAVE_FORTRAN)

if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.4)
  message(FATAL_ERROR "TFEL C++11 support is only available for clang version >= 3.4")
endif(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.4)
set(COMPILER_FLAGS "${COMPILER_FLAGS} -std=c++11")
