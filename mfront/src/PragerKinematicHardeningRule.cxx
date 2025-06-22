/*!
 * \file   PragerKinematicHardeningRule.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/StressCriterion.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/PragerKinematicHardeningRule.hxx"

namespace mfront::bbrick {

  std::string PragerKinematicHardeningRule::buildBackStrainImplicitEquations(
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const StressCriterion& fc,
      const std::vector<std::shared_ptr<KinematicHardeningRule>>& khrs,
      const std::string& fid,
      const std::string& kid,
      const bool b) const {
    const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
    auto c = std::string{};
    c = "f" + an + " -= ";
    c += "(this->dp" + fid + ") * n" + fid + ";\n";
    if (b) {
      // opposite of dfa_ds
      const auto mdf_ds = "(this->dp" + fid + ") * dn" + fid + "_ds" + fid;
      c += "df" + an + "_ddp" + fid + " = - n" + fid + ";\n";
      c += sp.generateImplicitEquationDerivatives(
          bd, "StrainStensor", an, "-" + mdf_ds, fc.isNormalDeviatoric());
      auto kid2 = decltype(khrs.size()){};
      for (const auto& khr : khrs) {
        c += khr->generateImplicitEquationDerivatives(an, mdf_ds, fid,
                                                      std::to_string(kid2));
        ++kid2;
      }
    }
    return c;
  }  // end of buildBackStrainImplicitEquations

  PragerKinematicHardeningRule::~PragerKinematicHardeningRule() = default;

}  // end of namespace mfront::bbrick
