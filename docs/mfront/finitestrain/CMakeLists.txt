configure_file(finitestrain.tex.in
  finitestrain.tex @ONLY)

ADD_CUSTOM_COMMAND(
  OUTPUT    finitestrain.pdf
  DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/finitestrain.tex.in
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      finitestrain.tex
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      finitestrain.tex
  COMMAND   BSTINPUTS=${abs_top_srcdir}/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
  ARGS      -terse finitestrain
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      finitestrain.tex
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      finitestrain.tex
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${DVIPS_CONVERTER}
  ARGS      -R0 finitestrain.dvi
  COMMAND   ${PS2PDF_CONVERTER}
  ARGS      finitestrain.ps
  COMMENT   "finitestrain.pdf")

add_custom_target(finitestrain-doc-pdf ALL DEPENDS finitestrain.pdf)
add_dependencies(doc-pdf
  finitestrain-doc-pdf)

if(TFEL_APPEND_SUFFIX)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/finitestrain.pdf
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/finitestrain.pdf
    DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/mfront
    COMPONENT website)
else(TFEL_APPEND_SUFFIX)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/finitestrain.pdf
    DESTINATION share/doc/mfront/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/finitestrain.pdf
    DESTINATION share/doc/tfel/web/documents/mfront
    COMPONENT website)
endif(TFEL_APPEND_SUFFIX)
