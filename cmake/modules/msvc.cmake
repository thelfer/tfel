include(cmake/modules/OptimizeForArchitecture.cmake)

# tfel_add_cxx_compiler_flag_if_available(COMPILER_CXXFLAGS "MD")
tfel_add_cxx_compiler_flag_if_available(COMPILER_CXXFLAGS "EHsc")
tfel_add_cxx_compiler_flag_if_available(COMPILER_CXXFLAGS "utf-8")

tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "W4")
#4251 : warning about export of STL classes
tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "wd4251")
#4710 : warning function not being inlined
tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "wd4710")
#4514 : warning inline function was not used and was removed
tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "wd4514")
tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "wd4275")
#4250 : inheritance via dominance
tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "wd4250")
#4127 : conditional expression is constant
tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "wd4127")
#4244 : conversion
tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "wd4244")
#4996 : conversion
tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "wd4996")

# remove a warning about deprecated symbol
set(COMPILER_FLAGS "-D_CRT_SECURE_NO_WARNINGS ${COMPILER_FLAGS}")
set(COMPILER_FLAGS "-D_SCL_SECURE_NO_WARNINGS ${COMPILER_FLAGS}")
set(OPTIMISATION_FLAGS "/O2 -DNDEBUG ${OPTIMISATION_FLAGS}")
set(OPTIMISATION_FLAGS "-DTFEL_NO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if(NOT enable-fast-math)
  set(COMPILER_FLAGS "/fp:strict ${COMPILER_FLAGS}")
endif(NOT enable-fast-math)

# optimise build
OptimizeForArchitecture()