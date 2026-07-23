add_executable(tfel-config tfel-config.cxx)
# type of architecture
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  target_compile_definitions(tfel-config PUBLIC "TFEL_ARCH64")
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  target_compile_definitions(tfel-config PUBLIC "TFEL_ARCH32")
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
target_include_directories(tfel-config
  PRIVATE ${PROJECT_BINARY_DIR}/include
  PRIVATE ${PROJECT_BINARY_DIR}/tfel-config/include
  PRIVATE ${PROJECT_SOURCE_DIR}/include)

set_property(TARGET tfel-config PROPERTY POSITION_INDEPENDENT_CODE TRUE)

if(TFEL_HAVE_MADNEX)
  target_compile_definitions(tfel-config
    PUBLIC TFEL_HAVE_MADNEX)
endif(TFEL_HAVE_MADNEX)

if(enable-tdls)
  target_compile_definitions(tfel-config
    PUBLIC TFEL_MATH_TDLS_SUPPORT)
  set(TFEL_TDLS_INCLUDEDIR $<TARGET_PROPERTY:tdls::tdls,INTERFACE_INCLUDE_DIRECTORIES>)
  target_compile_definitions(tfel-config
    PUBLIC "TFEL_TDLS_INCLUDEDIR=\"${TFEL_TDLS_INCLUDEDIR}\"")
endif(enable-tdls)

if(enable-python-bindings)
  target_compile_definitions(tfel-config
    PUBLIC TFEL_PYTHON_BINDINGS)
endif(enable-python-bindings)

if(WIN32 AND enable-static)
  set_target_properties(tfel-config PROPERTIES COMPILE_FLAGS "-DTFEL_STATIC_BUILD")
  set_target_properties(tfel-config PROPERTIES LINK_FLAGS "-static-libgcc -static") 
  set_target_properties(tfel-config PROPERTIES LINK_SEARCH_END_STATIC ON) 
endif(WIN32 AND enable-static)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(tfel-config
    PROPERTIES OUTPUT_NAME "tfel-config-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)

install(TARGETS tfel-config DESTINATION bin
  COMPONENT core)
