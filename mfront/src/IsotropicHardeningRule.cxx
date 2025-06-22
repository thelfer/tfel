/*!
 * \file   mfront/src/IsotropicHardeningRule.cxx
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

#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"

namespace mfront {

  namespace bbrick {

    std::string IsotropicHardeningRule::getVariableId(const std::string& n,
                                                      const std::string& fid,
                                                      const std::string& id) {
      if (fid.empty()) {
        return "ihr_" + n + "_" + id;
      }
      return "ihr_" + n + fid + "_" + id;
    }  // end of IsotropicHardeningRule::getVariableId

    IsotropicHardeningRule::~IsotropicHardeningRule() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
