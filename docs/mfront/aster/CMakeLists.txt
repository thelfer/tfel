configure_file(aster.tex.in
  aster.tex @ONLY)

ADD_CUSTOM_COMMAND(
  OUTPUT    aster.pdf
  DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/aster.tex.in
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      aster.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      aster.tex
  #    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
  #    ARGS      -terse aster
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist aenv.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist aheaders.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist amkeys.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist atfel.idx
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      aster.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      aster.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
  ARGS      -R0 aster.dvi
  COMMAND   ${PS2PDF_CONVERTER}
  ARGS      aster.ps
  COMMENT   "aster.pdf")

add_custom_target(aster-doc-pdf ALL DEPENDS aster.pdf)

add_dependencies(doc-pdf
  aster-doc-pdf)

if(TFEL_APPEND_VERSION)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/aster.pdf
    DESTINATION share/doc/mfront-${TFEL_VERSION}/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/aster.pdf
    DESTINATION share/doc/tfel-${TFEL_VERSION}/web/documents/mfront
    COMPONENT website)
else(TFEL_APPEND_VERSION)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/aster.pdf
    DESTINATION share/doc/mfront/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/aster.pdf
    DESTINATION share/doc/tfel/web/documents/mfront
    COMPONENT website)
endif(TFEL_APPEND_VERSION)