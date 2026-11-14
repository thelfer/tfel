mfront_database_python_module(_mfront_database
  MFrontDatabase.cxx)
target_link_libraries(py_mfront_database__mfront_database
  PRIVATE
  TFELMFrontDatabase
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
  tfel_python_script_base(mfront_${TFEL_SUFFIX_FOR_PYTHON_MODULES}/database __init__.py)
else(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
  tfel_python_script_base(mfront/database __init__.py)
endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
