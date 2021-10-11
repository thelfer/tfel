add_subdirectory(mtest)
add_subdirectory(ptest)

macro(install_mtest_desc file)
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${file}.md
      DESTINATION share/doc/mtest-${TFEL_SUFFIX}
      COMPONENT mtest)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}.md DESTINATION share/doc/mtest
      COMPONENT mtest)
  endif(TFEL_APPEND_SUFFIX)
endmacro(install_mtest_desc file)

install_mtest_desc(Author)
install_mtest_desc(Behaviour)
install_mtest_desc(CastemAccelerationPeriod)
install_mtest_desc(CastemAccelerationTrigger)
install_mtest_desc(Date)
install_mtest_desc(DynamicTimeStepScaling)
install_mtest_desc(Description)
install_mtest_desc(Evolution)
install_mtest_desc(ExternalStateVariable)
install_mtest_desc(InternalStateVariable)
install_mtest_desc(HandleThermalExpansion)
install_mtest_desc(MaterialProperty)
install_mtest_desc(MaximumNumberOfIterations)
install_mtest_desc(MaximumNumberOfSubSteps)
install_mtest_desc(Model)
install_mtest_desc(OutputFrequency)
install_mtest_desc(Parameter)
install_mtest_desc(Import)
install_mtest_desc(IntegerParameter)
install_mtest_desc(UnsignedIntegerParameter)
install_mtest_desc(Message)
install_mtest_desc(MinimalTimeStep)
install_mtest_desc(MinimalTimeStepScalingFactor)
install_mtest_desc(MaximalTimeStep)
install_mtest_desc(MaximalTimeStepScalingFactor)
install_mtest_desc(OutputFile)
install_mtest_desc(OutputFilePrecision)
install_mtest_desc(Print)
install_mtest_desc(PredictionPolicy)
install_mtest_desc(Real)
install_mtest_desc(ResidualFile)
install_mtest_desc(ResidualFilePrecision)
install_mtest_desc(StiffnessMatrixType)
install_mtest_desc(StiffnessUpdatePolicy)
install_mtest_desc(Times)
install_mtest_desc(UseCastemAccelerationAlgorithm)
install_mtest_desc(OutOfBoundsPolicy)
install_mtest_desc(AccelerationAlgorithm)
install_mtest_desc(AccelerationAlgorithmParameter)
install_mtest_desc(CompareToNumericalTangentOperator)
install_mtest_desc(NumericalTangentOperatorPerturbationValue)

if(HAVE_LATEX AND GENERATE_REFERENCE_DOC)

  configure_file(Champs.tex.in Champs.tex @ONLY)
  configure_file(mtest.tex.in
    mtest.tex @ONLY)
  
  ADD_CUSTOM_COMMAND(
    OUTPUT    mtest.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/Champs.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/mtest.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS       mtest.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS       mtest.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse mtest
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS       mtest.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS       mtest.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 mtest.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      mtest.ps
    COMMENT   "mtest.pdf")
  
  add_custom_target(mtest-doc-pdf ALL DEPENDS mtest.pdf)
  add_dependencies(doc-pdf
    mtest-doc-pdf)

  if(TFEL_APPEND_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mtest.pdf
      DESTINATION share/doc/mtest-${TFEL_SUFFIX}
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mtest.pdf
      DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/mtest
      COMPONENT website)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mtest.pdf
      DESTINATION share/doc/mtest
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mtest.pdf
      DESTINATION share/doc/tfel/web/documents/mtest
      COMPONENT website)
  endif(TFEL_APPEND_SUFFIX)
endif(HAVE_LATEX AND GENERATE_REFERENCE_DOC)
