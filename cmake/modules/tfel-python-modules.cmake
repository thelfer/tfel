function(python_module_base fullname name)
    if(${ARGC} LESS 1)
    message(FATAL_ERROR "python_lib_module : no source specified")
  endif(${ARGC} LESS 1)
  add_library(py_${fullname} MODULE ${ARGN})
  if(WIN32)
    set_target_properties(py_${fullname} PROPERTIES
      COMPILE_FLAGS "-DHAVE_ROUND")
    set_target_properties(py_${fullname} PROPERTIES SUFFIX ".pyd")
  elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(py_${fullname} PROPERTIES SUFFIX ".so")
  endif(WIN32)
  set_target_properties(py_${fullname} PROPERTIES PREFIX "")
  set_target_properties(py_${fullname} PROPERTIES OUTPUT_NAME ${name})
  if(enable-pybind11)
    target_include_directories(py_${fullname}
      PRIVATE "${PROJECT_SOURCE_DIR}/bindings/python/pybind11/include"
      PRIVATE "${PROJECT_SOURCE_DIR}/include")
    target_link_libraries(py_${fullname} PRIVATE
      pybind11::module pybind11::windows_extras)
    if(TARGET pybind11::lto)
      target_link_libraries(py_${fullname} PRIVATE pybind11::lto)
    endif(TARGET pybind11::lto)
    pybind11_extension(py_${fullname})
    if(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
      # Strip unnecessary sections of the binary on Linux/macOS
      pybind11_strip(py_${fullname})
    endif(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
  else(enable-pybind11)
    target_include_directories(py_${fullname}
      PRIVATE "${PROJECT_SOURCE_DIR}/bindings/python/boost/include"
      PRIVATE "${PROJECT_SOURCE_DIR}/include")
    target_include_directories(py_${fullname}
      SYSTEM
      PRIVATE "${Boost_INCLUDE_DIRS}"
      PRIVATE "${Python_INCLUDE_DIRS}")
  endif(enable-pybind11)
  if(python-static-interpreter-workaround)
    if(APPLE)
      target_link_options(py_${fullname}
        PRIVATE "-undefined" "dynamic_lookup")
    endif(APPLE)
  endif(python-static-interpreter-workaround)
endfunction(python_module_base)

function(python_lib_module name package)
  python_module_base(${package}_${name} ${name} ${ARGN})
  if(TFEL_APPEND_SUFFIX)
    install(TARGETS py_${package}_${name}
      DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/${package}_${TFEL_SUFFIX_FOR_PYTHON_MODULES}
      COMPONENT python_bindings)
  else(TFEL_APPEND_SUFFIX)
    install(TARGETS py_${package}_${name}
       DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/${package}
       COMPONENT python_bindings)
  endif(TFEL_APPEND_SUFFIX)
endfunction(python_lib_module)

function(std_python_module name)
  python_lib_module(${name} std ${ARGN})
endfunction(std_python_module)

function(tfel_python_module name)
  python_lib_module(${name} tfel ${ARGN})
endfunction(tfel_python_module)

function(mfront_python_module name)
  python_lib_module(${name} mfront ${ARGN})
  set(fullname "mfront_${name}")
  target_include_directories(py_${fullname}
    PRIVATE "${PROJECT_SOURCE_DIR}/mfront/include")
endfunction(mfront_python_module)

function(mtest_python_module name)
  python_lib_module(${name} mtest ${ARGN})
  set(fullname "mtest_${name}")
  target_include_directories(py_${fullname}
    PRIVATE "${PROJECT_SOURCE_DIR}/mtest/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/mfront/include")
endfunction(mtest_python_module)

function(mfm_test_generator_python_module name)
  python_lib_module(${name} mfm_test_generator ${ARGN})
  set(fullname "mfm_test_generator_${name}")
  target_include_directories(py_${fullname}
    PRIVATE "${PROJECT_SOURCE_DIR}/mfm-test-generator/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/mtest/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/mfront/include")
endfunction(mfm_test_generator_python_module)

function(tfel_python_script_base dir)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "tfel_python_script_base : no script specified")
  endif(${ARGC} LESS 1)
  foreach(pyscript ${ARGN})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
      if(TFEL_APPEND_SUFFIX)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in")
	  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in"
	    "${CMAKE_CURRENT_BINARY_DIR}/${pyscript}" @ONLY)
	else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in")
	  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in"
	    "${CMAKE_CURRENT_BINARY_DIR}/${pyscript}" @ONLY)
	endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.version.in")
      else(TFEL_APPEND_SUFFIX)
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in"
	  "${CMAKE_CURRENT_BINARY_DIR}/${pyscript}" @ONLY)
      endif(TFEL_APPEND_SUFFIX)
      set(python_script "${CMAKE_CURRENT_BINARY_DIR}/${pyscript}")
    else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
      set(python_script "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}")
    endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
    install(PROGRAMS ${python_script}
       DESTINATION ${TFEL_PYTHON_SITE_PACKAGES_DIR}/${dir}/
       COMPONENT python_bindings)
  endforeach(pyscript ${ARGN})
endfunction(tfel_python_script_base)

function(tfel_python_script dir)
  if(TFEL_APPEND_SUFFIX)
    tfel_python_script_base(${dir}_${TFEL_SUFFIX_FOR_PYTHON_MODULES} ${ARGN})
  else(TFEL_APPEND_SUFFIX)
    tfel_python_script_base(${dir} ${ARGN})
  endif(TFEL_APPEND_SUFFIX)
endfunction(tfel_python_script)
