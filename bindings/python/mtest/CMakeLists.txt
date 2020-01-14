mtest_python_module(_mtest
  pymtest.cxx
  Behaviour.cxx
  SchemeBase.cxx
  SingleStructureScheme.cxx
  Constraint.cxx
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
  TFELMTest
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

tfel_python_script(mtest __init__.py)
