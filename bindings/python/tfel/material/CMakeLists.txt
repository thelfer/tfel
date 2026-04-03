tfel_material_python_module(_material
  material.cxx
  ModellingHypothesis.cxx
  OutOfBoundsPolicy.cxx
  CrystalStructure.cxx
  PiPlane.cxx
  Barlat.cxx
  Hosford.cxx
  IsotropicModuli.cxx)
target_link_libraries(py_tfel_material__material
  PRIVATE TFELMaterial TFELMath TFELException
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_material_python_module(homogenization
  homogenization.cxx
  LinearHomogenizationSchemes.cxx
  LinearHomogenizationBounds.cxx
  HillTensors.cxx
  LocalisationTensors.cxx
  Microstructure.cxx
  MicrostructureLinearHomogenization.cxx)
target_link_libraries(py_tfel_material_homogenization
  PRIVATE TFELMaterial TFELMath TFELException
  ${TFEL_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES})

tfel_python_script(tfel/material __init__.py)
