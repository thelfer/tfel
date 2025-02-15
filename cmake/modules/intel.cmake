tfel_add_cxx_compiler_flag_if_available(COMPILER_WARNINGS  "Wall")
# 383 : value copied to temporary, reference to temporary used
# 981 : operands are evaluated in unspecified order
# 810 : conversion from "long double" to "double" may lose significant bits
# 1418: external function definition with no prior declaration
# 444 : destructor for base class is not virtual
# 1565: attributes are ignored on a class declaration that is not also a definition
tfel_add_cxx_compiler_flag_if_available("-diag-disable 981" DIAG_DISABLE_FLAG_AVAILABLE)
if(${DIAG_DISABLE_FLAG_AVAILABLE})
  message(STATUS "enabling flag '-diag-disable'")
  set(COMPILER_WARNINGS "-diag-disable 981,383,810,1418,444,2259,1565 ${COMPILER_WARNINGS}")
else(${DIAG_DISABLE_FLAG_AVAILABLE})
  message(STATUS "flag '-diag-disable' disabled")
endif(${DIAG_DISABLE_FLAG_AVAILABLE})

tfel_add_cxx_compiler_flag_if_available(OPTIMISATION_FLAGS_MARCH "xHost")
tfel_add_cxx_compiler_flag_if_available(TFEL_CMAKE_CXX_FLAGS     "ipo")

tfel_add_cxx_compiler_flag_if_available(VISIBILITY_FLAGS "fvisibility=hidden")
tfel_add_cxx_compiler_flag_if_available(VISIBILITY_FLAGS "fvisibility-inlines-hidden")

set(OPTIMISATION_FLAGS "-DTFEL_NO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
  set(OPTIMISATION_FLAGS "-O2 -DNDEBUG ${OPTIMISATION_FLAGS}")
endif((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions("-g")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if(HAVE_FORTRAN)
  if("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "Intel")
    set(INTEL_FORTRAN_COMPILER ON)
  elseif("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "IntelLLVM")
    set(INTEL_FORTRAN_COMPILER ON)
  else("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "Intel")
    include(cmake/modules/gnu-fortran-compiler.cmake)
  endif("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "Intel")
endif(HAVE_FORTRAN)

