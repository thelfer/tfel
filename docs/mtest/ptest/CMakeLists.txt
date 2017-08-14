macro(install_ptest file)
  if(TFEL_APPEND_VERSION)
    install(FILES ${file}.md
      DESTINATION share/doc/mtest-${TFEL_VERSION}/ptest
      COMPONENT mtest)
  else(TFEL_APPEND_VERSION)
    install(FILES ${file}.md DESTINATION share/doc/mtest/ptest
      COMPONENT mtest)
  endif(TFEL_APPEND_VERSION)
endmacro(install_ptest file)

install_ptest(AxialForceEvolution)
install_ptest(AxialGrowthEvolution)
install_ptest(AxialLoading)
install_ptest(RadialLoading)

