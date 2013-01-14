# warnings
add_definitions("-Wall -W -Wextra -pedantic -Wshadow")
# reduce number of exported symbols
add_definitions("-fvisibility-inlines-hidden -fvisibility=hidden")

# if(HAVE_FORTRAN)
#  if ("${CMAKE_Fortran_COMPILER_NAME}" STREQUAL "gfortran")
#    add_definitions("-D'F77_FUNC(X,Y)=X\\#\\#_' -D'F77_FUNC_(X,Y)=X\\#\\#_'")
#  elseif("${CMAKE_Fortran_COMPILER_NAME}" STREQUAL "g77")
#    add_definitions("-D'F77_FUNC(X,Y)=X\\#\\#_' -D'F77_FUNC_(X,Y)=X\\#\\#__'")
#  else("${CMAKE_Fortran_COMPILER_NAME}" STREQUAL "gfortran")
#    message(FATAL_ERROR "unsupported fortran compiler ${CMAKE_Fortran_COMPILER_NAME}")
#  endif("${CMAKE_Fortran_COMPILER_NAME}" STREQUAL "gfortran")
# endif(HAVE_FORTRAN)

