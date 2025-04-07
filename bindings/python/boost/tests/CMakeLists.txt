if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
  set(tfel_system_python_module "tfel_${TFEL_SUFFIX_FOR_PYTHON_MODULES}.system_${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
  set(tfel_material_python_module "tfel_${TFEL_SUFFIX_FOR_PYTHON_MODULES}.material_${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
  set(mfront_python_module "mfront_${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
  set(mtest_python_module "mtest_${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
else(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
  set(tfel_system_python_module "tfel.system")
  set(tfel_material_python_module "tfel.material")
  set(mfront_python_module "mfront")
  set(mtest_python_module "mtest")
endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)

configure_file(behaviour-constructors.py.in behaviour-constructors.py @ONLY)
configure_file(small-strain-tridimensional-behaviour-wrapper.py.in
               small-strain-tridimensional-behaviour-wrapper.py @ONLY)
configure_file(material-properties.py.in material-properties.py @ONLY)

function(test_pymfront test)
  if(NOT CMAKE_CONFIGURATION_TYPES)
    configure_file(pymfront${test}.py.in
      pymfront${test}.py @ONLY)
    add_test(NAME pymfront${test}_py
      COMMAND ${Python_EXECUTABLE} pymfront${test}.py)
    set_tests_properties(pymfront${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python/boost")
  endif(NOT CMAKE_CONFIGURATION_TYPES)
endfunction(test_pymfront)

function(test_pymfront_mp interface library function)
  if(NOT CMAKE_CONFIGURATION_TYPES)
    add_test(NAME pymfront_material_properties_${interface}_py
      COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/material-properties.py)
    set_property(TEST pymfront_material_properties_${interface}_py
      PROPERTY ENVIRONMENT
      PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python/boost
      MFRONT_MATERIAL_PROPERTY_LIBRARY=${library}
      MFRONT_MATERIAL_PROPERTY_FUNCTION=${function})      
  endif(NOT CMAKE_CONFIGURATION_TYPES)
endfunction(test_pymfront_mp)

function(test_pymtest_bv file library)
  if(NOT CMAKE_CONFIGURATION_TYPES)
    add_test(NAME pymtest_behaviours_${file}_py
      COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/${file}.py)
    set_property(TEST pymtest_behaviours_${file}_py
      PROPERTY ENVIRONMENT
      PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python/boost
      MTEST_BEHAVIOUR_LIBRARY=${library})      
  endif(NOT CMAKE_CONFIGURATION_TYPES)
endfunction(test_pymtest_bv)

if(HAVE_ASTER)
  test_pymfront(test1)
endif(HAVE_ASTER)
test_pymfront(test2)
test_pymfront(test3)

test_pymfront_mp("generic" "$<TARGET_FILE:MFrontMaterialProperties-generic>" "VanadiumAlloy_YoungModulus_SRMA")
test_pymfront_mp("c" "$<TARGET_FILE:MFrontMaterialProperties>" "VanadiumAlloy_YoungModulus_SRMA")
test_pymfront_mp("cxx" "$<TARGET_FILE:MFrontMaterialProperties-cxx>" "VanadiumAlloy_YoungModulus_SRMA")
if(HAVE_FORTRAN)
 test_pymfront_mp("fortran" "$<TARGET_FILE:MFrontMaterialProperties-fortran>" "VanadiumAlloy_YoungModulus_SRMA")
endif(HAVE_FORTRAN)
if(HAVE_CASTEM)
 test_pymfront_mp("castem" "$<TARGET_FILE:MFrontMaterialProperties-castem>" "VanadiumAlloy_YoungModulus_SRMA")
endif(HAVE_CASTEM)

test_pymtest_bv(behaviour-constructors "$<TARGET_FILE:MFrontGenericBehaviours>")
test_pymtest_bv(small-strain-tridimensional-behaviour-wrapper "$<TARGET_FILE:MFrontGenericBehaviours>")
