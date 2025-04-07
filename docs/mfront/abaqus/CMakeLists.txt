if(HAVE_LATEX AND GENERATE_REFERENCE_DOC)

  configure_file(abaqus.tex.in
    abaqus.tex @ONLY)

  ADD_CUSTOM_COMMAND(
    OUTPUT    abaqus.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/abaqus.tex.in
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      abaqus.tex
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      abaqus.tex
   COMMAND   BSTINPUTS=${abs_top_srcdir}/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
   ARGS      -terse abaqus
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      abaqus.tex
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      abaqus.tex
    COMMENT   "abaqus.pdf")

  add_custom_target(abaqus-doc-pdf ALL DEPENDS abaqus.pdf)

  add_dependencies(doc-pdf
    abaqus-doc-pdf)
  if(TFEL_APPEND_VERSION)
    install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/abaqus.pdf
      DESTINATION share/doc/mfront-${TFEL_VERSION}/references
      COMPONENT docs)
    install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/abaqus.pdf
      DESTINATION share/doc/tfel-${TFEL_VERSION}/web/documents/mfront
      COMPONENT website)
  else(TFEL_APPEND_VERSION)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/abaqus.pdf
    DESTINATION share/doc/mfront/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/abaqus.pdf
    DESTINATION share/doc/tfel/web/documents/mfront
    COMPONENT website)
  endif(TFEL_APPEND_VERSION)

endif(HAVE_LATEX AND GENERATE_REFERENCE_DOC)

if(TFEL_APPEND_VERSION)
  install(FILES umat.cpp DESTINATION share/doc/mfront-${TFEL_VERSION}/abaqus
    COMPONENT mfront)
  install(FILES vumat-dp.cpp DESTINATION share/doc/mfront-${TFEL_VERSION}/abaqus
    COMPONENT mfront)
  install(FILES vumat-sp.cpp DESTINATION share/doc/mfront-${TFEL_VERSION}/abaqus
    COMPONENT mfront)
else(TFEL_APPEND_VERSION)
install(FILES umat.cpp DESTINATION share/doc/mfront/abaqus
    COMPONENT mfront)
install(FILES vumat-dp.cpp DESTINATION share/doc/mfront/abaqus
    COMPONENT mfront)
install(FILES vumat-sp.cpp DESTINATION share/doc/mfront/abaqus
    COMPONENT mfront)
endif(TFEL_APPEND_VERSION)
