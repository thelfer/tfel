configure_file(annexe-orthotropie.tex.in
  annexe-orthotropie.tex @ONLY)
configure_file(behaviours.tex.in
  behaviours.tex @ONLY)
configure_file(InternalNames.tex.in
  InternalNames.tex @ONLY)

ADD_CUSTOM_COMMAND(
  OUTPUT    behaviours.pdf
  DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/behaviours.tex.in
  DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/annexe-orthotropie.tex.in
  DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/InternalNames.tex.in
  DEPENDS    ${PROJECT_SOURCE_DIR}/docs/mfront/glossary.tex.in
  DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.tex
  DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/annexe-orthotropie.tex
  DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/InternalNames.tex
  DEPENDS    ${PROJECT_BINARY_DIR}/docs/mfront/glossary.tex
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      behaviours.tex
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      behaviours.tex
  COMMAND   BSTINPUTS=${abs_top_srcdir}/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
  ARGS      -terse behaviours
  # COMMAND   ${MAKEINDEX_COMPILER}
  # ARGS      -s ${PROJECT_SOURCE_DIR}/docs/mfront/style.ist benv.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${PROJECT_SOURCE_DIR}/docs/mfront/style.ist bheaders.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${PROJECT_SOURCE_DIR}/docs/mfront/style.ist bmkeys.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${PROJECT_SOURCE_DIR}/docs/mfront/style.ist btfel.idx
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      behaviours.tex
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      behaviours.tex
  COMMAND   TEXMFHOME=${abs_top_srcdir}/docs/tex/texmf        ${DVIPS_CONVERTER}
  ARGS      -R0 behaviours.dvi
  COMMAND   ${PS2PDF_CONVERTER}
  ARGS      behaviours.ps
  COMMENT   "behaviours.pdf")

add_custom_target(behaviours-doc-pdf ALL DEPENDS behaviours.pdf)

add_dependencies(doc-pdf
  behaviours-doc-pdf)

if(TFEL_APPEND_SUFFIX)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.pdf
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/behaviours/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.pdf
    DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/mfront
    COMPONENT website)
else(TFEL_APPEND_SUFFIX)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.pdf
    DESTINATION share/doc/mfront/behaviours/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.pdf
    DESTINATION share/doc/tfel/web/documents/mfront
    COMPONENT website)
endif(TFEL_APPEND_SUFFIX)
