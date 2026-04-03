add_subdirectory(material)

tfel_python_module(_tfel
  tfel.cxx
  Config.cxx
  PhysicalConstants.cxx)
target_link_libraries(py_tfel__tfel
  PRIVATE TFELConfig ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_module(tests tests.cxx
  TestResult.cxx
  XMLTestOutput.cxx)
target_link_libraries(py_tfel_tests
  PRIVATE TFELTests ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_module(utilities utilities.cxx
  Data.cxx
  TextData.cxx)
target_link_libraries(py_tfel_utilities
  PRIVATE TFELUtilities ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_module(glossary
  pyglossary.cxx
  Glossary.cxx
  GlossaryEntry.cxx)
target_link_libraries(py_tfel_glossary
  PRIVATE TFELGlossary ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

set(TFEL_PYTHON_MATH_MODULE_SOURCES
  math.cxx
  vector.cxx
  tvector.cxx
  stensor.cxx
  st2tost2.cxx
  Evaluator.cxx
  AccelerationAlgorithms.cxx)

tfel_python_module(math ${TFEL_PYTHON_MATH_MODULE_SOURCES})
target_compile_options(py_tfel_math PRIVATE "-DTFEL_NUMPY_SUPPORT")
target_link_libraries(py_tfel_math
   PRIVATE TFELMath TFELMathParser TFELException
   ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})


set(PY_TFEL_SYSTEM_SOURCES)
list(APPEND PY_TFEL_SYSTEM_SOURCES
     system.cxx
     LibraryInformation.cxx
     ExternalLibraryManager.cxx
     ExternalMaterialKnowledgeDescription.cxx
     ExternalMaterialPropertyDescription.cxx
     ExternalBehaviourDescription.cxx)

if(NOT WIN32)
  list(APPEND PY_TFEL_SYSTEM_SOURCES
       ProcessManagerCommand.cxx)
endif(NOT WIN32)

tfel_python_module(system ${PY_TFEL_SYSTEM_SOURCES})
target_link_libraries(py_tfel_system
  PRIVATE TFELSystem
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_script(tfel __init__.py)
