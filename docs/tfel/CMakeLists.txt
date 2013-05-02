configure_file(Champs.tex.in Champs.tex @ONLY)
configure_file(reference-guide.tex.in
               reference-guide.tex @ONLY)

ADD_CUSTOM_COMMAND(
    OUTPUT    reference-guide.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/Champs.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/reference-guide.tex.in
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode reference-guide.tex
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode reference-guide.tex
    # COMMAND   ${BIBTEX_COMPILER}
    # ARGS      -terse reference-guide
    # COMMAND   ${MAKEINDEX_COMPILER}
    # ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode reference-guide.tex
    COMMAND   ${DVIPS_CONVERTER}
    ARGS      reference-guide.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      reference-guide.ps
    COMMENT   "reference-guide.pdf")
  
# Eventually trigger the whole process
add_custom_target(tfel-doc-pdf echo DEPENDS reference-guide.pdf)
add_custom_target(tfel-doc-pdf-install
                  ${CMAKE_COMMAND} -E copy reference-guide.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/tfel/reference-guide.pdf)
add_dependencies(tfel-doc-pdf-install tfel-doc-pdf)

add_dependencies(doc-pdf
  tfel-doc-pdf)

add_dependencies(doc-install
  tfel-doc-pdf-install)
