include_directories("${PROJECT_SOURCE_DIR}/bindings/python/include")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/mfront/include")
include_directories("${Boost_INCLUDE_DIRS}")
include_directories("${PYTHON_INCLUDE_DIRS}")

link_directories("${PROJECT_BINARY_DIR}/mfront/mtest")
link_directories("${PROJECT_BINARY_DIR}/mfront/src")
link_directories("${PROJECT_BINARY_DIR}/src/Exception")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")
link_directories("${PROJECT_BINARY_DIR}/src/Math")
link_directories("${PROJECT_BINARY_DIR}/src/System")
link_directories("${PROJECT_BINARY_DIR}/src/Tests")
link_directories("${Boost_LIBRARY_DIRS}")
link_directories("${PYTHON_LIBRARY_PATH}")

mfront_python_module(mtest
  pymtest.cxx
  MTest.cxx
  MTestParser.cxx
  MTestLogStream.cxx)
target_link_libraries(py_mfront_mtest TFELMTest)

tfel_python_script(mfront __init__.py)
