include_directories("${PROJECT_SOURCE_DIR}/bindings/python/include")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/mfront/include")
include_directories("${PROJECT_SOURCE_DIR}/mtest/include")
include_directories("${Boost_INCLUDE_DIRS}")
include_directories("${PYTHON_INCLUDE_DIRS}")

link_directories("${PROJECT_BINARY_DIR}/mtest/src")
link_directories("${PROJECT_BINARY_DIR}/src/Exception")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")
link_directories("${PROJECT_BINARY_DIR}/src/Material")
link_directories("${PROJECT_BINARY_DIR}/src/Math")
link_directories("${PROJECT_BINARY_DIR}/src/System")
link_directories("${PROJECT_BINARY_DIR}/src/Tests")
link_directories("${Boost_LIBRARY_DIRS}")
link_directories("${PYTHON_LIBRARY_PATH}")

mtest_python_module(_mtest
  pymtest.cxx
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
  MFrontLogStream)

target_link_libraries(py_mtest__mtest
  TFELMTest)

tfel_python_script(mtest __init__.py)
