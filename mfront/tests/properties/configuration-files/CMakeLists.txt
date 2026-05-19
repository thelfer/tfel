# options to install configuration files
function(install_configuration_file file)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${file}.in 
                 ${CMAKE_CURRENT_BINARY_DIR}/${file} @ONLY)
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
    DESTINATION ${TFEL_DATAROOTDIR}/test-suite/material-properties/generic-parallel/configuration-files
    COMPONENT core)
endfunction(install_configuration_file)

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
     install_configuration_file(config-hip-clang-nvidia-platform.json)
  endif(HIP_PLATFORM STREQUAL "nvidia")
endif(_tfel-enable-generic-parallel-mp-hip-clang-tests)

if(_tfel-enable-generic-parallel-mp-stlpar-parunseq-gcc-tests)
  find_package(TBB REQUIRED)
  install_configuration_file(config-stlpar-parunseq-gcc.json)
endif(_tfel-enable-generic-parallel-mp-stlpar-parunseq-gcc-tests)

if(_tfel-enable-generic-parallel-mp-stlpar-parunseq-clang-tests)
  install_configuration_file(config-stlpar-parunseq-clang.json)
endif(_tfel-enable-generic-parallel-mp-stlpar-parunseq-clang-tests)

if(_tfel-enable-generic-parallel-mp-stlpar-parunseq-nvhpc-gpu-tests)
  install_configuration_file(config-stlpar-parunseq-nvhpc-gpu.json)
endif(_tfel-enable-generic-parallel-mp-stlpar-parunseq-nvhpc-gpu-tests)

if(_tfel-enable-generic-parallel-mp-stlpar-parunseq-icpx-tests)
  install_configuration_file(config-stlpar-parunseq-icpx.json)
endif(_tfel-enable-generic-parallel-mp-stlpar-parunseq-icpx-tests)

if(_tfel-enable-generic-parallel-mp-sycl-gpu-icpx-tests)
  install_configuration_file(config-sycl-gpu-icpx.json)
endif(_tfel-enable-generic-parallel-mp-sycl-gpu-icpx-tests)