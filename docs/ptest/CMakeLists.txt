if(HAVE_LATEX AND GENERATE_REFERENCE_DOC)

  configure_file(ptest.tex.in
    ptest.tex @ONLY)
  
  ADD_CUSTOM_COMMAND(
    OUTPUT    ptest.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/ptest.tex.in
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS       ptest.tex
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS       ptest.tex
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS       ptest.tex
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS       ptest.tex
    COMMENT   "ptest.pdf")
  
  add_custom_target(ptest-doc-pdf ALL DEPENDS ptest.pdf)
  add_dependencies(doc-pdf
    ptest-doc-pdf)
  if(TFEL_APPEND_VERSION)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ptest.pdf
      DESTINATION share/doc/mtest-${TFEL_VERSION}/ptest
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ptest.pdf
      DESTINATION share/doc/tfel-${TFEL_VERSION}/web/documents/ptest
      COMPONENT website)
  else(TFEL_APPEND_VERSION)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ptest.pdf
      DESTINATION share/doc/mtest/ptest
    COMPONENT docs)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ptest.pdf
    DESTINATION share/doc/tfel/web/documents/ptest
    COMPONENT website) 
  endif(TFEL_APPEND_VERSION)
endif(HAVE_LATEX AND GENERATE_REFERENCE_DOC)
