configure_file(mfront-presentation.tex.in
               mfront-presentation.tex @ONLY)

macro(latex_presentation doc)
  ADD_CUSTOM_COMMAND(
    OUTPUT    ${doc}.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/${doc}.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode ${doc}.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode ${doc}.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode ${doc}.tex
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
#macro(latex_presentation mfront-presentation)