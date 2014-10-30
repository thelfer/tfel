macro(pdflatex_tutorial doc)
  configure_file(${doc}.tex.in ${doc}.tex @ONLY)
  ADD_CUSTOM_COMMAND(
    OUTPUT    ${doc}.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/${doc}.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images -interaction=batchmode ${doc}.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse ${doc}

    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images -interaction=batchmode ${doc}.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images -interaction=batchmode ${doc}.tex
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
