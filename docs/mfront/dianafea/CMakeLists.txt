if(TFEL_APPEND_SUFFIX)
  set(tfel_dianafea_shared_dir "share/doc/mfront-${TFEL_SUFFIX}/dianafea")
else()
  set(tfel_dianafea_shared_dir "share/doc/mfront/dianafea")
endif()
install(FILES
        dianafea_external_behaviour_call.cxx
        usrmat.f90
        test-usrmat.cxx
        test.f90
  DESTINATION ${tfel_dianafea_shared_dir}
  COMPONENT mfront)
install(FILES CMakeLists-dianafea.txt
  DESTINATION ${tfel_dianafea_shared_dir}
  RENAME CMakeLists.txt
  COMPONENT mfront)
