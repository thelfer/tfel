mfm_test_generator_python_module(_mfm_test_generator
  pymfm_test_generator.cxx
  MFMTestGeneratorFileExport.cxx)

target_link_libraries(py_mfm_test_generator__mfm_test_generator
  PRIVATE
  MFMTestGenerator
  TFELMTest TFELMaterial
  TFELMath  TFELUtilities
  TFELException
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_script(mfm_test_generator __init__.py)
