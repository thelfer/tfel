add_subdirectory(umatxx)
add_subdirectory(mtest)

include_directories("${PROJECT_SOURCE_DIR}/bindings/python/include")
include_directories("${PROJECT_SOURCE_DIR}/mfront/include")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories(SYSTEM "${Boost_INCLUDE_DIRS}")
include_directories(SYSTEM "${PYTHON_INCLUDE_DIRS}")

link_directories("${Boost_LIBRARY_DIRS}")
link_directories("${PYTHON_LIBRARY_PATH}")
link_directories("${PROJECT_BINARY_DIR}/src/Exception")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")
link_directories("${PROJECT_BINARY_DIR}/src/Math")
link_directories("${PROJECT_BINARY_DIR}/src/System")
link_directories("${PROJECT_BINARY_DIR}/src/Glossary")
link_directories("${PROJECT_BINARY_DIR}/src/Tests")
link_directories("${PROJECT_BINARY_DIR}/mfront/src")

mfront_python_module(_mfront mfront.cxx
  AbstractDSL.cxx
  FileDescription.cxx
  TargetsDescription.cxx
  LibraryDescription.cxx
  DSLFactory.cxx
  AbstractBehaviourDSL.cxx
  VariableBoundsDescription.cxx
  VariableDescription.cxx
  BehaviourAttribute.cxx
  BehaviourData.cxx
  BehaviourSymmetryType
  BehaviourDescription.cxx
  SearchPathsHandler.cxx
  GeneratorOptions.cxx
  MakefileGenerator.cxx
  CMakeGenerator.cxx)
target_link_libraries(py_mfront__mfront TFELMFront
  MFrontLogStream TFELMaterial
  TFELMathParser  TFELSystem
  TFELUtilities   TFELGlossary
  TFELException   TFELConfig)

tfel_python_script(mfront __init__.py)
