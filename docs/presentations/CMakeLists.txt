configure_file(mfront-presentation.tex.in
               mfront-presentation.tex @ONLY)

ADD_CUSTOM_COMMAND(
    OUTPUT    mfront-presentation.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/mfront-presentation.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mfront-presentation.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mfront-presentation.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mfront-presentation.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 mfront-presentation.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      mfront-presentation.ps
    COMMENT   "mfront-presentation.pdf")
  
# Eventually trigger the whole process
add_custom_target(mfront-presentation-doc-pdf echo DEPENDS mfront-presentation.pdf)
add_custom_target(mfront-presentation-doc-pdf-install
                  ${CMAKE_COMMAND} -E copy mfront-presentation.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront-presentation/mfront-presentation.pdf)
add_dependencies(mfront-presentation-doc-pdf-install mfront-presentation-doc-pdf)

add_dependencies(doc-pdf
  mfront-presentation-doc-pdf)

add_dependencies(doc-pdf-install
  mfront-presentation-doc-pdf-install)
