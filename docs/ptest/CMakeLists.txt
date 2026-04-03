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
# COMMAND   BSTINPUTS=${abs_top_srcdir}/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
# ARGS      -terse ptest
# COMMAND   ${MAKEINDEX_COMPILER}
# ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
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
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ptest.pdf
      DESTINATION share/doc/mtest-${TFEL_SUFFIX}/ptest
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ptest.pdf
      DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/ptest
      COMPONENT website)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ptest.pdf
      DESTINATION share/doc/mtest/ptest
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ptest.pdf
      DESTINATION share/doc/tfel/web/documents/ptest
      COMPONENT website)
  endif(TFEL_APPEND_SUFFIX)
endif(HAVE_LATEX AND GENERATE_REFERENCE_DOC)
