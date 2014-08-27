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
configure_file(finitestrain.tex.in
               finitestrain.tex @ONLY)
configure_file(planestress.tex.in
               planestress.tex @ONLY)
configure_file(umat.tex.in
               umat.tex @ONLY)
configure_file(zmat.tex.in
               zmat.tex @ONLY)
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
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mfront.tex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mfront.tex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse mfront
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist env.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mfront.tex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode mfront.tex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 mfront.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      mfront.ps
    COMMENT   "mfront.pdf")

ADD_CUSTOM_COMMAND(
    OUTPUT    behaviours.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/behaviours.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/annexe-orthotropie.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images behaviours.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images behaviours.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse behaviours
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist env.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist headers.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist tfel.idx
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images behaviours.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images behaviours.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 behaviours.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      behaviours.ps
    COMMENT   "behaviours.pdf")

ADD_CUSTOM_COMMAND(
    OUTPUT    finitestrain.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/finitestrain.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images finitestrain.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images finitestrain.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse finitestrain
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images finitestrain.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images finitestrain.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 finitestrain.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      finitestrain.ps
    COMMENT   "finitestrain.pdf")

ADD_CUSTOM_COMMAND(
    OUTPUT    planestress.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/planestress.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images planestress.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images planestress.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse planestress
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images planestress.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images planestress.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 planestress.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      planestress.ps
    COMMENT   "planestress.pdf")

ADD_CUSTOM_COMMAND(
    OUTPUT    umat.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/umat.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images umat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images umat.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse umat
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist env.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist headers.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist tfel.idx
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images umat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images umat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 umat.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      umat.ps
    COMMENT   "umat.pdf")

ADD_CUSTOM_COMMAND(
    OUTPUT    zmat.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/zmat.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images zmat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images zmat.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse zmat
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist env.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist headers.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist tfel.idx
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images zmat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images zmat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 zmat.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      zmat.ps
    COMMENT   "zmat.pdf")

ADD_CUSTOM_COMMAND(
    OUTPUT    aster.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/aster.tex.in
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images aster.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images aster.tex
#    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
#    ARGS      -terse aster
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist env.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist headers.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist tfel.idx
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images aster.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images aster.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 aster.dvi
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

add_custom_target(finitestrain-doc-pdf echo DEPENDS finitestrain.pdf)
add_custom_target(finitestrain-doc-pdf-install
                  ${CMAKE_COMMAND} -E copy finitestrain.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/finitestrain.pdf)
add_dependencies(finitestrain-doc-pdf-install finitestrain-doc-pdf)

add_custom_target(planestress-doc-pdf echo DEPENDS planestress.pdf)
add_custom_target(planestress-doc-pdf-install
                  ${CMAKE_COMMAND} -E copy planestress.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/planestress.pdf)
add_dependencies(planestress-doc-pdf-install planestress-doc-pdf)

add_custom_target(umat-doc-pdf echo DEPENDS umat.pdf)
add_custom_target(umat-doc-pdf-install
                  ${CMAKE_COMMAND} -E copy umat.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/umat.pdf)
add_dependencies(umat-doc-pdf-install umat-doc-pdf)

add_custom_target(zmat-doc-pdf echo DEPENDS zmat.pdf)
add_custom_target(zmat-doc-pdf-install
                  ${CMAKE_COMMAND} -E copy zmat.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/zmat.pdf)
add_dependencies(zmat-doc-pdf-install zmat-doc-pdf)

add_custom_target(aster-doc-pdf echo DEPENDS aster.pdf)
add_custom_target(aster-doc-pdf-install
                  ${CMAKE_COMMAND} -E copy aster.pdf ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/aster.pdf)
add_dependencies(aster-doc-pdf-install aster-doc-pdf)

add_dependencies(doc-pdf
  mfront-doc-pdf
  behaviours-doc-pdf
  finitestrain-doc-pdf
  planestress-doc-pdf
  umat-doc-pdf
  zmat-doc-pdf
  aster-doc-pdf)

add_dependencies(doc-pdf-install
  mfront-doc-pdf-install
  behaviours-doc-pdf-install
  finitestrain-doc-pdf-install
  planestress-doc-pdf-install
  umat-doc-pdf-install
  zmat-doc-pdf-install
  aster-doc-pdf-install)

install_mfront_desc(Author.txt)
install_mfront_desc(Bounds.txt)
install_mfront_desc(Date.txt)
install_mfront_desc(Description.txt)
install_mfront_desc(Parameter.txt)

install_mfront_desc2(MaterialLaw Constant.txt)
install_mfront_desc2(MaterialLaw Function.txt)
