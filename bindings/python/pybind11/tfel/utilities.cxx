/*!
 * \file  bindings/python/tfel/utilities.cxx
 * \brief
 * \author Thomas Helfer
 * \date 30/08/2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include "TFEL/Macros.hxx"

#define TFEL_UTILITIES_MODULE_NAME \
  TFEL_PP_JOIN(utilities_, TFEL_SUFFIX_FOR_PYTHON_MODULES)

void declareData(pybind11::module_&);
void declareTextData(pybind11::module_&);

#ifdef TFEL_SUFFIX_FOR_PYTHON_MODULES
PYBIND11_MODULE(TFEL_UTILITIES_MODULE_NAME, m) {
#else
PYBIND11_MODULE(utilities, m) {
#endif
  declareData(m);
  declareTextData(m);
}
