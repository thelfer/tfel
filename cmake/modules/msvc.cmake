add_definitions("-DMICROSOFT_COMPILER_MSC_=1")
tfel_enable_cxx_compiler_flag(COMPILER_FLAGS "MD")
tfel_enable_cxx_compiler_flag(COMPILER_FLAGS "EHsc")

tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "W4")
#4251 : warning about export of STL classes
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4251")
#4710 : warning function not being inlined
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4710")
#4514 : warning inline function was not used and was removed
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4514")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4275")

set(OPTIMISATION_FLAGS "/O2 -DNDEBUG ${OPTIMISATION_FLAGS}")
set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if(HAVE_FORTRAN)
  include(cmake/modules/default-fortran.cmake)
endif(HAVE_FORTRAN)