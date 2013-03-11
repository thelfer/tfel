include_directories("${PROJECT_SOURCE_DIR}/bindings/python/include")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${Boost_INCLUDE_DIRS}")
include_directories("${PYTHON_INCLUDE_DIRS}")

link_directories("${PROJECT_BINARY_DIR}/src/Exception")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")
link_directories("${PROJECT_BINARY_DIR}/src/Math")
link_directories("${PROJECT_BINARY_DIR}/src/System")
link_directories("${Boost_LIBRARY_DIRS}")
link_directories("${PYTHON_LIBRARY_PATH}")

tfel_python_module(math math.cxx
  tvector.cxx
  stensor.cxx)
target_link_libraries(py_tfel_math TFELMath)

tfel_python_module(material
  ModellingHypothesis.cxx)

tfel_python_module(system system.cxx
  ProcessManagerCommand.cxx
  ExternalLibraryManager.cxx)
target_link_libraries(py_tfel_system
  TFELSystem)
