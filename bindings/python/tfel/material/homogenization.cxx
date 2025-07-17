
/*!
 * \file  bindings/python/tfel/material/homogenization.cxx
 * \brief
 * \author Antoine MARTIN
 * \date 15/05/2025
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include "TFEL/Macros.hxx"

#define TFEL_MATERIAL_HOMOGENIZATION_MODULE_NAME \
  TFEL_PP_JOIN(homogenization_, TFEL_SUFFIX_FOR_PYTHON_MODULES)

void declareHillTensors(pybind11::module_&);
void declareLocalisationTensors(pybind11::module_&);
void declareLinearHomogenizationSchemes(pybind11::module_&);
void declareLinearHomogenizationBounds(pybind11::module_&);
void declareMicrostructure(pybind11::module_&);
void declareMicrostructureHomogenization(pybind11::module_&);


#ifdef TFEL_SUFFIX_FOR_PYTHON_MODULES
PYBIND11_MODULE(TFEL_MATERIAL_HOMOGENIZATION_MODULE_NAME, m) {
#else
PYBIND11_MODULE(homogenization, m) {
#endif
  declareHillTensors(m);
  declareLocalisationTensors(m);
  declareLinearHomogenizationSchemes(m);
  declareLinearHomogenizationBounds(m);
  declareMicrostructure(m);
  declareMicrostructureHomogenization(m);
}
