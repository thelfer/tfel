include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")

set(mfront_tests_SOURCES
  Elasticity Plasticity
  Norton NortonRK DDIF2
  Norton_Euler Norton_rk2
  Norton_rk4   Norton_rk42
  Norton_rkCastem
  ImplicitNorton)

mfront_behaviour_check_library(MFrontAsterBehaviours  aster ${mfront_tests_SOURCES})
