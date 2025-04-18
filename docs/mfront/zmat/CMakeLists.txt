configure_file(zmat.tex.in
  zmat.tex @ONLY)

ADD_CUSTOM_COMMAND(
  OUTPUT    zmat.pdf
  DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/zmat.tex.in
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      zmat.tex
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      zmat.tex
  COMMAND   BSTINPUTS=${abs_top_srcdir}/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
  ARGS      -terse zmat
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      zmat.tex
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      zmat.tex
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${DVIPS_CONVERTER}
  ARGS      -R0 zmat.dvi
  COMMAND   ${PS2PDF_CONVERTER}
  ARGS      zmat.ps
  COMMENT   "zmat.pdf")

add_custom_target(zmat-doc-pdf ALL DEPENDS zmat.pdf)

add_dependencies(doc-pdf
  zmat-doc-pdf)

if(TFEL_APPEND_SUFFIX)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/zmat.pdf
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/zmat.pdf
    DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/mfront
    COMPONENT website)
else(TFEL_APPEND_SUFFIX)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/zmat.pdf
    DESTINATION share/doc/mfront/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/zmat.pdf
    DESTINATION share/doc/tfel/web/documents/mfront
    COMPONENT website)
endif(TFEL_APPEND_SUFFIX)
