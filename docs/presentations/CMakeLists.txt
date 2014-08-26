macro(latex_presentation doc)
  configure_file(${doc}.tex.in ${doc}.tex @ONLY)
  ADD_CUSTOM_COMMAND(
    OUTPUT    ${doc}.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/${doc}.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images -interaction=batchmode ${doc}.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images -interaction=batchmode ${doc}.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images -interaction=batchmode ${doc}.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 ${doc}.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      ${doc}.ps
    COMMENT   "${doc}.pdf")
  
  # Eventually trigger the whole process
  add_custom_target(${doc}-doc-pdf echo DEPENDS ${doc}.pdf)
  add_custom_target(${doc}-doc-pdf-install
    ${CMAKE_COMMAND} -E copy ${doc}.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/${doc}/${doc}.pdf)
  add_dependencies(${doc}-doc-pdf-install ${doc}-doc-pdf)
  
  add_dependencies(doc-pdf
    ${doc}-doc-pdf)
  
  add_dependencies(doc-pdf-install
    ${doc}-doc-pdf-install)
endmacro(latex_presentation doc)

latex_presentation(mfront-presentation)
latex_presentation(mfront-behaviours)
latex_presentation(mfront-finitestrain)
latex_presentation(mtest-presentation)
