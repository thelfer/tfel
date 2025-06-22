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

#include <boost/python.hpp>
#include "MFront/BehaviourSymmetryType.hxx"

void declareBehaviourSymmetryType();
void declareBehaviourSymmetryType() {
  boost::python::enum_<mfront::BehaviourSymmetryType>("BehaviourSymmetryType")
      .value("ISOTROPIC", mfront::BehaviourSymmetryType::ISOTROPIC)
      .value("ORTHOTROPIC", mfront::BehaviourSymmetryType::ORTHOTROPIC);

}  // end of declareBehaviourSymmetryType
