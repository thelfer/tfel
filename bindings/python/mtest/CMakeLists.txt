mtest_python_module(_mtest
  pymtest.cxx
  Behaviour.cxx
  SchemeBase.cxx
  SingleStructureScheme.cxx
  MTest.cxx
  MTestParser.cxx
  PipeMesh.cxx
  PipeTest.cxx
  CurrentState.cxx
  StructureCurrentState.cxx
  StudyCurrentState.cxx
  SolverWorkSpace.cxx
  MFrontLogStream.cxx)

target_link_libraries(py_mtest__mtest
  PRIVATE
  TFELMTest TFELMaterial
  TFELMath  TFELUtilities
  TFELException
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_script(mtest __init__.py)
