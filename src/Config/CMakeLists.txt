include_directories("${PROJECT_BINARY_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/include")

configure_file(${PROJECT_SOURCE_DIR}/src/Config/GetTFELVersion.c.in
               ${PROJECT_BINARY_DIR}/src/Config/GetTFELVersion.c)

tfel_library(TFELConfig
  GetInstallPath.cxx
  ${PROJECT_BINARY_DIR}/src/Config/GetTFELVersion.c)

