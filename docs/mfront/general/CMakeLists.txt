configure_file(Champs.tex.in Champs.tex @ONLY)
configure_file(mfront.tex.in
  mfront.tex @ONLY)

ADD_CUSTOM_COMMAND(
  OUTPUT    mfront.pdf
  DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/Champs.tex.in
  DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/mfront.tex.in
  DEPENDS    ${CMAKE_SOURCE_DIR}/docs/mfront/glossary.tex.in
  DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/Champs.tex
  DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/mfront.tex
  DEPENDS    ${CMAKE_BINARY_DIR}/docs/mfront/glossary.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      mfront.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      mfront.tex
  COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
  ARGS      -terse mfront
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist env.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist mkeys.idx
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      mfront.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      mfront.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
  ARGS      -R0 mfront.dvi
  COMMAND   ${PS2PDF_CONVERTER}
  ARGS      mfront.ps
  COMMENT   "mfront.pdf")

add_custom_target(mfront-doc-pdf ALL DEPENDS mfront.pdf)

add_dependencies(doc-pdf
  mfront-doc-pdf)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/mfront.pdf
  DESTINATION share/doc/mfront/references
  COMPONENT docs)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/mfront.pdf
  DESTINATION share/doc/tfel/web/documents/mfront
  COMPONENT website)

