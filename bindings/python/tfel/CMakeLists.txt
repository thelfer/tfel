include_directories("${PROJECT_SOURCE_DIR}/bindings/python/include")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${Boost_INCLUDE_DIRS}")
include_directories("${PYTHON_INCLUDE_DIRS}")

link_directories("${PROJECT_BINARY_DIR}/src/Exception")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")
link_directories("${PROJECT_BINARY_DIR}/src/Math")
link_directories("${PROJECT_BINARY_DIR}/src/System")
link_directories("${PROJECT_BINARY_DIR}/src/Glossary")
link_directories("${PROJECT_BINARY_DIR}/src/Tests")
link_directories("${Boost_LIBRARY_DIRS}")
link_directories("${PYTHON_LIBRARY_PATH}")

tfel_python_module(tests tests.cxx
  TestResult.cxx)
target_link_libraries(py_tfel_tests TFELTests)

tfel_python_module(utilities utilities.cxx
  TextData.cxx)
target_link_libraries(py_tfel_utilities TFELUtilities)

tfel_python_module(glossary
  pyglossary.cxx
  Glossary.cxx
  GlossaryEntry.cxx)
target_link_libraries(py_tfel_glossary TFELGlossary)

tfel_python_module(math math.cxx
  vector.cxx
  tvector.cxx
  stensor.cxx)
target_link_libraries(py_tfel_math TFELMath)

tfel_python_module(material
  material.cxx
  ModellingHypothesis.cxx
  OutOfBoundsPolicy.cxx)

if(NOT WIN32)
  tfel_python_module(system system.cxx
    ProcessManagerCommand.cxx
    ExternalLibraryManager.cxx
    ExternalBehaviourDescription.cxx)
else(NOT WIN32)
  tfel_python_module(system system.cxx
    ExternalLibraryManager.cxx
    ExternalBehaviourDescription.cxx)
endif(NOT WIN32)
target_link_libraries(py_tfel_system
  TFELSystem)

tfel_python_script(tfel __init__.py)
