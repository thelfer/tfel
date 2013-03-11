include_directories("${PROJECT_SOURCE_DIR}/bindings/python/include")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${Boost_INCLUDE_DIRS}")
include_directories("${PYTHON_INCLUDE_DIRS}")

link_directories("${Boost_LIBRARY_DIRS}")
link_directories("${PYTHON_LIBRARY_PATH}")

std_python_module(_stl_vector vector.cxx)
std_python_module(_stl_map    map.cxx)
std_python_module(_stl_pair   pair.cxx)
