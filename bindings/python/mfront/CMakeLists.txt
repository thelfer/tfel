add_subdirectory(umatxx)
add_subdirectory(mtest)
add_subdirectory(database)

mfront_python_module(_mfront mfront.cxx
  madnex.cxx
  MaterialKnowledgeDescriptionAttribute.cxx
  AbstractDSL.cxx
  FileDescription.cxx
  TargetsDescription.cxx
  CompiledTargetDescriptionBase.cxx
  LibraryDescription.cxx
  DSLFactory.cxx
  MaterialPropertyDSL.cxx
  MaterialPropertyDescription.cxx
  AbstractBehaviourDSL.cxx
  VariableBoundsDescription.cxx
  VariableDescription.cxx
  BehaviourData.cxx
  BehaviourSymmetryType.cxx
  BehaviourDescription.cxx
  SearchPathsHandler.cxx
  GeneratorOptions.cxx
  MakefileGenerator.cxx
  CMakeGenerator.cxx
  OverridableImplementation.cxx)
target_link_libraries(py_mfront__mfront
  PRIVATE
  TFELMFront
  MFrontLogStream TFELMaterial
  TFELMathParser  TFELSystem
  TFELUtilities   TFELGlossary
  TFELException   TFELConfig
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

if(TFEL_HAVE_MADNEX)
  target_compile_definitions(py_mfront__mfront
    PUBLIC MFRONT_HAVE_MADNEX)
  target_link_libraries(py_mfront__mfront
    PRIVATE madnex::Madnex)
endif(TFEL_HAVE_MADNEX)

tfel_python_script(mfront __init__.py)
