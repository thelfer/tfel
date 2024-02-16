include_directories("${PROJECT_BINARY_DIR}/include"
  "${PROJECT_BINARY_DIR}/tfel-config/include"
  "${PROJECT_SOURCE_DIR}/include")

add_executable(tfel-config tfel-config.cxx)
set_property(TARGET tfel-config PROPERTY POSITION_INDEPENDENT_CODE TRUE)
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
