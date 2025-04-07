if(TFEL_PYTHON_MODULES_APPEND_VERSION)
  set(tfel_system_python_module "tfel_${TFEL_VERSION_FOR_PYTHON_MODULES}.system_${TFEL_VERSION_FOR_PYTHON_MODULES}")
  set(tfel_material_python_module "tfel_${TFEL_VERSION_FOR_PYTHON_MODULES}.material_${TFEL_VERSION_FOR_PYTHON_MODULES}")
  set(mfront_python_module "mfront_${TFEL_VERSION_FOR_PYTHON_MODULES}")
  set(mtest_python_module "mtest_${TFEL_VERSION_FOR_PYTHON_MODULES}")
else(TFEL_PYTHON_MODULES_APPEND_VERSION)
  set(tfel_system_python_module "tfel.system")
  set(tfel_material_python_module "tfel.material")
  set(mfront_python_module "mfront")
  set(mtest_python_module "mtest")
endif(TFEL_PYTHON_MODULES_APPEND_VERSION)

function(test_pymfront test)
  if(NOT CMAKE_CONFIGURATION_TYPES)
    configure_file(pymfront${test}.py.in
      pymfront${test}.py @ONLY)
    add_test(NAME pymfront${test}_py
      COMMAND ${Python_EXECUTABLE} pymfront${test}.py)
    set_tests_properties(pymfront${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python")
  endif(NOT CMAKE_CONFIGURATION_TYPES)
endfunction(test_pymfront)

if(HAVE_ASTER)
  test_pymfront(test1)
endif(HAVE_ASTER)
test_pymfront(test2)
test_pymfront(test3)
