if(HAVE_LATEX)
  configure_file(tfel-2.0.4.tex.in
    tfel-2.0.4.tex @ONLY)

  ADD_CUSTOM_COMMAND(
    OUTPUT    tfel-2.0.4.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/tfel-2.0.4.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-2.0.4.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-2.0.4.tex
    # COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    # ARGS      -terse tfel-2.0.4
    # COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    # ARGS      -interaction=batchmode tfel-2.0.4.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode tfel-2.0.4.tex
    COMMENT   "tfel-2.0.4.pdf")

  add_custom_target(tfel-2.0.4-doc-pdf ALL DEPENDS tfel-2.0.4.pdf)
  add_dependencies(doc-pdf
    tfel-2.0.4-doc-pdf)
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.4.pdf
      DESTINATION share/doc/tfel-${TFEL_SUFFIX}/references
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.4.pdf
      DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/tfel
      COMPONENT website)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.4.pdf
      DESTINATION share/doc/tfel/references
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tfel-2.0.4.pdf
      DESTINATION share/doc/tfel/web/documents/tfel
      COMPONENT website)
  endif(TFEL_APPEND_SUFFIX)
endif(HAVE_LATEX)
