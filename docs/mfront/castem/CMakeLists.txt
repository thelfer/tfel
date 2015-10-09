configure_file(castem.tex.in
  castem.tex @ONLY)

ADD_CUSTOM_COMMAND(
  OUTPUT    castem.pdf
  DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/castem.tex.in
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      castem.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      castem.tex
  COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
  ARGS      -terse castem
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist uenv.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist uheaders.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist umkeys.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist utfel.idx
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      castem.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      castem.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
  ARGS      -R0 castem.dvi
  COMMAND   ${PS2PDF_CONVERTER}
  ARGS      castem.ps
  COMMENT   "castem.pdf")

add_custom_target(castem-doc-pdf ALL DEPENDS castem.pdf)
add_dependencies(doc-pdf
  castem-doc-pdf)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/castem.pdf
  DESTINATION share/doc/mfront/references
  COMPONENT docs)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/castem.pdf
  DESTINATION share/doc/tfel/web/documents/mfront
  COMPONENT website)
