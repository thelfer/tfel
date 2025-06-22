/*!
 * \file  bindings/python/tfel/material.cxx
 * \brief
 * \author Thomas Helfer
 * \date 31/08/2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

void declareModellingHypothesis();
void declareOutOfBoundsPolicy();
void declareCrystalStructure();
void declarePiPlane();
void declareHosford();
void declareBarlat();

BOOST_PYTHON_MODULE(material) {
  declareModellingHypothesis();
  declareOutOfBoundsPolicy();
  declareCrystalStructure();
  declarePiPlane();
  declareHosford();
  declareBarlat();
}
