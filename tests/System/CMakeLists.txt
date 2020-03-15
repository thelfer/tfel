# a simple macro
macro(tests_system test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
  	         PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELPhysicalConstants>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_link_libraries(${test_arg} TFELSystem TFELException TFELTests)
endmacro(tests_system)

if(UNIX)
tests_system(process_test_target)
tests_system(process)
tests_system(rwstream)
tests_system(binary_write)
endif(UNIX)

if((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
  tests_system(ThreadPoolTest)
  tests_system(ThreadPoolTest2)
endif((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
