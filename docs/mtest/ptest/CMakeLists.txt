macro(install_ptest file)
  install(FILES ${file}.md DESTINATION share/doc/mtest/ptest
    COMPONENT mtest)
endmacro(install_ptest file)

install_ptest(AxialForceEvolution)
install_ptest(AxialGrowthEvolution)
install_ptest(AxialLoading)
install_ptest(RadialLoading)

