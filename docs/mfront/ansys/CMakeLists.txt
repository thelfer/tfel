if(TFEL_APPEND_SUFFIX)
  install(FILES usermat.cpp
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/ansys
    COMPONENT mfront)
else(TFEL_APPEND_SUFFIX)
  install(FILES usermat.cpp DESTINATION share/doc/mfront/ansys
    COMPONENT mfront)
endif(TFEL_APPEND_SUFFIX)