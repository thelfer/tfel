/*!
 * \file   mfront/src/KinematicHardeningRule.cxx
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

#include "MFront/BehaviourBrick/KinematicHardeningRule.hxx"

namespace mfront::bbrick {

  std::string KinematicHardeningRule::getVariableId(const std::string& n,
                                                    const std::string& fid,
                                                    const std::string& kid) {
    if (kid.empty()) {
      return "khr_" + n + fid;
    }
    return "khr_" + n + fid + '_' + kid;
  }  // end of getVariableId

  KinematicHardeningRule::~KinematicHardeningRule() = default;

}  // end of namespace mfront::bbrick
