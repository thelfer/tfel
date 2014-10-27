macro(install_mtest_desc file)
  install(FILES ${file}.txt DESTINATION share/doc/mtest
    COMPONENT mtest)
endmacro(install_mtest_desc file)

configure_file(Champs.tex.in Champs.tex @ONLY)
configure_file(mtest.tex.in
               mtest.tex @ONLY)

ADD_CUSTOM_COMMAND(
    OUTPUT    mtest.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/Champs.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/mtest.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mtest.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mtest.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse mtest
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mtest.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mtest.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 mtest.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      mtest.ps
    COMMENT   "mtest.pdf")
  
# Eventually trigger the whole process
add_custom_target(mtest-doc-pdf echo DEPENDS mtest.pdf)
add_custom_target(mtest-doc-pdf-install
                  COMMAND ${CMAKE_COMMAND} -E copy mtest.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mtest/mtest.pdf
                  COMMAND ${CMAKE_COMMAND} -E copy mtest.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/tfel/web/documents/mtest/mtest.pdf)
add_dependencies(mtest-doc-pdf-install mtest-doc-pdf)

add_dependencies(doc-pdf
  mtest-doc-pdf)

add_dependencies(doc-pdf-install
  mtest-doc-pdf-install)

install_mtest_desc(Author)
install_mtest_desc(Behaviour)
install_mtest_desc(CastemAccelerationPeriod)
install_mtest_desc(CastemAccelerationTrigger)
install_mtest_desc(Date)
install_mtest_desc(Description)
install_mtest_desc(Evolution)
install_mtest_desc(ExternalStateVariable)
install_mtest_desc(ImposedStrain)
install_mtest_desc(ImposedStress)
install_mtest_desc(InternalStateVariable)
install_mtest_desc(MaterialProperty)
install_mtest_desc(MaximumNumberOfIterations)
install_mtest_desc(MaximumNumberOfSubSteps)
install_mtest_desc(ModellingHypothesis)
install_mtest_desc(Parameter)
install_mtest_desc(IntegerParameter)
install_mtest_desc(UnsignedIntegerParameter)
install_mtest_desc(OutputFile)
install_mtest_desc(OutputFilePrecision)
install_mtest_desc(PredictionPolicy)
install_mtest_desc(Real)
install_mtest_desc(RotationMatrix)
install_mtest_desc(StiffnessMatrixType)
install_mtest_desc(StiffnessUpdatePolicy)
install_mtest_desc(Strain)
install_mtest_desc(StrainEpsilon)
install_mtest_desc(DeformationGradient)
install_mtest_desc(DeformationGradientEpsilon)
install_mtest_desc(Stress)
install_mtest_desc(StressEpsilon)
install_mtest_desc(CohesiveForce)
install_mtest_desc(CohesiveForceEpsilon)
install_mtest_desc(Test)
install_mtest_desc(Times)
install_mtest_desc(UseCastemAccelerationAlgorithm)

install_mtest_desc(AccelerationAlgorithm)
install_mtest_desc(AccelerationAlgorithmParameter)
install_mtest_desc(CompareToNumericalTangentOperator)
install_mtest_desc(NumericalTangentOperatorPerturbationValue)