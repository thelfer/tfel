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
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse ${doc}
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
    DESTINATION share/doc/mfront//mfront-user-meeting-2015/
    COMPONENT docs)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
    DESTINATION share/doc/tfel/web/documents//mfront-user-meeting-2015/
    COMPONENT website)
endmacro(latex_presentation doc)

macro(latex_presentation2 doc)
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
    DESTINATION share/doc/mfront/mfront-user-meeting-2015/
    COMPONENT docs)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
    DESTINATION share/doc/tfel/web/documents/mfront-user-meeting-2015/
    COMPONENT website)
endmacro(latex_presentation2 doc)

if(HAVE_LATEX)
  latex_presentation(mfront-cea)
  latex_presentation2(tfel-3.0-overview)
  latex_presentation(writing-portable-behaviour)
endif(HAVE_LATEX)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Contributions/VM-ClubU_MFront_6-2-2015.pdf
  DESTINATION share/doc/tfel/web/documents/mfront-user-meeting-2015/
  COMPONENT website)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Contributions/petry_MFRONT.pdf
  DESTINATION share/doc/tfel/web/documents/mfront-user-meeting-2015/
  COMPONENT website)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Contributions/2015-ClubU-MFront-REX-Aster.pdf
  DESTINATION share/doc/tfel/web/documents/mfront-user-meeting-2015/
  COMPONENT website)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Contributions/hamon_ClubU_MFront_6-2-2015.pdf
  DESTINATION share/doc/tfel/web/documents/mfront-user-meeting-2015/
  COMPONENT website)
