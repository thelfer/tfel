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
  add_custom_target(${doc}-doc-pdf echo DEPENDS ${doc}.pdf)
  add_dependencies(doc-pdf
    ${doc}-doc-pdf)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
    DESTINATION share/doc/mfront/${doc}
    COMPONENT docs OPTIONAL)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
    DESTINATION share/doc/tfel/web/documents/${doc}
    COMPONENT website OPTIONAL)
endmacro(latex_presentation doc)

macro(pdflatex_presentation doc)
  configure_file(${doc}.tex.in ${doc}.tex @ONLY)
  ADD_CUSTOM_COMMAND(
    OUTPUT    ${doc}.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/${doc}.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/pdflatex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images -interaction=batchmode ${doc}.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/pdflatex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images -interaction=batchmode ${doc}.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/pdflatex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images -interaction=batchmode ${doc}.tex
    COMMENT   "${doc}.pdf")
  add_custom_target(${doc}-doc-pdf echo DEPENDS ${doc}.pdf)
  add_dependencies(doc-pdf
    ${doc}-doc-pdf)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
    DESTINATION share/doc/mfront/${doc}
    COMPONENT docs OPTIONAL)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
    DESTINATION share/doc/tfel/web/documents/${doc}
    COMPONENT website OPTIONAL)
endmacro(pdflatex_presentation doc)

if(HAVE_LATEX)
  latex_presentation(mfront-presentation)
  latex_presentation(mfront-behaviours)
  latex_presentation(mfront-finitestrain)
  latex_presentation(mtest-presentation)
  pdflatex_presentation(mfront-formation)
  pdflatex_presentation(tp)
endif(HAVE_LATEX)

if(HAVE_PANDOC)
  add_subdirectory(mfront-user-meeting)
  add_subdirectory(portable-behaviour)
endif(HAVE_PANDOC)