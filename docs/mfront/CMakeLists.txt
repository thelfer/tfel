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

  configure_file(glossary.tex.in
    glossary.tex @ONLY)
  add_subdirectory(general)
  add_subdirectory(behaviours)
  add_subdirectory(finitestrain)
  add_subdirectory(planestress)
  add_subdirectory(umat)
  add_subdirectory(aster)
  add_subdirectory(zmat)

endif(HAVE_LATEX AND GENERATE_REFERENCE_DOC)
