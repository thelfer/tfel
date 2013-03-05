include(cmake/modules/compiler.cmake)

tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wall")

# 383 : value copied to temporary, reference to temporary used
# 981 : operands are evaluated in unspecified order
# 810 : conversion from "long double" to "double" may lose significant bits
# 1418: external function definition with no prior declaration
# 444 : destructor for base class is not virtual
tfel_check_cxx_compiler_flag("-diag-disable 981"
                             DIAG_DISABLE_FLAG_AVAILABLE)
if(${DIAG_DISABLE_FLAG_AVAILABLE})
  message(STATUS "enabling flag '-diag-disable'")
  set(COMPILER_WARNINGS "-diag-disable 981,383,810,1418,444 ${COMPILER_WARNINGS}")
else(${DIAG_DISABLE_FLAG_AVAILABLE})
  message(STATUS "flag '-diag-disable' disabled")
endif(${DIAG_DISABLE_FLAG_AVAILABLE})

tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wall")
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fvisibility=hidden")
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "fvisibility-inlines-hidden")
tfel_enable_cxx_compiler_flag(OPTIMISATION_FLAGS "xHost")
tfel_enable_cxx_compiler_flag(CMAKE_CXX_FLAGS    "ipo")

set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
  add_definitions("-O3 -DNDEBUG")
  set(OPTIMISATION_FLAGS "-O3 -DNDEBUG ${OPTIMISATION_FLAGS}")
endif((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions("-g")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if(HAVE_FORTRAN)
  get_filename_component (Fortran_COMPILER_NAME ${CMAKE_Fortran_COMPILER} NAME)
  if ("${Fortran_COMPILER_NAME}" MATCHES "^ifort.*")
    set(INTEL_FORTRAN_COMPILER ON)
    add_definitions("-D'F77_FUNC(X,Y)=X\\#\\#_' -D'F77_FUNC_(X,Y)=X\\#\\#_'")
  else("${Fortran_COMPILER_NAME}" MATCHES "^ifort.*")
    message(FATAL_ERROR "unsupported fortran compiler ${CMAKE_Fortran_COMPILER_NAME}")
  endif("${Fortran_COMPILER_NAME}" MATCHES "^ifort.*")
endif(HAVE_FORTRAN)
