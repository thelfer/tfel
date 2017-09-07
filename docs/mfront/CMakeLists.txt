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
  if(TFEL_APPEND_VERSION)
    install(FILES ${file}.md
      DESTINATION share/doc/mfront-${TFEL_VERSION}
      COMPONENT mfront)
  else(TFEL_APPEND_VERSION)
    install(FILES ${file}.md DESTINATION share/doc/mfront
      COMPONENT mfront)
  endif(TFEL_APPEND_VERSION)
endmacro(install_mfront_desc file)

macro(install_mfront_desc2 directory file)
  # if(HAVE_PANDOC)
  #   ADD_CUSTOM_COMMAND(
  #     OUTPUT    ${file}-${directory}.html
  #     DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/${directory}/${file}.md
  #     COMMAND   ${PANDOC}
  #     ARGS      -f markdown-markdown_in_html_blocks+tex_math_single_backslash
  #     ARGS      --mathjax
  #     ARGS      ${ARGN}
  #     ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/${directory}/${file}.md -o ${file}-${directory}.html)
  # add_custom_target(mfront-${file}-${directory}-html ALL DEPENDS ${file}-${directory}.html)
  # add_dependencies(website mfront-${file}-${directory}-html)
  #   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}-${directory}.html
  #     DESTINATION share/doc/tfel/web/mfront/${directory}/${file}.html
  #     COMPONENT website)
  # endif(HAVE_PANDOC)
  if(TFEL_APPEND_VERSION)
    install(FILES ${directory}/${file}.md
      DESTINATION share/doc/mfront-${TFEL_VERSION}/${directory}
      COMPONENT mfront)
  else(TFEL_APPEND_VERSION)
    install(FILES ${directory}/${file}.md
      DESTINATION share/doc/mfront/${directory}
      COMPONENT mfront)
  endif(TFEL_APPEND_VERSION)
endmacro(install_mfront_desc2 directory file)

install_mfront_desc(Algorithm)
install_mfront_desc(Author)
install_mfront_desc(AuxiliaryStateVar)
install_mfront_desc(AuxiliaryStateVariable)
install_mfront_desc(AxialGrowth)
install_mfront_desc(Behaviour)
install_mfront_desc(Bounds)
install_mfront_desc(Brick)
install_mfront_desc(Coef)
install_mfront_desc(ComputeFinalStress)
install_mfront_desc(ComputeStiffnessTensor)
install_mfront_desc(ComputeStressFreeExpansion)
install_mfront_desc(ComputeStress)
install_mfront_desc(ComputeThermalExpansion)
install_mfront_desc(CompareToNumericalJacobian)
install_mfront_desc(Date)
install_mfront_desc(Description)
install_mfront_desc(DissipatedEnergy)
install_mfront_desc(DSL)
install_mfront_desc(ElasticMaterialProperties)
install_mfront_desc(Epsilon)
install_mfront_desc(ExternalStateVar)
install_mfront_desc(ExternalStateVariable)
install_mfront_desc(Import)
install_mfront_desc(InitializeJacobian)
install_mfront_desc(InitializeJacobianInvert)
install_mfront_desc(InitJacobian)
install_mfront_desc(InitJacobianInvert)
install_mfront_desc(Includes)
install_mfront_desc(IntegrationVariable)
install_mfront_desc(Interface)
install_mfront_desc(InternalEnergy)
install_mfront_desc(InitLocalVars)
install_mfront_desc(InitLocalVariables)
install_mfront_desc(InitializeLocalVariables)
install_mfront_desc(IsotropicBehaviour)
install_mfront_desc(IsotropicElasticBehaviour)
install_mfront_desc(IterMax)
install_mfront_desc(JacobianComparisonCriterion)
install_mfront_desc(JacobianComparisonCriterium)
install_mfront_desc(Library)
install_mfront_desc(Link)
install_mfront_desc(LocalVar)
install_mfront_desc(Material)
install_mfront_desc(MaterialLaw)
install_mfront_desc(MaterialProperty)
install_mfront_desc(MaximumNumberOfIterations)
install_mfront_desc(Model)
install_mfront_desc(NumericallyComputedJacobianBlocks)
install_mfront_desc(OrthotropicBehaviour)
install_mfront_desc(Parameter)
install_mfront_desc(Parser)
install_mfront_desc(PerturbationValueForNumericalJacobianComputation)
install_mfront_desc(PhysicalBounds)
install_mfront_desc(Private)
install_mfront_desc(Profiling)
install_mfront_desc(Relocation)
install_mfront_desc(RequireStiffnessOperator)
install_mfront_desc(RequireStiffnessTensor)
install_mfront_desc(RequireThermalExpansionCoefficientTensor)
install_mfront_desc(Sources)
install_mfront_desc(StateVar)
install_mfront_desc(StateVariable)
install_mfront_desc(StaticVar)
install_mfront_desc(StaticVariable)
install_mfront_desc(Swelling)
install_mfront_desc(TangentOperator)
install_mfront_desc(Theta)
install_mfront_desc(UpdateAuxiliaryStateVars)
install_mfront_desc(UpdateAuxiliaryStateVariables)
install_mfront_desc(UsableInPurelyImplicitResolution)
install_mfront_desc(UseQt)

install_mfront_desc2(MaterialLaw Constant)
install_mfront_desc2(MaterialLaw Function)
install_mfront_desc2(MaterialLaw Input)
install_mfront_desc2(MaterialLaw Law)
install_mfront_desc2(MaterialLaw MFront)
install_mfront_desc2(MaterialLaw Output)

install_mfront_desc2(Model ConstantMaterialProperty)
install_mfront_desc2(Model Domain)
install_mfront_desc2(Model Domains)
install_mfront_desc2(Model Function)
install_mfront_desc2(Model Input)
install_mfront_desc2(Model GlobalParameter)
install_mfront_desc2(Model LocalParameter)
install_mfront_desc2(Model Model)
install_mfront_desc2(Model Output)

install_mfront_desc2(bricks StandardElasticity)

if(HAVE_LATEX AND GENERATE_REFERENCE_DOC)

  configure_file(glossary.tex.in
    glossary.tex @ONLY)
  add_subdirectory(general)
  add_subdirectory(behaviours)
  add_subdirectory(finitestrain)
  add_subdirectory(planestress)
  add_subdirectory(castem)
  add_subdirectory(aster)
  add_subdirectory(zmat)

endif(HAVE_LATEX AND GENERATE_REFERENCE_DOC)

add_subdirectory(abaqus)
add_subdirectory(ansys)