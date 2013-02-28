configure_file(Champs.tex.in Champs.tex @ONLY)
configure_file(annexe-orthotropie.tex.in
               annexe-orthotropie.tex @ONLY)
configure_file(mfront.tex.in
               mfront.tex @ONLY)
configure_file(behaviours.tex.in
               behaviours.tex @ONLY)
configure_file(glossary.tex.in
               glossary.tex @ONLY)
configure_file(InternalNames.tex.in
               InternalNames.tex @ONLY)

ADD_CUSTOM_COMMAND(
    OUTPUT    mfront.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/Champs.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/mfront.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/annexe-orthotropie.tex.in
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mfront.tex
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mfront.tex
    COMMAND   ${BIBTEX_COMPILER}
    ARGS      -terse mfront
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist env.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist headers.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist tfel.idx
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mfront.tex
    COMMAND   ${DVIPS_CONVERTER}
    ARGS      mfront.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      mfront.ps
    COMMENT   "mfront.pdf")

ADD_CUSTOM_COMMAND(
    OUTPUT    behaviours.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/Champs.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/behaviours.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/annexe-orthotropie.tex.in
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode behaviours.tex
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode behaviours.tex
    COMMAND   ${BIBTEX_COMPILER}
    ARGS      -terse behaviours
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist env.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist headers.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist tfel.idx
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode behaviours.tex
    COMMAND   ${DVIPS_CONVERTER}
    ARGS      behaviours.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      behaviours.ps
    COMMENT   "behaviours.pdf")
  
# Eventually trigger the whole process
add_custom_target(mfront-doc echo DEPENDS mfront.pdf)
add_custom_target(mfront-doc-install
                  ${CMAKE_COMMAND} -E copy mfront.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/mfront.pdf)
add_dependencies(mfront-doc-install mfront-doc)

add_custom_target(behaviours-doc echo DEPENDS behaviours.pdf)
add_custom_target(behaviours-doc-install
                  ${CMAKE_COMMAND} -E copy behaviours.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/behaviours.pdf)
add_dependencies(behaviours-doc-install behaviours-doc)

add_dependencies(doc-pdf
  mfront-doc
  behaviours-doc)
add_dependencies(doc-install
  mfront-doc-install
  behaviours-doc-install)
