# options to install configuration files
function(install_configuration_file file)
  install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/${file}
    DESTINATION ${TFEL_DATAROOTDIR}/mfront/configuration-files
    COMPONENT core)
endfunction(install_configuration_file)

if(_tfel-enable-generic-parallel-cuda-nvcc-configuration)
  install_configuration_file(config-cuda-nvcc.json)
endif(_tfel-enable-generic-parallel-cuda-nvcc-configuration)

if(_tfel-enable-generic-parallel-cuda-clang-configuration)
  install_configuration_file(config-cuda-clang.json)
endif(_tfel-enable-generic-parallel-cuda-clang-configuration)

if(_tfel-enable-generic-parallel-hip-hipcc-configuration)
  install_configuration_file(config-hip-hipcc.json)
endif(_tfel-enable-generic-parallel-hip-hipcc-configuration)

if(_tfel-enable-generic-parallel-hip-clang-configuration)
  if(HIP_PLATFORM STREQUAL "nvidia")
     install_configuration_file(config-hip-clang-nvidia-platform.json)
  endif(HIP_PLATFORM STREQUAL "nvidia")
endif(_tfel-enable-generic-parallel-hip-clang-configuration)

if(_tfel-enable-generic-parallel-stlpar-parunseq-gcc-configuration)
  find_package(TBB REQUIRED)
  install_configuration_file(config-stlpar-parunseq-gcc.json)
endif(_tfel-enable-generic-parallel-stlpar-parunseq-gcc-configuration)

if(_tfel-enable-generic-parallel-stlpar-parunseq-clang-configuration)
  install_configuration_file(config-stlpar-parunseq-clang.json)
endif(_tfel-enable-generic-parallel-stlpar-parunseq-clang-configuration)

if(_tfel-enable-generic-parallel-stlpar-parunseq-nvhpc-gpu-configuration)
  install_configuration_file(config-stlpar-parunseq-nvhpc-gpu.json)
endif(_tfel-enable-generic-parallel-stlpar-parunseq-nvhpc-gpu-configuration)

if(_tfel-enable-generic-parallel-stlpar-parunseq-icpx-configuration)
  install_configuration_file(config-stlpar-parunseq-icpx.json)
endif(_tfel-enable-generic-parallel-stlpar-parunseq-icpx-configuration)

if(_tfel-enable-generic-parallel-sycl-default-icpx-configuration)
  install_configuration_file(config-sycl-default-icpx.json)
endif(_tfel-enable-generic-parallel-sycl-default-icpx-configuration)

if(_tfel-enable-generic-parallel-sycl-default-acpp-configuration)
  install_configuration_file(config-sycl-default-acpp.json)
endif(_tfel-enable-generic-parallel-sycl-default-acpp-configuration)

# if(_tfel-enable-generic-parallel-stlpar-parunseq-acpp-generic-configuration)
#   install_configuration_file(config-stlpar-parunseq-acpp-generic.json)
# endif(_tfel-enable-generic-parallel-stlpar-parunseq-acpp-generic-configuration)
