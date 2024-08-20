tfel_library(TFELUnicodeSupport UnicodeSupport.cxx)
target_include_directories(TFELUnicodeSupport
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
if(enable-static)
   target_include_directories(TFELUnicodeSupport-static
     PUBLIC 
     $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
     $<INSTALL_INTERFACE:include>)
endif(enable-static)
