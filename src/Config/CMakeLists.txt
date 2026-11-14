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

# the substitutions declared by add_tfel_substitution are written as
# string literals in a generated header, the backslashes and the double
# quotes of the values being escaped
set(TFEL_SUBSTITUTIONS_INITIALIZERS "")
get_property(tfel-substitutions-keys GLOBAL PROPERTY TFEL_SUBSTITUTIONS_KEYS)
foreach(key ${tfel-substitutions-keys})
  get_property(value GLOBAL PROPERTY TFEL_SUBSTITUTION_${key})
  string(REPLACE "\\" "\\\\" value "${value}")
  string(REPLACE "\"" "\\\"" value "${value}")
  string(APPEND TFEL_SUBSTITUTIONS_INITIALIZERS
    "      {\"${key}\", \"${value}\"},\n")
endforeach()
configure_file(${PROJECT_SOURCE_DIR}/src/Config/Substitutions-defines.hxx.in
               ${PROJECT_BINARY_DIR}/include/TFEL/Config/Substitutions-defines.hxx
               @ONLY)
