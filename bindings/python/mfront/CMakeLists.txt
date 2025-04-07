add_subdirectory(umatxx)
add_subdirectory(mtest)

mfront_python_module(_mfront mfront.cxx
  AbstractDSL.cxx
  FileDescription.cxx
  TargetsDescription.cxx
  LibraryDescription.cxx
  DSLFactory.cxx
  AbstractBehaviourDSL.cxx
  VariableDescription.cxx
  BehaviourAttribute.cxx
  BehaviourData.cxx
  BehaviourDescription.cxx
  GeneratorOptions.cxx
  MakefileGenerator.cxx)
target_link_libraries(py_mfront__mfront
  PRIVATE
  TFELMFront
  MFrontLogStream TFELMaterial
  TFELMathParser  TFELSystem
  TFELUtilities   TFELGlossary
  TFELException   TFELConfig
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_script(mfront __init__.py)
