include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_BINARY_DIR}/src/TFELConfig")

configure_file(${PROJECT_SOURCE_DIR}/src/TFELConfig/tfel-config.hxx.in
               ${PROJECT_BINARY_DIR}/src/TFELConfig/tfel-config.hxx)

add_executable(tfel_config tfel-config.cxx)
install(TARGETS tfel_config DESTINATION bin)
