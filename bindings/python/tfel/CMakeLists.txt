if(TFEL_NUMPY_SUPPORT)
  tfel_library(TFELNumpySupport
    InitNumpy.cxx
    ndarray.cxx)
  target_include_directories(TFELNumpySupport
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/bindings/python/include>
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
  target_include_directories(TFELNumpySupport
    SYSTEM
    PUBLIC "${Boost_INCLUDE_DIRS}"
    PUBLIC "${PYTHON_INCLUDE_DIRS}")
  if(NUMPY_INCLUDE_DIRS) 
    target_include_directories(TFELNumpySupport
      SYSTEM
      PUBLIC "${NUMPY_INCLUDE_DIRS}")
  endif(NUMPY_INCLUDE_DIRS)
  if(TFEL_USES_CONAN)
    target_link_libraries(TFELNumpySupport
      ${CONAN_LIBS} ${PYTHON_LIBRARIES})
  else(TFEL_USES_CONAN)
    target_link_libraries(TFELNumpySupport
      PUBLIC ${Boost_NUMPY_LIBRARY} ${Boost_PYTHON_LIBRARY}
			 ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})
  endif(TFEL_USES_CONAN)
endif(TFEL_NUMPY_SUPPORT)

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
if(TFEL_NUMPY_SUPPORT)
  list(APPEND TFEL_PYTHON_MATH_MODULE_SOURCES
       AccelerationAlgorithms.cxx)
endif(TFEL_NUMPY_SUPPORT)

tfel_python_module(math ${TFEL_PYTHON_MATH_MODULE_SOURCES})
if(TFEL_NUMPY_SUPPORT)
  target_compile_options(py_tfel_math PRIVATE "-DTFEL_NUMPY_SUPPORT")
  if(TFEL_USES_CONAN)
    target_link_libraries(py_tfel_math
      PRIVATE
	  TFELNumpySupport TFELMath TFELMathParser
      TFELException ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})
  else(TFEL_USES_CONAN)
    target_link_libraries(py_tfel_math
     PRIVATE
     TFELNumpySupport TFELMath TFELMathParser
     TFELException ${Boost_NUMPY_LIBRARY}
     ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})
  endif(TFEL_USES_CONAN)
else(TFEL_NUMPY_SUPPORT)
  target_link_libraries(py_tfel_math
   PRIVATE
	 TFELMath TFELMathParser TFELException
   ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})
endif(TFEL_NUMPY_SUPPORT)

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
    ExternalMaterialKnowledgeDescription.cxx
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
