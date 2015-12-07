if(HAVE_LATEX AND GENERATE_REFERENCE_DOC)

  configure_file(ptest.tex.in
    ptest.tex @ONLY)
  
  ADD_CUSTOM_COMMAND(
    OUTPUT    ptest.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/ptest.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS       ptest.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS       ptest.tex
# COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
# ARGS      -terse ptest
# COMMAND   ${MAKEINDEX_COMPILER}
# ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS       ptest.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS       ptest.tex
    COMMENT   "ptest.pdf")
  
  add_custom_target(ptest-doc-pdf ALL DEPENDS ptest.pdf)
  add_dependencies(doc-pdf
    ptest-doc-pdf)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ptest.pdf
    DESTINATION share/doc/ptest
    COMPONENT docs)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ptest.pdf
    DESTINATION share/doc/tfel/web/documents/ptest
    COMPONENT website)
  
endif(HAVE_LATEX AND GENERATE_REFERENCE_DOC)
