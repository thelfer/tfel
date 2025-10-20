cmake_minimum_required(VERSION 3.0)
project(MFrontDianaFEAInterface
        LANGUAGES CXX Fortran)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


set(CMAKE_CXX_STANDARD 11)

add_library(usrmat SHARED
  usrmat.f90
  dianafea_external_behaviour_call.cxx)

add_executable(test-usrmat test-usrmat.cxx)
target_include_directories(test-usrmat
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(test-usrmat-f90 test.f90)
target_link_libraries(test-usrmat-f90
  PRIVATE usrmat)

if(UNIX)
  target_link_libraries(usrmat dl)
  target_link_libraries(test-usrmat dl)
endif(UNIX)


