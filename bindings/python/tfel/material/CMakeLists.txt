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

if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
  tfel_python_script_base(tfel_${TFEL_SUFFIX_FOR_PYTHON_MODULES}/material __init__.py)
else(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
  tfel_python_script_base(tfel/material __init__.py)
endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
