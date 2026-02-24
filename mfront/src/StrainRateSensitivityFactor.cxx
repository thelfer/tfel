/*!
 * \file   mfront/src/StrainRateSensitivityFactor.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/11/2024
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/BehaviourBrick/StrainRateSensitivityFactor.hxx"

namespace mfront::bbrick {

  std::string StrainRateSensitivityFactor::getVariableId(
      const std::string& n, const std::string& fid, const std::string& id) {
    if (fid.empty()) {
      return "mfront_srsf_" + n + "_" + id;
    }
    return "mfront_srsf_" + n + fid + "_" + id;
  }  // end of getVariableId

  StrainRateSensitivityFactor::~StrainRateSensitivityFactor() = default;

}  // end of namespace mfront::bbrick
