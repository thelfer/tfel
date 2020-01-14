std_python_module(_stl_vector vector.cxx)
std_python_module(_stl_map    map.cxx)
std_python_module(_stl_pair   pair.cxx)
std_python_module(_stl_tuple  tuple.cxx)

tfel_python_script(std __init__.py)
