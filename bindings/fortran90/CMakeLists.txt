add_library(TFELPhysicalConstantsFortran90Bindings tfel_physical_constants.f90)
set_target_properties(TFELPhysicalConstantsFortran90Bindings
  PROPERTIES Fortran_MODULE_DIRECTORY "TFEL")
if(TFEL_APPEND_SUFFIX)  
  install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/TFEL"
    DESTINATION "include/TFEL-${TFEL_SUFFIX}"
    COMPONENT core)
else(TFEL_APPEND_SUFFIX)
  install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/TFEL"
    DESTINATION "include"
    COMPONENT core)
endif(TFEL_APPEND_SUFFIX)
