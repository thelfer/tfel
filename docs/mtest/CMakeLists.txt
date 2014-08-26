macro(install_mtest_desc file)
  install(FILES ${file} DESTINATION share/doc/mtest)
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
                  ${CMAKE_COMMAND} -E copy mtest.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mtest/mtest.pdf)
add_dependencies(mtest-doc-pdf-install mtest-doc-pdf)

add_dependencies(doc-pdf
  mtest-doc-pdf)

add_dependencies(doc-pdf-install
  mtest-doc-pdf-install)

install_mtest_desc(Author.txt)
install_mtest_desc(Behaviour.txt)
install_mtest_desc(CastemAccelerationPeriod.txt)
install_mtest_desc(CastemAccelerationTrigger.txt)
install_mtest_desc(Date.txt)
install_mtest_desc(Description.txt)
install_mtest_desc(Evolution.txt)
install_mtest_desc(ExternalStateVariable.txt)
install_mtest_desc(ImposedStrain.txt)
install_mtest_desc(ImposedStress.txt)
install_mtest_desc(InternalStateVariable.txt)
install_mtest_desc(MaterialProperty.txt)
install_mtest_desc(MaximumNumberOfIterations.txt)
install_mtest_desc(MaximumNumberOfSubSteps.txt)
install_mtest_desc(ModellingHypothesis.txt)
install_mtest_desc(Parameter.txt)
install_mtest_desc(IntegerParameter.txt)
install_mtest_desc(UnsignedIntegerParameter.txt)
install_mtest_desc(OutputFile.txt)
install_mtest_desc(OutputFilePrecision.txt)
install_mtest_desc(PredictionPolicy.txt)
install_mtest_desc(Real.txt)
install_mtest_desc(RotationMatrix.txt)
install_mtest_desc(StiffnessMatrixType.txt)
install_mtest_desc(StiffnessUpdatePolicy.txt)
install_mtest_desc(Strain.txt)
install_mtest_desc(StrainEpsilon.txt)
install_mtest_desc(Stress.txt)
install_mtest_desc(StressEpsilon.txt)
install_mtest_desc(Test.txt)
install_mtest_desc(Times.txt)
install_mtest_desc(UseCastemAccelerationAlgorithm.txt)
