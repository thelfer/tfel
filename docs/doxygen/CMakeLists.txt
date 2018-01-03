if(TFEL_APPEND_SUFFIX)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
    DESTINATION share/doc/tfel-${TFEL_SUFFIX}
    COMPONENT doxygen)
  install(FILES
    ${PROJECT_SOURCE_DIR}/docs/web/img/background.svg
    ${PROJECT_SOURCE_DIR}/docs/mfront-en.svg
    ${PROJECT_SOURCE_DIR}/docs/mfront-fr.svg
    DESTINATION share/doc/tfel-${TFEL_SUFFIX}/html/img/
    COMPONENT doxygen)
else(TFEL_APPEND_SUFFIX)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
    DESTINATION share/doc/tfel
    COMPONENT doxygen)
  install(FILES
    ${PROJECT_SOURCE_DIR}/docs/web/img/background.svg
    ${PROJECT_SOURCE_DIR}/docs/mfront-en.svg
    ${PROJECT_SOURCE_DIR}/docs/mfront-fr.svg
    DESTINATION share/doc/tfel/html/img/
    COMPONENT doxygen)
endif(TFEL_APPEND_SUFFIX)