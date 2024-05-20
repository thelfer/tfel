set(TFELException_SRC TFELException.cxx ContractViolation.cxx)
tfel_library(TFELException ${TFELException_SRC})
target_include_directories(TFELException
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
if(enable-static)
   target_include_directories(TFELException-static
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
endif(enable-static)
