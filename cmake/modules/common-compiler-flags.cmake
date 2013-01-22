# This files gather compiler flags that are common to g++, clang and
# pathCC

include(cmake/modules/compiler.cmake)

tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wall" "W" "Wextra" "pedantic" "Wshadow")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wpointer-arith" "Wcast-qual" "Wcast-align")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wwrite-strings" "Wctor-dtor-privacy" "Wnon-virtual-dtor")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Woverloaded-virtual" "Wreturn-type" "Wfloat-equal")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wno-endif-labels" "Wsign-compare" "Wmissing-format-attribute")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wno-multichar" "Wno-deprecated-declarations" "Wpacked")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wredundant-decls" "Wlong-long")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wunknown-pragmas" "Wundef" "Wreorder")
