
/*!
 * \file  bindings/python/tfel/material.cxx
 * \brief
 * \author Thomas Helfer
 * \date 31/08/2010
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>

void declareModellingHypothesis(pybind11::module_&);
void declareOutOfBoundsPolicy(pybind11::module_&);
void declareCrystalStructure(pybind11::module_&);
void declarePiPlane(pybind11::module_&);
void declareHosford(pybind11::module_&);
void declareBarlat(pybind11::module_&);

PYBIND11_MODULE(material, m) {
  declareModellingHypothesis(m);
  declareOutOfBoundsPolicy(m);
  declareCrystalStructure(m);
  declarePiPlane(m);
  declareHosford(m);
  declareBarlat(m);
}
