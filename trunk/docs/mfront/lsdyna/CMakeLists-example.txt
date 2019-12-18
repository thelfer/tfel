include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(test-lsdyna test-lsdyna.cxx)
if(UNIX)
  target_link_libraries(test-lsdyna dl)
endif(UNIX)
