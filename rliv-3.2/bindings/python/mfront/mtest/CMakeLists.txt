if(TFEL_APPEND_SUFFIX)
  tfel_python_script_base(mfront_${TFEL_SUFFIX_FOR_PYTHON_MODULES}/mtest __init__.py)
else(TFEL_APPEND_SUFFIX)
  tfel_python_script_base(mfront/mtest __init__.py)
endif(TFEL_APPEND_SUFFIX)

  