if(TFEL_APPEND_SUFFIX)
  install(FILES mfront-lsdyna.cxx
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/lsdyna
    COMPONENT mfront)
  install(FILES test-lsdyna.cxx
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/lsdyna
    COMPONENT mfront)
  install(FILES CMakeLists-example.txt
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/lsdyna/
    RENAME CMakeLists.txt
    COMPONENT mfront)
else(TFEL_APPEND_SUFFIX)
  install(FILES mfront-lsdyna.cxx DESTINATION share/doc/mfront/lsdyna
    COMPONENT mfront)
  install(FILES test-lsdyna.cxx DESTINATION share/doc/mfront/lsdyna
    COMPONENT mfront)
  install(FILES CMakeLists-example.txt DESTINATION share/doc/mfront/lsdyna/
    RENAME CMakeLists.txt
    COMPONENT mfront)
endif(TFEL_APPEND_SUFFIX)
