include_directories("${PROJECT_SOURCE_DIR}/bindings/python/include")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories(SYSTEM "${Boost_INCLUDE_DIRS}")
include_directories(SYSTEM "${Python_INCLUDE_DIRS}")

link_directories("${PROJECT_BINARY_DIR}/src/Exception")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")
link_directories("${PROJECT_BINARY_DIR}/src/Math")
link_directories("${PROJECT_BINARY_DIR}/src/System")
link_directories("${PROJECT_BINARY_DIR}/src/Glossary")
link_directories("${PROJECT_BINARY_DIR}/src/Tests")
link_directories("${Boost_LIBRARY_DIRS}")
link_directories("${Python_LIBRARY_PATH}")

tfel_python_module(_tfel tfel.cxx
  PhysicalConstants.cxx)
target_link_libraries(py_tfel__tfel
  PRIVATE ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_module(tests tests.cxx
  TestResult.cxx
  XMLTestOutput.cxx)
target_link_libraries(py_tfel_tests
  PRIVATE TFELTests ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

if(NOT WIN32)
tfel_python_module(utilities utilities.cxx
  Data.cxx
  TextData.cxx)
target_link_libraries(py_tfel_utilities
  PRIVATE TFELUtilities ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})
endif(NOT WIN32)

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
  Evaluator.cxx)


tfel_python_module(math ${TFEL_PYTHON_MATH_MODULE_SOURCES})
target_link_libraries(py_tfel_math
   PRIVATE TFELMath TFELMathParser TFELException
   ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_module(material
  material.cxx
  ModellingHypothesis.cxx
  OutOfBoundsPolicy.cxx
  CrystalStructure.cxx
  PiPlane.cxx
  Barlat.cxx
  Hosford.cxx)
target_link_libraries(py_tfel_material
  PRIVATE
  TFELMaterial TFELMath TFELException
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

if(NOT WIN32)
  tfel_python_module(system system.cxx
    ProcessManagerCommand.cxx
    LibraryInformation.cxx
    ExternalLibraryManager.cxx
    ExternalMaterialPropertyDescription.cxx
    ExternalBehaviourDescription.cxx)
else(NOT WIN32)
  tfel_python_module(system system.cxx
    LibraryInformation.cxx
    ExternalLibraryManager.cxx
    ExternalBehaviourDescription.cxx)
endif(NOT WIN32)
target_link_libraries(py_tfel_system
  PRIVATE
  TFELSystem
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_script(tfel __init__.py)
