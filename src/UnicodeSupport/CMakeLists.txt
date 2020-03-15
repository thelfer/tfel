tfel_library(TFELUnicodeSupport UnicodeSupport.cxx)
target_include_directories(TFELUnicodeSupport
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
