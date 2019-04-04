if(HAVE_LATEX AND GENERATE_REFERENCE_DOC)

  configure_file(abaqus.tex.in
    abaqus.tex @ONLY)

  ADD_CUSTOM_COMMAND(
    OUTPUT    abaqus.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/abaqus.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      abaqus.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      abaqus.tex
   COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
   ARGS      -terse abaqus
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      abaqus.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${PDFLATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      abaqus.tex
    COMMENT   "abaqus.pdf")

  add_custom_target(abaqus-doc-pdf ALL DEPENDS abaqus.pdf)

  add_dependencies(doc-pdf
    abaqus-doc-pdf)
  if(TFEL_APPEND_SUFFIX)
    install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/abaqus.pdf
      DESTINATION share/doc/mfront-${TFEL_SUFFIX}/references
      COMPONENT docs)
    install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/abaqus.pdf
      DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/mfront
      COMPONENT website)
  else(TFEL_APPEND_SUFFIX)
    install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/abaqus.pdf
      DESTINATION share/doc/mfront/references
      COMPONENT docs)
    install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/abaqus.pdf
      DESTINATION share/doc/tfel/web/documents/mfront
      COMPONENT website)
  endif(TFEL_APPEND_SUFFIX)

endif(HAVE_LATEX AND GENERATE_REFERENCE_DOC)

if(TFEL_APPEND_SUFFIX)
  install(FILES umat.cpp DESTINATION share/doc/mfront-${TFEL_SUFFIX}/abaqus
    COMPONENT mfront)
  install(FILES vumat-dp.cpp DESTINATION share/doc/mfront-${TFEL_SUFFIX}/abaqus
    COMPONENT mfront)
  install(FILES vumat-sp.cpp DESTINATION share/doc/mfront-${TFEL_SUFFIX}/abaqus
    COMPONENT mfront)
else(TFEL_APPEND_SUFFIX)
  install(FILES umat.cpp DESTINATION share/doc/mfront/abaqus
    COMPONENT mfront)
  install(FILES vumat-dp.cpp DESTINATION share/doc/mfront/abaqus
    COMPONENT mfront)
  install(FILES vumat-sp.cpp DESTINATION share/doc/mfront/abaqus
    COMPONENT mfront)
endif(TFEL_APPEND_SUFFIX)