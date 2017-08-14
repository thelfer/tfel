if(HAVE_LATEX)
  macro(pdflatex_tutorial doc)
    configure_file(${doc}.tex.in ${doc}.tex @ONLY)
    ADD_CUSTOM_COMMAND(
      OUTPUT    ${doc}.pdf
      DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/${doc}.tex.in
      COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
      ARGS      --include-directory=@abs_top_srcdir@/docs/tutorial
      ARGS      -interaction=batchmode ${doc}.tex
      COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
      ARGS      -terse ${doc}
      COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
      ARGS      --include-directory=@abs_top_srcdir@/docs/tutorial
      ARGS      -interaction=batchmode ${doc}.tex
      ${doc}.tex
      COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
      ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
      ARGS      --include-directory=@abs_top_srcdir@/docs/tutorial
      ARGS      -interaction=batchmode ${doc}.tex
      ${doc}.tex
      COMMENT   "${doc}.pdf")
    add_custom_target(${doc}-doc-pdf ALL DEPENDS ${doc}.pdf)
    add_dependencies(doc-pdf
      ${doc}-doc-pdf)
    if(TFEL_APPEND_VERSION)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
	DESTINATION share/doc/mfront-${TFEL_VERSION}/${doc}
	COMPONENT docs)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
	DESTINATION share/doc/tfel-${TFEL_VERSION}/web/documents/${doc}
	COMPONENT website)
    else(TFEL_APPEND_VERSION)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
      DESTINATION share/doc/mfront/${doc}
      COMPONENT docs)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${doc}.pdf
      DESTINATION share/doc/tfel/web/documents/${doc}
      COMPONENT website)
    endif(TFEL_APPEND_VERSION)
  endmacro(pdflatex_tutorial doc)

  configure_file(Bibliographie.tex.in
    Bibliographie.tex @ONLY)
  configure_file(introduction.tex.in
    introduction.tex @ONLY)
  configure_file(tutoriel.tex.in
    tutoriel.tex @ONLY)	
  configure_file(Chaboche.tex.in
    Chaboche.tex @ONLY)	
  configure_file(rappels.tex.in
    rappels.tex @ONLY)	
  configure_file(Conclusions.tex.in
    Conclusions.tex @ONLY)
  configure_file(resume.tex.in
    resume.tex @ONLY)	
  configure_file(ViscoCompressible.tex.in
    ViscoCompressible.tex @ONLY)

  pdflatex_tutorial(tutoriel)
endif(HAVE_LATEX)
