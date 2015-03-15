macro(latex_presentation doc)
  configure_file(${doc}.tex.in ${doc}.tex @ONLY)
  ADD_CUSTOM_COMMAND(
    OUTPUT    ${doc}.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/${doc}.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/beamer/themes
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      -interaction=batchmode
    ARGS      ${doc}.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/beamer/themes
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      -interaction=batchmode
    ARGS      ${doc}.tex
# COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
# ARGS      -terse ${doc}
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/beamer/themes
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      -interaction=batchmode
    ARGS      ${doc}.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 ${doc}.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      ${doc}.ps
    COMMENT   "${doc}.pdf")
  add_custom_target(${doc}-doc-pdf ALL DEPENDS ${doc}.pdf)
  add_dependencies(doc-pdf
    ${doc}-doc-pdf)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
    DESTINATION share/doc/mfront/CodeAsterUserMeeting2015/
    COMPONENT docs)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
    DESTINATION share/doc/tfel/web/documents/CodeAsterUserMeeting2015/
    COMPONENT website)
endmacro(latex_presentation doc)

if(HAVE_LATEX)
  latex_presentation(mfront-aster-2015)
endif(HAVE_LATEX)
