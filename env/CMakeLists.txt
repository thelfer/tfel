if(UNIX)
  if(enable-python-bindings)
    configure_file(env-python.sh.in env.sh @ONLY)
  else(enable-python-bindings) 
   configure_file(env.sh.in env.sh @ONLY)
  endif(enable-python-bindings)
  if(TFEL_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/env.sh
            DESTINATION share/tfel-${TFEL_SUFFIX}/env
            COMPONENT core)
  else(TFEL_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/env.sh
            DESTINATION share/tfel/env
            COMPONENT core)
  endif(TFEL_SUFFIX)
endif(UNIX)

if(WIN32)
  cmake_path(NATIVE_PATH TFEL_PYTHON_SITE_PACKAGES_DIR NATIVE_TFEL_PYTHON_SITE_PACKAGES_DIR)
  if(enable-python-bindings)
    configure_file(env-python.ps1.in env.ps1 @ONLY)
    configure_file(env-python.bat.in env.bat @ONLY)
  else(enable-python-bindings) 
   configure_file(env.ps1.in env.ps1 @ONLY)
   configure_file(env.bat.in env.bat @ONLY)
  endif(enable-python-bindings)
  if(TFEL_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/env.ps1
            DESTINATION share/tfel-${TFEL_SUFFIX}/env
            COMPONENT core)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/env.bat
            DESTINATION share/tfel-${TFEL_SUFFIX}/env
            COMPONENT core)
  else(TFEL_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/env.ps1
            DESTINATION share/tfel/env
            COMPONENT core)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/env.bat
            DESTINATION share/tfel/env
            COMPONENT core)
  endif(TFEL_SUFFIX)
endif(WIN32)