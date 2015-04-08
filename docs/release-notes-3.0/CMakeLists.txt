if(HAVE_LATEX)
  configure_file(Champs.tex.in Champs.tex @ONLY)
  configure_file(tfel-3.0.tex.in
    tfel-3.0.tex @ONLY)

  ADD_CUSTOM_COMMAND(
    OUTPUT    tfel-3.0.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/Champs.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/tfel-3.0.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-3.0.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-3.0.tex
    # COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    # ARGS      -terse tfel-3.0
    # COMMAND   ${MAKEINDEX_COMPILER}
    # ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-3.0.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-3.0.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 tfel-3.0.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      tfel-3.0.ps
    COMMENT   "tfel-3.0.pdf")

  add_custom_target(tfel-3.0-doc-pdf ALL DEPENDS tfel-3.0.pdf)
  add_dependencies(doc-pdf
    tfel-3.0-doc-pdf)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-3.0.pdf
    DESTINATION share/doc/tfel/references
    COMPONENT docs)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-3.0.pdf
    DESTINATION share/doc/tfel/web/documents/tfel/tfel
    COMPONENT website)
endif(HAVE_LATEX)
