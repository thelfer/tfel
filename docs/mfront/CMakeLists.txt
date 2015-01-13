macro(install_mfront_desc file)
  install(FILES ${file} DESTINATION share/doc/mfront
    COMPONENT mfront)
endmacro(install_mfront_desc file)

macro(install_mfront_desc2 parser file)
  install(FILES ${parser}/${file} DESTINATION share/doc/mfront/${parser}
    COMPONENT mfront)
endmacro(install_mfront_desc2 parser file)

install_mfront_desc(Author.txt)
install_mfront_desc(Bounds.txt)
install_mfront_desc(Date.txt)
install_mfront_desc(Description.txt)
install_mfront_desc(Parameter.txt)

install_mfront_desc2(MaterialLaw Constant.txt)
install_mfront_desc2(MaterialLaw Function.txt)

if(HAVE_LATEX AND GENERATE_REFERENCE_DOC)

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
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/glossary.tex.in
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/Champs.tex
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/mfront.tex
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/glossary.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
        ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      mfront.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
        ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      mfront.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse mfront
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist env.idx
    COMMAND   ${MAKEINDEX_COMPILER}
    ARGS      -s ${CMAKE_CURRENT_SOURCE_DIR}/style.ist mkeys.idx
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
        ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      mfront.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
        ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      mfront.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 mfront.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      mfront.ps
    COMMENT   "mfront.pdf")

  ADD_CUSTOM_COMMAND(
    OUTPUT    behaviours.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/behaviours.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/annexe-orthotropie.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/InternalNames.tex.in
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/glossary.tex.in
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.tex
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/annexe-orthotropie.tex
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/InternalNames.tex
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/glossary.tex
    # additional dependencies to avoid a conflict: both mfront.tex.in
    # and behaviours.tex.in includes glossary.tex, so the latex
    # processing generates in both cases a glossary.aux file.
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/mfront.pdf
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      behaviours.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      behaviours.tex
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
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      behaviours.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      behaviours.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 behaviours.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      behaviours.ps
    COMMENT   "behaviours.pdf")

  ADD_CUSTOM_COMMAND(
    OUTPUT    finitestrain.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/finitestrain.tex.in
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.pdf
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      finitestrain.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      finitestrain.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse finitestrain
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      finitestrain.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      finitestrain.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 finitestrain.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      finitestrain.ps
    COMMENT   "finitestrain.pdf")

  ADD_CUSTOM_COMMAND(
    OUTPUT    planestress.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/planestress.tex.in
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/finitestrain.pdf
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      planestress.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      planestress.tex
    COMMAND   BSTINPUTS=@abs_top_srcdir@/docs/tex/texmf/bibtex ${BIBTEX_COMPILER}
    ARGS      -terse planestress
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      planestress.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      planestress.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 planestress.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      planestress.ps
    COMMENT   "planestress.pdf")

  ADD_CUSTOM_COMMAND(
    OUTPUT    umat.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/umat.tex.in
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/planestress.pdf
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      umat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      umat.tex
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
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      umat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      umat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 umat.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      umat.ps
    COMMENT   "umat.pdf")

  ADD_CUSTOM_COMMAND(
    OUTPUT    zmat.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/zmat.tex.in
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/umat.pdf
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      zmat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      zmat.tex
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
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      zmat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      zmat.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 zmat.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      zmat.ps
    COMMENT   "zmat.pdf")

  ADD_CUSTOM_COMMAND(
    OUTPUT    aster.pdf
    DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/aster.tex.in
    DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/zmat.pdf
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      aster.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      aster.tex
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
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      aster.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${LATEX_COMPILER}
    ARGS      -interaction=batchmode
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/latex/NT
    ARGS      --include-directory=@abs_top_srcdir@/docs/tex/texmf/tex/images
    ARGS      aster.tex
    COMMAND   TEXMFHOME=@abs_top_srcdir@/docs/tex/texmf        ${DVIPS_CONVERTER}
    ARGS      -R0 aster.dvi
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      aster.ps
    COMMENT   "aster.pdf")

  add_custom_target(mfront-doc-pdf ALL DEPENDS mfront.pdf)
  add_custom_target(behaviours-doc-pdf ALL DEPENDS behaviours.pdf)
  add_custom_target(finitestrain-doc-pdf ALL DEPENDS finitestrain.pdf)
  add_custom_target(planestress-doc-pdf ALL DEPENDS planestress.pdf)
  add_custom_target(umat-doc-pdf ALL DEPENDS umat.pdf)
  add_custom_target(zmat-doc-pdf ALL DEPENDS zmat.pdf)
  add_custom_target(aster-doc-pdf ALL DEPENDS aster.pdf)

  add_dependencies(doc-pdf
    mfront-doc-pdf
    behaviours-doc-pdf
    finitestrain-doc-pdf
    planestress-doc-pdf
    umat-doc-pdf
    zmat-doc-pdf
    aster-doc-pdf)

  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/mfront.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/finitestrain.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/planestress.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/umat.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/zmat.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/aster.pdf
    DESTINATION share/doc/mfront/references
    COMPONENT docs)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/mfront.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/behaviours.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/finitestrain.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/planestress.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/umat.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/zmat.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/aster.pdf
    DESTINATION share/doc/tfel/web/documents/mfront
    COMPONENT website)

endif(HAVE_LATEX AND GENERATE_REFERENCE_DOC)
