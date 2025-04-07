std_python_module(_stl_vector vector.cxx)
target_link_libraries(py_std__stl_vector
  PRIVATE ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})
std_python_module(_stl_map    map.cxx)
target_link_libraries(py_std__stl_map
  PRIVATE ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})
std_python_module(_stl_pair   pair.cxx)
target_link_libraries(py_std__stl_pair
  PRIVATE ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})
std_python_module(_stl_tuple  tuple.cxx)
target_link_libraries(py_std__stl_tuple
  PRIVATE ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_script(std __init__.py)
