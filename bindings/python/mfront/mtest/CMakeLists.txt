if(TFEL_APPEND_VERSION)
  tfel_python_script_base(mfront_${TFEL_VERSION_FOR_PYTHON_MODULES}/mtest __init__.py)
else(TFEL_APPEND_VERSION)
  tfel_python_script_base(mfront/mtest __init__.py)
endif(TFEL_APPEND_VERSION)

  