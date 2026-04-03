/*!
 * \file   mfront/src/StressCriterion.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/BehaviourBrick/StressCriterion.hxx"

namespace mfront::bbrick {

  std::string StressCriterion::getVariableId(const std::string& n,
                                             const std::string& fid,
                                             const Role r) {
    if (r == StressCriterion::FLOWCRITERION) {
      return "scf_" + n + fid;
    }
    return "sc_" + n + fid;
  }  // end of StressCriterion::getVariableId

  StressCriterion::~StressCriterion() = default;

}  // end of namespace mfront::bbrick
