add_executable(tfel-unicode-filt tfel-unicode-filt.cxx)
target_include_directories(tfel-unicode-filt
  PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(tfel-unicode-filt
	PRIVATE TFELUnicodeSupport TFELUtilities)

if(WIN32 AND enable-static)
  set_target_properties(tfel-unicode-filt PROPERTIES COMPILE_FLAGS "-DTFEL_STATIC_BUILD")
  set_target_properties(tfel-unicode-filt PROPERTIES LINK_FLAGS "-static-libgcc -static") 
  set_target_properties(tfel-unicode-filt PROPERTIES LINK_SEARCH_END_STATIC ON) 
endif(WIN32 AND enable-static)
if(TFEL_APPEND_SUFFIX)
  set_target_properties(tfel-unicode-filt
    PROPERTIES OUTPUT_NAME "tfel-unicode-filt-${TFEL_SUFFIX}")
endif(TFEL_APPEND_SUFFIX)

install(TARGETS tfel-unicode-filt DESTINATION bin
  COMPONENT core)
