if(enable-python-bindings)
  add_subdirectory(python)
endif(enable-python-bindings)

if(enable-fortran-bindings)
  add_subdirectory(fortran77)
  add_subdirectory(fortran90)
endif(enable-fortran-bindings)
