if(TFEL_APPEND_VERSION)
  install(FILES usermat.cpp
    DESTINATION share/doc/mfront-${TFEL_VERSION}/ansys
    COMPONENT mfront)
else(TFEL_APPEND_VERSION)
  install(FILES usermat.cpp DESTINATION share/doc/mfront/ansys
    COMPONENT mfront)
endif(TFEL_APPEND_VERSION)