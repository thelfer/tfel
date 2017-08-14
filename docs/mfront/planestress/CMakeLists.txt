configure_file(planestress.tex.in
  planestress.tex @ONLY)

ADD_CUSTOM_COMMAND(
  OUTPUT    planestress.pdf
  DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/planestress.tex.in
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      planestress.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      planestress.tex
  COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
  ARGS      -terse planestress
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      planestress.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      planestress.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
  ARGS      -R0 planestress.dvi
  COMMAND   ${PS2PDF_CONVERTER}
  ARGS      planestress.ps
  COMMENT   "planestress.pdf")

add_custom_target(planestress-doc-pdf ALL DEPENDS planestress.pdf)
add_dependencies(doc-pdf planestress-doc-pdf)

if(TFEL_APPEND_VERSION)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/planestress.pdf
    DESTINATION share/doc/mfront-${TFEL_VERSION}/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/planestress.pdf
    DESTINATION share/doc/tfel-${TFEL_VERSION}/web/documents/mfront
    COMPONENT website)
else(TFEL_APPEND_VERSION)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/planestress.pdf
    DESTINATION share/doc/mfront/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/planestress.pdf
    DESTINATION share/doc/tfel/web/documents/mfront
    COMPONENT website)
endif(TFEL_APPEND_VERSION)