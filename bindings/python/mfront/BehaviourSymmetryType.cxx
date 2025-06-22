/*!
 * \file   BehaviourSymmetryType.cxx
 * \brief
 * \author Thomas Helfer
 * \date   29/03/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/BehaviourSymmetryType.hxx"

void declareBehaviourSymmetryType(pybind11::module_&);

void declareBehaviourSymmetryType(pybind11::module_& m) {
  pybind11::enum_<mfront::BehaviourSymmetryType>(m,

                                                 "BehaviourSymmetryType")
      .value("ISOTROPIC", mfront::BehaviourSymmetryType::ISOTROPIC)
      .value("ORTHOTROPIC", mfront::BehaviourSymmetryType::ORTHOTROPIC);
}  // end of declareBehaviourSymmetryType
