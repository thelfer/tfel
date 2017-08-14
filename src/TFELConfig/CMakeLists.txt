include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_BINARY_DIR}/src/TFELConfig")

configure_file(${PROJECT_SOURCE_DIR}/src/TFELConfig/tfel-config.hxx.in
               ${PROJECT_BINARY_DIR}/src/TFELConfig/tfel-config.hxx)

add_executable(tfel-config tfel-config.cxx)

if(TFEL_APPEND_VERSION)
  set_target_properties(tfel-config
    PROPERTIES OUTPUT_NAME "tfel-config-${TFEL_VERSION}")
endif(TFEL_APPEND_VERSION)
if(WIN32 AND enable-static)
  set_target_properties(tfel-config PROPERTIES COMPILE_FLAGS "-DTFEL_STATIC_BUILD")
  set_target_properties(tfel-config PROPERTIES LINK_FLAGS "-static-libgcc -static") 
  set_target_properties(tfel-config PROPERTIES LINK_SEARCH_END_STATIC ON) 
endif(WIN32 AND enable-static)

install(TARGETS tfel-config DESTINATION bin
  COMPONENT core)
