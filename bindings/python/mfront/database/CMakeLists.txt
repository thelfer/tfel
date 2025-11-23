mfront_database_python_module(_mfront_database
  MFrontDatabase.cxx)
target_link_libraries(py_mfront_database__mfront_database
  PRIVATE
  TFELMFrontDatabase
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_script(mfront/database __init__.py)
