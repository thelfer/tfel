if(TFEL_APPEND_SUFFIX)
  if(unversioned-python-module-names)
    set(TFEL_PYTHON_MODULES_APPEND_SUFFIX OFF)
  else(unversioned-python-module-names)
    set(TFEL_PYTHON_MODULES_APPEND_SUFFIX ON)
    string(REPLACE "." "_" TFEL_SUFFIX_FOR_PYTHON_MODULES "${TFEL_SUFFIX}")
    string(REPLACE "-" "_" TFEL_SUFFIX_FOR_PYTHON_MODULES "${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
    tfel_add_c_cxx_definitions("TFEL_SUFFIX_FOR_PYTHON_MODULES=${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
  endif(unversioned-python-module-names)
else(TFEL_APPEND_SUFFIX)
  set(TFEL_PYTHON_MODULES_APPEND_SUFFIX OFF)
endif(TFEL_APPEND_SUFFIX)

function(python_module_base fullname name)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "python_lib_module : no source specified")
  endif(${ARGC} LESS 1)
  add_library(py_${fullname} MODULE ${ARGN})
  target_link_libraries(py_${fullname} PRIVATE
    pybind11::module pybind11::windows_extras)
  if(TARGET pybind11::lto)
    target_link_libraries(py_${fullname} PRIVATE pybind11::lto)
  endif(TARGET pybind11::lto)
  pybind11_extension(py_${fullname})
  if(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
    # Strip unnecessary sections of the binary on Linux/macOS
    pybind11_strip(py_${fullname})
  endif()
  set_target_properties(py_${fullname} PROPERTIES PREFIX "")
  if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    set_target_properties(py_${fullname} PROPERTIES OUTPUT_NAME "${name}_${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
  else(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    set_target_properties(py_${fullname} PROPERTIES OUTPUT_NAME ${name})
  endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
  set_target_properties(py_${fullname} PROPERTIES
    CXX_VISIBILITY_PRESET "hidden"
    CUDA_VISIBILITY_PRESET "hidden")
endfunction(python_module_base)

function(python_lib_module name package)
  python_module_base(${package}_${name} ${name} ${ARGN})
  if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    install(TARGETS py_${package}_${name}
      DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/${package}_${TFEL_SUFFIX_FOR_PYTHON_MODULES}
      COMPONENT python_bindings)
  else(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    install(TARGETS py_${package}_${name}
       DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/${package}
       COMPONENT python_bindings)
  endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
endfunction(python_lib_module)

function(tfel_python_module name)
  python_lib_module(${name} tfel ${ARGN})
  if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    set_target_properties(py_tfel_${name} PROPERTIES
                          LIBRARY_OUTPUT_DIRECTORY
                          ${PROJECT_BINARY_DIR}/bindings/python/tfel_${TFEL_SUFFIX_FOR_PYTHON_MODULES})
  endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
endfunction(tfel_python_module)

function(tfel_material_python_module name)
  python_module_base(tfel_material_${name} ${name} ${ARGN})
  if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    install(TARGETS py_tfel_material_${name}
      DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/tfel_${TFEL_SUFFIX_FOR_PYTHON_MODULES}/material
      COMPONENT python_bindings)
  else(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    install(TARGETS py_tfel_material_${name}
       DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/tfel/material
       COMPONENT python_bindings)
  endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
  if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    set_target_properties(py_tfel_material_${name} PROPERTIES
                          LIBRARY_OUTPUT_DIRECTORY
                          ${PROJECT_BINARY_DIR}/bindings/python/tfel_${TFEL_SUFFIX_FOR_PYTHON_MODULES}/material)
  endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
endfunction(tfel_material_python_module)

function(mfront_python_module name)
  python_lib_module(${name} mfront ${ARGN})
  set(fullname "mfront_${name}")
  target_include_directories(py_${fullname}
    PRIVATE "${PROJECT_SOURCE_DIR}/mfront/include")
  if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    set_target_properties(py_mfront_${name} PROPERTIES
                          LIBRARY_OUTPUT_DIRECTORY
                          ${PROJECT_BINARY_DIR}/bindings/python/mfront_${TFEL_SUFFIX_FOR_PYTHON_MODULES})
  endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
endfunction(mfront_python_module)

function(mfront_database_python_module name)
  python_module_base(mfront_database_${name} ${name} ${ARGN})
  set(fullname "mfront_database_${name}")
  target_include_directories(py_${fullname}
    PRIVATE "${PROJECT_SOURCE_DIR}/mfront/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/include")
  if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    install(TARGETS py_mfront_database_${name}
      DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/mfront_${TFEL_SUFFIX_FOR_PYTHON_MODULES}/database
      COMPONENT python_bindings)
  else(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    install(TARGETS py_mfront_database_${name}
       DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/mfront/database
       COMPONENT python_bindings)
  endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
  if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    set_target_properties(py_mfront_database_${name} PROPERTIES
                          LIBRARY_OUTPUT_DIRECTORY
                          ${PROJECT_BINARY_DIR}/bindings/python/mfront_${TFEL_SUFFIX_FOR_PYTHON_MODULES}/database)
  endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
endfunction(mfront_database_python_module)

function(mtest_python_module name)
  python_lib_module(${name} mtest ${ARGN})
  set(fullname "mtest_${name}")
  target_include_directories(py_${fullname}
    PRIVATE "${PROJECT_SOURCE_DIR}/mtest/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/mfront/include")
  if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    set_target_properties(py_mtest_${name} PROPERTIES
                          LIBRARY_OUTPUT_DIRECTORY
                          ${PROJECT_BINARY_DIR}/bindings/python/mtest_${TFEL_SUFFIX_FOR_PYTHON_MODULES})
  endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
endfunction(mtest_python_module)

function(mfm_test_generator_python_module name)
  python_lib_module(${name} mfm_test_generator ${ARGN})
  set(fullname "mfm_test_generator_${name}")
  target_include_directories(py_${fullname}
    PRIVATE "${PROJECT_SOURCE_DIR}/mfm-test-generator/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/mtest/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/mfront/include")
  if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    set_target_properties(py_mfm_test_generator_${name} PROPERTIES
                          LIBRARY_OUTPUT_DIRECTORY
                          ${PROJECT_BINARY_DIR}/bindings/python/mfm_test_generator_${TFEL_SUFFIX_FOR_PYTHON_MODULES})
  endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
endfunction(mfm_test_generator_python_module)

function(tfel_python_script_base dir)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "tfel_python_script_base : no script specified")
  endif(${ARGC} LESS 1)
  foreach(pyscript ${ARGN})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
      set(_target_directory "${PROJECT_BINARY_DIR}/bindings/python/${dir}")
      if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in")
	  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in"
	                 "${_target_directory}/${pyscript}" @ONLY)
	else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in")
	  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in"
	                 "${_target_directory}/${pyscript}" @ONLY)
	endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in")
      else(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in"
	               "${_target_directory}/${pyscript}" @ONLY)
      endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
      set(python_script "${_target_directory}/${pyscript}")
    else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
      set(python_script "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}")
    endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
    install(PROGRAMS ${python_script}
       DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/${dir}
       COMPONENT python_bindings)
  endforeach(pyscript ${ARGN})
endfunction(tfel_python_script_base)

function(tfel_python_script dir)
  if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    tfel_python_script_base(${dir}_${TFEL_SUFFIX_FOR_PYTHON_MODULES} ${ARGN})
  else(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
    tfel_python_script_base(${dir} ${ARGN})
  endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
endfunction(tfel_python_script)
