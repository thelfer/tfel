if(HAVE_LATEX)
  configure_file(Champs.tex.in Champs.tex @ONLY)
  configure_file(reference-guide.tex.in
    reference-guide.tex @ONLY)

  ADD_CUSTOM_COMMAND(
    OUTPUT    reference-guide.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/Champs.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/reference-guide.tex.in
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode reference-guide.tex
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode reference-guide.tex
    COMMAND   BSTINPUTS=${abs_top_srcdir}/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse reference-guide
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist tclass.idx
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode reference-guide.tex
    COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 reference-guide.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      reference-guide.ps
    COMMENT   "reference-guide.pdf")
  
  # Eventually trigger the whole process
  add_custom_target(tfel-doc-pdf ALL DEPENDS reference-guide.pdf)
  add_dependencies(doc-pdf
    tfel-doc-pdf)
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/reference-guide.pdf
      DESTINATION share/doc/tfel-${TFEL_SUFFIX}
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/reference-guide.pdf
      DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/tfel
      COMPONENT website)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/reference-guide.pdf
      DESTINATION share/doc/tfel
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/reference-guide.pdf
      DESTINATION share/doc/tfel/web/documents/tfel
      COMPONENT website)
  endif(TFEL_APPEND_SUFFIX)
endif(HAVE_LATEX)
