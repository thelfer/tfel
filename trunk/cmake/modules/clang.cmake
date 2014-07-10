tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wall" "W" "Wextra" "pedantic" "Wshadow")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wpointer-arith" "Wcast-qual" "Wcast-align")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wwrite-strings" "Wctor-dtor-privacy" "Wnon-virtual-dtor")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Woverloaded-virtual" "Wreturn-type" "Wfloat-equal")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-endif-labels" "Wsign-compare" "Wmissing-format-attribute")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-multichar" "Wno-deprecated-declarations" "Wpacked")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wredundant-decls" "Wlong-long" "Wdisabled-optimization")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wunknown-pragmas" "Wundef" "Wreorder")

tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fvisibility=hidden")
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fvisibility-inlines-hidden")
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "march=native")

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
