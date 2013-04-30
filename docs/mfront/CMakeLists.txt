macro(install_mfront_desc file)
  install(FILES ${file} DESTINATION share/doc/mfront)
endmacro(install_mfront_desc file)

macro(install_mfront_desc2 parser file)
  install(FILES ${parser}/${file} DESTINATION share/doc/mfront/${parser})
endmacro(install_mfront_desc2 parser file)

configure_file(Champs.tex.in Champs.tex @ONLY)
configure_file(annexe-orthotropie.tex.in
               annexe-orthotropie.tex @ONLY)
configure_file(mfront.tex.in
               mfront.tex @ONLY)
configure_file(behaviours.tex.in
               behaviours.tex @ONLY)
configure_file(umat.tex.in
               umat.tex @ONLY)
configure_file(aster.tex.in
               aster.tex @ONLY)
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
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mfront.tex
    COMMAND   ${DVIPS_CONVERTER}
    ARGS      mfront.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      mfront.ps
    COMMENT   "mfront.pdf")

ADD_CUSTOM_COMMAND(
    OUTPUT    behaviours.pdf
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

ADD_CUSTOM_COMMAND(
    OUTPUT    umat.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/umat.tex.in
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode umat.tex
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode umat.tex
    COMMAND   ${BIBTEX_COMPILER}
    ARGS      -terse umat
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist env.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist headers.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist tfel.idx
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode umat.tex
    COMMAND   ${DVIPS_CONVERTER}
    ARGS      umat.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      umat.ps
    COMMENT   "umat.pdf")

ADD_CUSTOM_COMMAND(
    OUTPUT    aster.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/aster.tex.in
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode aster.tex
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode aster.tex
#    COMMAND   ${BIBTEX_COMPILER}
#    ARGS      -terse aster
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist env.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist headers.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist tfel.idx
    COMMAND   ${LATEX_COMPILER}
    ARGS      -interaction=batchmode aster.tex
    COMMAND   ${DVIPS_CONVERTER}
    ARGS      aster.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      aster.ps
    COMMENT   "aster.pdf")
  
# Eventually trigger the whole process
add_custom_target(mfront-doc-pdf echo DEPENDS mfront.pdf)
add_custom_target(mfront-doc-pdf-install
                  ${CMAKE_COMMAND} -E copy mfront.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/mfront.pdf)
add_dependencies(mfront-doc-pdf-install mfront-doc-pdf)

add_custom_target(behaviours-doc-pdf echo DEPENDS behaviours.pdf)
add_custom_target(behaviours-doc-pdf-install
                  ${CMAKE_COMMAND} -E copy behaviours.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/behaviours.pdf)
add_dependencies(behaviours-doc-pdf-install behaviours-doc-pdf)

add_custom_target(umat-doc-pdf echo DEPENDS umat.pdf)
add_custom_target(umat-doc-pdf-install
                  ${CMAKE_COMMAND} -E copy umat.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/umat.pdf)
add_dependencies(umat-doc-pdf-install umat-doc-pdf)

add_custom_target(aster-doc-pdf echo DEPENDS aster.pdf)
add_custom_target(aster-doc-pdf-install
                  ${CMAKE_COMMAND} -E copy aster.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/aster.pdf)
add_dependencies(aster-doc-pdf-install aster-doc-pdf)

add_dependencies(doc-pdf
  mfront-doc-pdf
  behaviours-doc-pdf
  umat-doc-pdf
  aster-doc-pdf)

add_dependencies(doc-install
  mfront-doc-pdf-install
  behaviours-doc-pdf-install
  umat-doc-pdf-install
  aster-doc-pdf-install)

install_mfront_desc(Author.txt)
install_mfront_desc(Bounds.txt)
install_mfront_desc(Date.txt)
install_mfront_desc(Description.txt)

install_mfront_desc2(MaterialLaw Constant.txt)
install_mfront_desc2(MaterialLaw Function.txt)
