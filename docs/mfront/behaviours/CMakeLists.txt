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
  DEPENDS    ${CMAKE_SOURCE_DIR}/docs/mfront/glossary.tex.in
  DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.tex
  DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/annexe-orthotropie.tex
  DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/InternalNames.tex
  DEPENDS    ${CMAKE_BINARY_DIR}/docs/mfront/glossary.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      behaviours.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      behaviours.tex
  COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
  ARGS      -terse behaviours
  # COMMAND   ${MAKEINDEX_COMPILER}
  # ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist benv.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist bheaders.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist bmkeys.idx
  COMMAND   ${MAKEINDEX_COMPILER}
  ARGS      -s ${CMAKE_SOURCE_DIR}/docs/mfront/style.ist btfel.idx
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      behaviours.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
  ARGS      -interaction=batchmode
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
  ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
  ARGS      behaviours.tex
  COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
  ARGS      -R0 behaviours.dvi
  COMMAND   ${PS2PDF_CONVERTER}
  ARGS      behaviours.ps
  COMMENT   "behaviours.pdf")

add_custom_target(behaviours-doc-pdf ALL DEPENDS behaviours.pdf)

add_dependencies(doc-pdf
  behaviours-doc-pdf)

if(TFEL_APPEND_VERSION)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.pdf
    DESTINATION share/doc/mfront-${TFEL_VERSION}/behaviours/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.pdf
    DESTINATION share/doc/tfel-${TFEL_VERSION}/web/documents/mfront
    COMPONENT website)
else(TFEL_APPEND_VERSION)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.pdf
    DESTINATION share/doc/mfront/behaviours/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.pdf
    DESTINATION share/doc/tfel/web/documents/mfront
    COMPONENT website)
endif(TFEL_APPEND_VERSION)