tfel_python_module(_tfel tfel.cxx
  PhysicalConstants.cxx)

tfel_python_module(tests tests.cxx
  TestResult.cxx
  XMLTestOutput.cxx)
target_link_libraries(py_tfel_tests TFELTests)

if(NOT WIN32)
tfel_python_module(utilities utilities.cxx
  Data.cxx
  TextData.cxx)
target_link_libraries(py_tfel_utilities TFELUtilities)
endif(NOT WIN32)

tfel_python_module(glossary
  pyglossary.cxx
  Glossary.cxx
  GlossaryEntry.cxx)
target_link_libraries(py_tfel_glossary TFELGlossary)

tfel_python_module(math math.cxx
  vector.cxx
  tvector.cxx
  stensor.cxx
  st2tost2.cxx
  Evaluator.cxx)
target_link_libraries(py_tfel_math TFELMath TFELMathParser TFELException)

tfel_python_module(material
  material.cxx
  ModellingHypothesis.cxx
  OutOfBoundsPolicy.cxx
  CrystalStructure.cxx
  PiPlane.cxx
  Barlat.cxx
  Hosford.cxx)
target_link_libraries(py_tfel_material TFELMaterial TFELMath TFELException)

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
  TFELSystem)

tfel_python_script(tfel __init__.py)
