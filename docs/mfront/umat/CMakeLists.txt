configure_file(umat.tex.in
  umat.tex @ONLY)

ADD_CUSTOM_COMMAND(
  OUTPUT    umat.pdf
  DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/umat.tex.in
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      umat.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      umat.tex
  COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
  ARGS      -terse umat
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
  ARGS      umat.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      umat.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
  ARGS      -R0 umat.dvi
  COMMAND   ${PS2PDF_CONVERTER}
  ARGS      umat.ps
  COMMENT   "umat.pdf")

add_custom_target(umat-doc-pdf ALL DEPENDS umat.pdf)
add_dependencies(doc-pdf
  umat-doc-pdf)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/umat.pdf
  DESTINATION share/doc/mfront/references
  COMPONENT docs)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/umat.pdf
  DESTINATION share/doc/tfel/web/documents/mfront
  COMPONENT website)
