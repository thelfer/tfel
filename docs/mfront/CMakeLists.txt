macro(install_mfront_desc file)
  # if(HAVE_PANDOC)
  #   ADD_CUSTOM_COMMAND(
  #     OUTPUT    ${file}.html
  #     DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/${file}.md
  #     DEPENDS   ${PROJECT_SOURCE_DIR}/docs/web/mfront-template.html
  #     DEPENDS   ${PROJECT_SOURCE_DIR}/docs/web/css/main.css
  #     COMMAND   ${PANDOC}
  #     ARGS      -f markdown-markdown_in_html_blocks+tex_math_single_backslash
  #     ARGS      --mathjax
  #     ARGS      ${ARGN}
  #     ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/${file}.md -o ${file}.html)
  #   add_custom_target(mfront-${file}-html ALL DEPENDS ${file}.html)
  #   add_dependencies(website mfront-${file}-html)
  #   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}.html
  #     DESTINATION share/doc/tfel/web/mfront
  #     COMPONENT website)
  # endif(HAVE_PANDOC)
  install(FILES ${file}.md DESTINATION share/doc/mfront
    COMPONENT mfront)
endmacro(install_mfront_desc file)

macro(install_mfront_desc2 parser file)
  # if(HAVE_PANDOC)
  #   ADD_CUSTOM_COMMAND(
  #     OUTPUT    ${file}-${parser}.html
  #     DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/${parser}/${file}.md
  #     COMMAND   ${PANDOC}
  #     ARGS      -f markdown-markdown_in_html_blocks+tex_math_single_backslash
  #     ARGS      --mathjax
  #     ARGS      ${ARGN}
  #     ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/${parser}/${file}.md -o ${file}-${parser}.html)
  # add_custom_target(mfront-${file}-${parser}-html ALL DEPENDS ${file}-${parser}.html)
  # add_dependencies(website mfront-${file}-${parser}-html)
  #   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}-${parser}.html
  #     DESTINATION share/doc/tfel/web/mfront/${parser}/${file}.html
  #     COMPONENT website)
  # endif(HAVE_PANDOC)
  install(FILES ${parser}/${file}.md DESTINATION share/doc/mfront/${parser}
    COMPONENT mfront)
endmacro(install_mfront_desc2 parser file)

install_mfront_desc(Algorithm)
install_mfront_desc(Author)
install_mfront_desc(AuxiliaryStateVar)
install_mfront_desc(AuxiliaryStateVariable)
install_mfront_desc(Behaviour)
install_mfront_desc(Bounds)
install_mfront_desc(Brick)
install_mfront_desc(Coef)
install_mfront_desc(Date)
install_mfront_desc(Description)
install_mfront_desc(DSL)
install_mfront_desc(ExternalStateVar)
install_mfront_desc(InitLocalVars)
install_mfront_desc(LocalVar)
install_mfront_desc(OrthotropicBehaviour)
install_mfront_desc(Parameter)
install_mfront_desc(Parser)
install_mfront_desc(PerturbationValueForNumericalJacobianComputation)
install_mfront_desc(PhysicalBounds)
install_mfront_desc(Private)
install_mfront_desc(Profiling)
install_mfront_desc(RequireStiffnessOperator)
install_mfront_desc(RequireStiffnessTensor)
install_mfront_desc(RequireThermalExpansionCoefficientTensor)
install_mfront_desc(Sources)
install_mfront_desc(StateVar)
install_mfront_desc(StateVariable)
install_mfront_desc(StaticVar)
install_mfront_desc(StaticVariable)
install_mfront_desc(TangentOperator)
install_mfront_desc(Theta)
install_mfront_desc(UpdateAuxiliaryStateVars)
install_mfront_desc(UpdateAuxiliaryStateVariables)
install_mfront_desc(UsableInPurelyImplicitResolution)
install_mfront_desc(UseQt)

install_mfront_desc2(MaterialLaw Constant)
install_mfront_desc2(MaterialLaw Function)

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
