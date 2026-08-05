configure_file(${PROJECT_SOURCE_DIR}/src/Config/GetTFELVersion.c.in
               ${PROJECT_BINARY_DIR}/src/Config/GetTFELVersion.c)

tfel_library(TFELConfig
  GetInstallPath.cxx
  Substitutions.cxx
  ${PROJECT_BINARY_DIR}/src/Config/GetTFELVersion.c)
target_include_directories(TFELConfig
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>
   PRIVATE
   $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
if(enable-static)
   target_include_directories(TFELConfig-static
     PUBLIC 
     $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
endif(enable-static)

get_property(tfel-substitutions CACHE tfel-substitutions PROPERTY STRINGS)
string(REPLACE ";" "@" TFEL_SUBSTITUTIONS_VALUE "${tfel-substitutions}")
target_compile_definitions(TFELConfig
  PRIVATE "TFEL_SUBSTITUTIONS=\"${TFEL_SUBSTITUTIONS_VALUE}\"")
if(enable-static)
  target_compile_definitions(TFELConfig-static
    PRIVATE "TFEL_SUBSTITUTIONS=\"${TFEL_SUBSTITUTIONS_VALUE}\"")
endif(enable-static)
