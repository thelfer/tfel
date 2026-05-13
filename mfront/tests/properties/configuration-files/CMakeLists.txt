# options to install configuration files

function(install_configuration_file file)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${file}.in 
                 ${CMAKE_CURRENT_BINARY_DIR}/${file} @ONLY)
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
    DESTINATION ${TFEL_DATAROOTDIR}/test-suite/generic-parallel/material-properties/configuration-files
    COMPONENT core)
endfunction(install_configuration_file)

set(_tfel-generic-parallel_mp_tests_supported_configurations
    "cuda-nvcc" "cuda-clang" "hip-hipcc" "hip-clang")

foreach(conf ${generic-parallel-mp-tests-configurations})
  if(NOT "${conf}" IN_LIST _tfel-generic-parallel_mp_tests_supported_configurations)
    message(FATAL_ERROR "unsupported configuration '${conf}' for the tests of the generic-parallel interface. "
            "Supported configuration are '${_tfel-generic-parallel_mp_tests_supported_configurations}'")
  endif()
  set(_tfel-enable-generic-parallel-mp-${conf}-tests ON)
endforeach()

set(TFEL_REQUIRES_CUDA OFF)
set(TFEL_REQUIRES_HIP OFF)

if(_tfel-enable-generic-parallel-mp-cuda-nvcc-tests OR
   _tfel-enable-generic-parallel-mp-cuda-clang-tests)
  set(TFEL_REQUIRES_CUDA ON)
endif()

if(_tfel-enable-generic-parallel-mp-hip-hipcc-tests OR
   _tfel-enable-generic-parallel-mp-hip-clang-tests)
  set(TFEL_REQUIRES_HIP ON)
endif()

if(TFEL_REQUIRES_HIP)
 if(NOT HIP_PLATFORM)
   if(NOT DEFINED ENV{HIP_PLATFORM})
     message(FATAL_ERROR "HIP_PLATFORM is not defined")
   endif()
   set(HIP_PLATFORM "$ENV{HIP_PLATFORM}")
 endif(NOT HIP_PLATFORM)
 if(HIP_PLATFORM STREQUAL "nvidia")
   set(TFEL_REQUIRES_CUDA ON)
 elseif(HIP_PLATFORM STREQUAL "amd")
 else()
   message(FATAL_ERROR "unsupported value of HIP_PLATFORM. Expected 'amd' or 'nvidia', got '${HIP_PLATFORM}'")
 endif()
endif(TFEL_REQUIRES_HIP)

if(TFEL_REQUIRES_CUDA)
  find_package(CUDAToolkit REQUIRED)
  if(NOT CMAKE_CUDA_COMPILER)
   set(CMAKE_CUDA_COMPILER "${CUDAToolkit_NVCC_EXECUTABLE}")
  endif()
  message(STATUS "nvcc compiler: ${CMAKE_CUDA_COMPILER}")
  if(CUDAToolkit_LIBRARY_ROOT)
    set(TFEL_CUDA_PATH "${CUDAToolkit_LIBRARY_ROOT}")
  else(CUDAToolkit_LIBRARY_ROOT)
    cmake_path(GET CUDAToolkit_BIN_DIR PARENT_PATH TFEL_CUDA_PATH)
  endif(CUDAToolkit_LIBRARY_ROOT)  
endif()

if(TFEL_REQUIRES_HIP)
  find_package(hip REQUIRED)
  set(TFEL_HIP_INCLUDES "${HIP_INCLUDE_DIR}")
endif(TFEL_REQUIRES_HIP)

if(_tfel-enable-generic-parallel-mp-cuda-nvcc-tests)
  install_configuration_file(config-cuda-nvcc.json)
endif(_tfel-enable-generic-parallel-mp-cuda-nvcc-tests)

if(_tfel-enable-generic-parallel-mp-cuda-clang-tests)
  install_configuration_file(config-cuda-clang.json)
endif(_tfel-enable-generic-parallel-mp-cuda-clang-tests)

if(_tfel-enable-generic-parallel-mp-hip-hipcc-tests)
  install_configuration_file(config-hip-hipcc.json)
endif(_tfel-enable-generic-parallel-mp-hip-hipcc-tests)

if(_tfel-enable-generic-parallel-mp-hip-clang-tests)
  if(HIP_PLATFORM STREQUAL "nvidia")
     install_configuration_file(config-hip-clang-nvida-platform.json)
  endif(HIP_PLATFORM STREQUAL "nvidia")
endif(_tfel-enable-generic-parallel-mp-hip-clang-tests)
