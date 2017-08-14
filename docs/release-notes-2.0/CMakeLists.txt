if(HAVE_LATEX)
  configure_file(Champs.tex.in Champs.tex @ONLY)
  configure_file(tfel-2.0.tex.in
    tfel-2.0.tex @ONLY)

  ADD_CUSTOM_COMMAND(
    OUTPUT    tfel-2.0.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/Champs.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/tfel-2.0.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-2.0.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-2.0.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse tfel-2.0
    # COMMAND   ${MAKEINDEX_COMPILER}
    # ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-2.0.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-2.0.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 tfel-2.0.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      tfel-2.0.ps
    COMMENT   "tfel-2.0.pdf")

  add_custom_target(tfel-2.0-doc-pdf ALL DEPENDS tfel-2.0.pdf)
  add_dependencies(doc-pdf
    tfel-2.0-doc-pdf)
  if(TFEL_APPEND_VERSION)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.pdf
      DESTINATION share/doc/tfel-${TFEL_VERSION}/references
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.pdf
      DESTINATION share/doc/tfel-${TFEL_VERSION}/web/documents/tfel
      COMPONENT website)
  else(TFEL_APPEND_VERSION)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.pdf
      DESTINATION share/doc/tfel/references
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.pdf
      DESTINATION share/doc/tfel/web/documents/tfel
      COMPONENT website)
  endif(TFEL_APPEND_VERSION)

endif(HAVE_LATEX)
