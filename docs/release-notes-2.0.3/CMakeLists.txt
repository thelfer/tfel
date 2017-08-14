if(HAVE_LATEX)
  configure_file(tfel-2.0.3.tex.in
    tfel-2.0.3.tex @ONLY)

  ADD_CUSTOM_COMMAND(
    OUTPUT    tfel-2.0.3.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/tfel-2.0.3.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-2.0.3.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-2.0.3.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse tfel-2.0.3
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-2.0.3.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-2.0.3.tex
    COMMENT   "tfel-2.0.3.pdf")

  add_custom_target(tfel-2.0.3-doc-pdf ALL DEPENDS tfel-2.0.3.pdf)
  add_dependencies(doc-pdf
    tfel-2.0.3-doc-pdf)

  if(TFEL_APPEND_VERSION)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.3.pdf
      DESTINATION share/doc/tfel-${TFEL_VERSION}/references
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.3.pdf
      DESTINATION share/doc/tfel-${TFEL_VERSION}/web/documents/tfel
      COMPONENT website)
  else(TFEL_APPEND_VERSION)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.3.pdf
      DESTINATION share/doc/tfel/references
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.3.pdf
      DESTINATION share/doc/tfel/web/documents/tfel
      COMPONENT website)
  endif(TFEL_APPEND_VERSION)

endif(HAVE_LATEX)
