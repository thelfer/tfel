/*!
 * \file   BurletCailletaudKinematicHardeningRule.cxx
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
#include "MFront/BehaviourBrick/BurletCailletaudKinematicHardeningRule.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription> BurletCailletaudKinematicHardeningRule::getOptions()
        const {
      auto opts = KinematicHardeningRuleBase::getOptions();
      opts.emplace_back("D", "back-strain callback coefficient",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of BurletCailletaudKinematicHardeningRule::getOptions()

    void BurletCailletaudKinematicHardeningRule::initialize(
        BehaviourDescription& bd,
        AbstractBehaviourDSL& dsl,
        const std::string& fid,
        const std::string& kid,
        const DataMap& d) {
      auto get_mp = [&bd, &dsl, &d, &fid, &kid](
          BehaviourDescription::MaterialProperty& mp, const std::string& n) {
        const auto nid = KinematicHardeningRule::getVariableId(n, fid, kid);
        if (d.count(n) == 0) {
          tfel::raise(
              "BurletCailletaudKinematicHardeningRule::initialize: "
              "material property '" +
              n + "' is not defined");
        }
        mp = getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
        declareParameterOrLocalVariable(bd, mp, nid);
      };
      KinematicHardeningRuleBase::initialize(bd, dsl, fid, kid, d);
      get_mp(this->D, "D");
    }  // end of BurletCailletaudKinematicHardeningRule::initialize

    void BurletCailletaudKinematicHardeningRule::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const std::string& fid,
        const std::string& kid) const {
      KinematicHardeningRuleBase::endTreatment(bd, dsl, fid, kid);
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
      const auto c =
          generateMaterialPropertyInitializationCode(dsl, bd, Dn, this->D);
      if (!c.empty()) {
        CodeBlock i;
        i.code = c;
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of BurletCailletaudKinematicHardeningRule::endTreatment

    std::string
    BurletCailletaudKinematicHardeningRule::buildBackStrainImplicitEquations(
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const std::vector<std::shared_ptr<KinematicHardeningRule>>& khrs,
        const std::string& fid,
        const std::string& kid,
        const bool b) const {
      const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
      const auto Xn = KinematicHardeningRule::getVariableId("X", fid, kid)+"_";
      const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
      const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
      auto c = std::string {};
      c += "f" + an + " -= ";
      c += "(this->dp" + fid + ")*(n" + fid + "-(this->" + Dn + ")*(" + an +
           "_|n" + fid + ")*" + an + "_);\n";
      if (b) {
        c += "df" + an + "_ddp" + fid + " = -(n" + fid + "-(this->" + Dn +
             ")*(" + an + "_|n" + fid + ")*" + an + "_);\n";
        c += sp.computeDerivatives(
            bd, an, "-(this->dp" + fid + ")*dn" + fid + "_ds" + fid);
        // term specific to this back strain
        c += "df" + an + "_dd" + an + " += ";
        c += "(this->theta)*(this->" + Dn + ")*(this->dp" + fid + ")*(";
        c += "("+an + "_^n" + fid + ")+(" + an + "_|n" + fid + ")*Stensor4::Id()";
        c += ");\n";
        // terms in common for all back strains
        auto kid2 = decltype(khrs.size()){};
        for (const auto khr : khrs) {
          const auto df_ds = "(this->dp" + fid + ")*(dn" + fid + "_ds" + fid +
                             "+(" + an + "_^(" + an + "_|dn" + fid + "_ds" +
                             fid + ")))";
          c += computeDerivatives(an, df_ds, fid, std::to_string(kid2));
          ++kid2;
        }
      }
      return c;
    }  // end of
    // BurletCailletaudKinematicHardeningRule::buildBackStrainImplicitEquations

    BurletCailletaudKinematicHardeningRule::~BurletCailletaudKinematicHardeningRule() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
