include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(TFELNUMODIS SHARED
  BCC.cxx
  Crystallo.cxx
  Cubic.cxx
  FCC.cxx
  GSystem.cxx
  Hardening.cxx
  HCP.cxx
  IBurgers.cxx
  IDirection.cxx
  IPlane.cxx
  Utilities.cxx
  Vect3.cxx)