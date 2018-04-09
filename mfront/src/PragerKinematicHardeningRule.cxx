/*!
 * \file   PragerKinematicHardeningRule.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   04/04/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/PragerKinematicHardeningRule.hxx"

namespace mfront {

  namespace bbrick {

    std::string PragerKinematicHardeningRule::buildBackStrainImplicitEquations(
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const std::string& fid,
        const std::string& kid,
        const bool b) const {
      const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
      const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
      auto c = std::string{};
      c = "f" + an + " -= ";
      c += "(this->dp" + fid + ")*dseq" + fid + "_ds" + fid + ";\n";
      if (b) {
        c += "df" + an + "_ddp" + fid + " = -dseq" + fid + "_ds" + fid + ";\n";
        c += sp.computeDerivatives(bd, an, "-(this->dp" + fid + ")*d2seq" +
                                               fid + "_ds" + fid + "ds" + fid);
        c += "df" + an + "_dd" + an + " += ";
        c += "(this->dp" + fid + ")*d2seq" + fid + "_ds" + fid + "ds" + fid +
             "*theta*2*(this->" + Cn + ")/3;\n";
      }
      return c;
    }  // end of PragerKinematicHardeningRule::buildBackStrainImplicitEquations

    PragerKinematicHardeningRule::~PragerKinematicHardeningRule() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
