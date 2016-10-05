include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/")

# # a simple macro
# macro(tests_check test_arg)
#   add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
#   add_test(NAME ${test_arg} COMMAND ${test_arg})
#   add_dependencies(check ${test_arg})
#   if((CMAKE_HOST_WIN32) AND (NOT MSYS))
#     set_property(TEST ${test_arg}
#   	         PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMath>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
#   endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
#   target_link_libraries(${test_arg} TFELCheck)
# endmacro(tests_check)

# test_check(test1)
# test_check(test2)
# test_check(test3)
# test_check(test4)
# test_check(test5)
# test_check(test6)
