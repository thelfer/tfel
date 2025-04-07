tfel_library(TFELConfig GetInstallPath.cxx)
target_include_directories(TFELConfig
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>
   PRIVATE
   $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src/TFELConfig>)
