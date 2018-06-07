if(TFEL_APPEND_SUFFIX)
  install(FILES usermat.cpp
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/ansys
    COMPONENT mfront)
  install(FILES test-usermat.cxx
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/ansys
    COMPONENT mfront)
  install(FILES CMakeLists-example.txt
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/ansys/
    RENAME CMakeLists.txt
    COMPONENT mfront)
else(TFEL_APPEND_SUFFIX)
  install(FILES usermat.cpp DESTINATION share/doc/mfront/ansys
    COMPONENT mfront)
  install(FILES test-usermat.cxx DESTINATION share/doc/mfront/ansys
    COMPONENT mfront)
  install(FILES CMakeLists-example.txt DESTINATION share/doc/mfront/ansys/
    RENAME CMakeLists.txt
    COMPONENT mfront)
endif(TFEL_APPEND_SUFFIX)