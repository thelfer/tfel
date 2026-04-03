macro(install_mtest_desc2 file)
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${file}.md
      DESTINATION share/doc/mtest-${TFEL_SUFFIX}/mtest
      COMPONENT mtest)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}.md
      DESTINATION share/doc/mtest/mtest
      COMPONENT mtest)
  endif(TFEL_APPEND_SUFFIX)
endmacro(install_mtest_desc2 file)

install_mtest_desc2(CohesiveForce)
install_mtest_desc2(CohesiveForceEpsilon)
install_mtest_desc2(DeformationGradient)
install_mtest_desc2(DeformationGradientEpsilon)
install_mtest_desc2(Event)
install_mtest_desc2(ImposedDeformationGradient)
install_mtest_desc2(ImposedStrain)
install_mtest_desc2(ImposedStress)
install_mtest_desc2(ModellingHypothesis)
install_mtest_desc2(PrintLagrangeMultipliers)
install_mtest_desc2(NonLinearConstraint)
install_mtest_desc2(RotationMatrix)
install_mtest_desc2(Strain)
install_mtest_desc2(StrainEpsilon)
install_mtest_desc2(Stress)
install_mtest_desc2(StressEpsilon)
install_mtest_desc2(Test)
