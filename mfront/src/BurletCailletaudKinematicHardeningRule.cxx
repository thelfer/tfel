/*!
 * \file   BurletCailletaudKinematicHardeningRule.cxx
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
#include "MFront/BehaviourBrick/BurletCailletaudKinematicHardeningRule.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription>
  BurletCailletaudKinematicHardeningRule::getOptions() const {
    auto opts = KinematicHardeningRuleBase::getOptions();
    opts.emplace_back("D", "back-strain callback coefficient",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("eta",
                      "weigth between the Amstrong-Frederic contribution "
                      "and the pure radial contribution",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  void BurletCailletaudKinematicHardeningRule::initialize(
      BehaviourDescription& bd,
      AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& kid,
      const DataMap& d) {
    auto get_mp = [&bd, &dsl, &d, &fid, &kid](
                      BehaviourDescription::MaterialProperty& mp,
                      const std::string& n) {
      const auto nid = KinematicHardeningRule::getVariableId(n, fid, kid);
      if (d.count(n) == 0) {
        tfel::raise(
            "BurletCailletaudKinematicHardeningRule::initialize: "
            "material property '" +
            n + "' is not defined");
      }
      mp = getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
      declareParameterOrLocalVariable(bd, mp, "real", nid);
    };
    KinematicHardeningRuleBase::initialize(bd, dsl, fid, kid, d);
    get_mp(this->D, "D");
    get_mp(this->eta, "eta");
  }  // end of initialize

  void BurletCailletaudKinematicHardeningRule::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& kid) const {
    KinematicHardeningRuleBase::endTreatment(bd, dsl, fid, kid);
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
    const auto en = KinematicHardeningRule::getVariableId("eta", fid, kid);
    auto c = generateMaterialPropertyInitializationCode(dsl, bd, Dn, this->D);
    c += generateMaterialPropertyInitializationCode(dsl, bd, en, this->eta);
    if (!c.empty()) {
      CodeBlock i;
      i.code = c;
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  std::string
  BurletCailletaudKinematicHardeningRule::buildBackStrainImplicitEquations(
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const StressCriterion& fc,
      const std::vector<std::shared_ptr<KinematicHardeningRule>>& khrs,
      const std::string& fid,
      const std::string& kid,
      const bool b) const {
    const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
    const auto Xn = KinematicHardeningRule::getVariableId("X", fid, kid) + "_";
    const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
    const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
    const auto en = KinematicHardeningRule::getVariableId("eta", fid, kid);
    const auto n = "n" + fid;
    auto df_ddp = "-" + n + "+(this->" + Dn + ")*((this->" + en + ")*" + an +
                  "_+(1-this->" + en + ")*2*(" + an + "_|" + n + ")/3*" + n +
                  ")";
    auto c = std::string{};
    if (b) {
      c += "df" + an + "_ddp" + fid + " = " + df_ddp + ";\n";
    } else {
      c += "const auto df" + an + "_ddp" + fid + " = eval(" + df_ddp + ");\n";
    }
    c += "f" + an + " += ";
    c += "(this->dp" + fid + ")*(df" + an + "_ddp" + fid + ");\n";
    if (b) {
      // opposite of the derivative of fa with respect to s
      auto mdf_ds = "(this->dp" + fid + ")*(";
      mdf_ds += "d" + n + "_ds" + fid;
      mdf_ds += "-(this->" + Dn + ")*(1-this->" + en + ")*2*(";
      mdf_ds += "(" + an + "_|" + n + ")*d" + n + "_ds" + fid + "+";
      mdf_ds += "(" + n + "^(" + an + "_|d" + n + "_ds" + fid + ")))/3)";
      c += sp.generateImplicitEquationDerivatives(
          bd, "StrainStensor", an, "-" + mdf_ds, fc.isNormalDeviatoric());
      // term specific to this back strain
      c += "df" + an + "_dd" + an + " += ";
      c += "(this->theta)*(this->dp" + fid + ")*(this->" + Dn + ")*(";
      c += "(this->" + en + ")*Stensor4::Id()+";
      c += "(1-this->" + en + ")*2*(" + n + "^" + n + ")/3);\n";
      // terms in common for all back strains
      auto kid2 = decltype(khrs.size()){};
      for (const auto& khr : khrs) {
        c += khr->generateImplicitEquationDerivatives(an, mdf_ds, fid,
                                                      std::to_string(kid2));
        ++kid2;
      }
    }
    return c;
  }  // end of buildBackStrainImplicitEquations

  BurletCailletaudKinematicHardeningRule::
      ~BurletCailletaudKinematicHardeningRule() = default;

}  // end of namespace mfront::bbrick
