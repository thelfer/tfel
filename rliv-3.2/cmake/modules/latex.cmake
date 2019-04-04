find_package(LATEX)
if(LATEX_COMPILER)
  if((NOT PS2PDF_CONVERTER) AND WIN32)
    # fixes cmake bug #0015198
    find_program(PS2PDF_CONVERTER
      NAMES ps2pdf14.bat ps2pdf14 ps2pdf
      PATHS ${GHOSTSCRIPT_LIBRARY_PATH}
      ${MIKTEX_BINARY_PATH})
  endif((NOT PS2PDF_CONVERTER) AND WIN32)
  add_custom_target(doc-pdf)
  add_dependencies(doc doc-pdf)
  message(STATUS "latex compiler found : ${LATEX_COMPILER}")
  set(HAVE_LATEX ON)
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
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
	DESTINATION share/doc/mfront-${TFEL_SUFFIX}/${doc}
	COMPONENT docs)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
	DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/${doc}
	COMPONENT website)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
	DESTINATION share/doc/mfront/${doc}
	COMPONENT docs)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
	DESTINATION share/doc/tfel/web/documents/${doc}
	COMPONENT website)
    endif(TFEL_APPEND_SUFFIX)
  endmacro(latex_presentation doc)

  macro(pdflatex_presentation doc)
    configure_file(${doc}.tex.in ${doc}.tex @ONLY)
    ADD_CUSTOM_COMMAND(
      OUTPUT    ${doc}.pdf
      DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/${doc}.tex.in
      COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
      ARGS      -interaction=batchmode
      ARGS      ${doc}.tex
      COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
      ARGS      -interaction=batchmode
      ARGS      ${doc}.tex
      COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
      ARGS      -interaction=batchmode
      ARGS      ${doc}.tex
      COMMENT   "${doc}.pdf")
    add_custom_target(${doc}-doc-pdf ALL DEPENDS ${doc}.pdf)
    add_dependencies(doc-pdf
      ${doc}-doc-pdf)
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
	DESTINATION share/doc/mfront-${TFEL_SUFFIX}/${doc}
	COMPONENT docs)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
	DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/${doc}
	COMPONENT website)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
	DESTINATION share/doc/mfront/${doc}
	COMPONENT docs)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
	DESTINATION share/doc/tfel/web/documents/${doc}
	COMPONENT website)
    endif(TFEL_APPEND_SUFFIX)
  endmacro(pdflatex_presentation doc)
endif(LATEX_COMPILER)
