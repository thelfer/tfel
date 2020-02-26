function(test_pymfront test)
  configure_file(pymfront${test}.py.in
    pymfront${test}.py @ONLY)
  add_test(NAME pymfront${test}_py
    COMMAND ${PYTHON_EXECUTABLE} pymfront${test}.py)
  if(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(pymfront${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python;$<TARGET_FILE_DIR:py_std__stl_vector>;$<TARGET_FILE_DIR:py_tfel_tests;$<TARGET_FILE_DIR:py_mfront__mfront>")
  else(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(pymfront${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python")
  endif(CMAKE_CONFIGURATION_TYPES)
endfunction(test_pymfront)

function(test_pymfront_mp interface library function)
  add_test(NAME pymfront_material_properties_${interface}_py
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/material-properties.py)
  if(CMAKE_CONFIGURATION_TYPES)
    set_property(TEST pymfront_material_properties_${interface}_py
      PROPERTY ENVIRONMENT
      PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python;$<TARGET_FILE_DIR:py_std__stl_vector>;$<TARGET_FILE_DIR:py_tfel_tests;$<TARGET_FILE_DIR:py_mfront__mfront>
      MFRONT_MATERIAL_PROPERTY_LIBRARY=${library}
      MFRONT_MATERIAL_PROPERTY_FUNCTION=${function})
  else(CMAKE_CONFIGURATION_TYPES)
    set_property(TEST pymfront_material_properties_${interface}_py
      PROPERTY ENVIRONMENT
      PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python
      MFRONT_MATERIAL_PROPERTY_LIBRARY=${library}
      MFRONT_MATERIAL_PROPERTY_FUNCTION=${function})      
  endif(CMAKE_CONFIGURATION_TYPES)
endfunction(test_pymfront_mp)

if(HAVE_ASTER)
  test_pymfront(test1)
endif(HAVE_ASTER)
test_pymfront(test2)
test_pymfront(test3)


test_pymfront_mp("c" "$<TARGET_FILE:MFrontMaterialProperties>" "VanadiumAlloy_YoungModulus_SRMA")
test_pymfront_mp("cxx" "$<TARGET_FILE:MFrontMaterialProperties-cxx>" "VanadiumAlloy_YoungModulus_SRMA")
if(HAVE_FORTRAN)
 test_pymfront_mp("fortran" "$<TARGET_FILE:MFrontMaterialProperties-fortran>" "VanadiumAlloy_YoungModulus_SRMA")
endif(HAVE_FORTRAN)
if(HAVE_CASTEM)
 test_pymfront_mp("castem" "$<TARGET_FILE:MFrontMaterialProperties-castem>" "VanadiumAlloy_YoungModulus_SRMA")
endif(HAVE_CASTEM)
