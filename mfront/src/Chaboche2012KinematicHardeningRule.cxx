/*!
 * \file   mfront/src/Chaboche2012KinematicHardeningRule.cxx
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
#include "MFront/BehaviourBrick/Chaboche2012KinematicHardeningRule.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription>
  Chaboche2012KinematicHardeningRule::getOptions() const {
    auto opts = KinematicHardeningRuleBase::getOptions();
    opts.emplace_back("D", "back-strain callback coefficient",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("m", "", OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("Phi_inf", "", OptionDescription::MATERIALPROPERTY,
                      std::vector<std::string>(1u, "b"),
                      std::vector<std::string>{});
    opts.emplace_back("b", "", OptionDescription::MATERIALPROPERTY,
                      std::vector<std::string>(1u, "Phi_inf"),
                      std::vector<std::string>{});
    opts.emplace_back("w", "", OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of Chaboche2012KinematicHardeningRule::getOptions()

  void Chaboche2012KinematicHardeningRule::initialize(BehaviourDescription& bd,
                                                      AbstractBehaviourDSL& dsl,
                                                      const std::string& fid,
                                                      const std::string& kid,
                                                      const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto get_mp = [&bd, &dsl, &d, &fid, &kid](
                      BehaviourDescription::MaterialProperty& mp,
                      const std::string& n) {
      const auto nid = KinematicHardeningRule::getVariableId(n, fid, kid);
      if (d.count(n) == 0) {
        tfel::raise(
            "Chaboche2012KinematicHardeningRule::initialize: "
            "material property '" +
            n + "' is not defined");
      }
      mp = getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
      declareParameterOrLocalVariable(bd, mp, "real", nid);
    };
    KinematicHardeningRuleBase::initialize(bd, dsl, fid, kid, d);
    get_mp(this->D, "D");
    get_mp(this->m, "m");
    if ((d.count("b") != 0) || (d.count("Phi_inf") != 0)) {
      get_mp(this->bp, "b");
      get_mp(this->Phi_inf, "Phi_inf");
    }
    get_mp(this->w, "w");
    // reserved named
    const auto DJan = KinematicHardeningRule::getVariableId("DJa", fid, kid);
    const auto iDJan = KinematicHardeningRule::getVariableId("iDJa", fid, kid);
    const auto dDJan = KinematicHardeningRule::getVariableId("dDJa", fid, kid);
    const auto Psin = KinematicHardeningRule::getVariableId("Psi", fid, kid);
    const auto dPsin = KinematicHardeningRule::getVariableId("dPsi", fid, kid) +
                       "_d" +
                       KinematicHardeningRule::getVariableId("DJa", fid, kid);
    bd.reserveName(uh, Psin);
    bd.reserveName(uh, dPsin);
    bd.reserveName(uh, DJan);
    bd.reserveName(uh, iDJan);
    bd.reserveName(uh, dDJan);
    bd.reserveName(uh, "r" + DJan + "_m_1");
    bd.reserveName(uh, "r" + DJan + "_m");
    bd.reserveName(uh, DJan + "_m_1");
    bd.reserveName(uh, DJan + "_m");
  }  // end of Chaboche2012KinematicHardeningRule::initialize

  void Chaboche2012KinematicHardeningRule::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& kid) const {
    KinematicHardeningRuleBase::endTreatment(bd, dsl, fid, kid);
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
    const auto mn = KinematicHardeningRule::getVariableId("m", fid, kid);
    const auto wn = KinematicHardeningRule::getVariableId("w", fid, kid);
    const auto bn = KinematicHardeningRule::getVariableId("b", fid, kid);
    const auto Phi_infn =
        KinematicHardeningRule::getVariableId("Phi_inf", fid, kid);
    auto c = generateMaterialPropertyInitializationCode(dsl, bd, Dn, this->D);
    c += generateMaterialPropertyInitializationCode(dsl, bd, mn, this->m);
    c += generateMaterialPropertyInitializationCode(dsl, bd, wn, this->w);
    if (!this->bp.empty()) {
      c += generateMaterialPropertyInitializationCode(dsl, bd, bn, this->bp);
      c += generateMaterialPropertyInitializationCode(dsl, bd, Phi_infn,
                                                      this->Phi_inf);
    }
    if (!c.empty()) {
      CodeBlock i;
      i.code = c;
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of Chaboche2012KinematicHardeningRule::endTreatment

  std::string
  Chaboche2012KinematicHardeningRule::buildBackStrainImplicitEquations(
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const StressCriterion& fc,
      const std::vector<std::shared_ptr<KinematicHardeningRule>>& khrs,
      const std::string& fid,
      const std::string& kid,
      const bool b) const {
    const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
    const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
    const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
    const auto mn = KinematicHardeningRule::getVariableId("m", fid, kid);
    const auto wn = KinematicHardeningRule::getVariableId("w", fid, kid);
    const auto bn = KinematicHardeningRule::getVariableId("b", fid, kid);
    const auto Phi_infn =
        KinematicHardeningRule::getVariableId("Phi_inf", fid, kid);
    const auto DJan = KinematicHardeningRule::getVariableId("DJa", fid, kid);
    const auto dDJan = KinematicHardeningRule::getVariableId("dDJa", fid, kid);
    const auto Phin = KinematicHardeningRule::getVariableId("Phi", fid, kid);
    const auto dPhin =
        KinematicHardeningRule::getVariableId("dPhi", fid, kid) + "_ddp" + fid;
    const auto Psin = KinematicHardeningRule::getVariableId("Psi", fid, kid);
    const auto dPsin = KinematicHardeningRule::getVariableId("dPsi", fid, kid) +
                       "_d" +
                       KinematicHardeningRule::getVariableId("Ja", fid, kid);
    const auto n = "n" + fid;
    auto c = std::string{};
    c += "const auto " + DJan + " = ";
    c += "(this->" + Dn + ")*sigmaeq(" + an + "_);\n";
    c += "if((2 * " + DJan + " > 3 * (this->" + wn + "))&&";
    c += "(this->dp" + fid + ">0)){\n";
    if (b) {
      c += "const auto [" + Psin + "," + dPsin + "] = ";
      c += "[this," + DJan + "]() -> std::pair<real,real>{\n";
      c += "const auto r" + DJan + " = (" + DJan + "-3*(this->" + wn +
           ")/2)/((1 - (this->" + wn + "))*" + DJan + ");\n";
      c += "const auto r" + DJan + "_m = ";
      c += "pow(r" + DJan + ",this->" + mn + ");\n";
      c += "return {r" + DJan + "_m,";
      c += "r" + DJan + "_m * 3 * (this->" + wn + ") * (this->" + Dn +
           ") *(this->" + mn + ")/((2 * " + DJan + "-3 * (this->" + wn +
           ")) * " + DJan + ")};\n";
      c += "}();\n";
    } else {
      c += "const auto " + Psin + " = ";
      c += "pow((" + DJan + "-3*(this->" + wn + ")/2)/((1-this->" + wn + ")*" +
           DJan + "),this->" + mn + ");\n";
    }
    if (!this->bp.empty()) {
      const auto exp_mbpn =
          KinematicHardeningRule::getVariableId("exp_mbp", fid, kid);
      c += "const auto " + exp_mbpn + " = ";
      c += "exp(-(this->" + bn + ")*(this->p" + fid +
           " + ((this->theta) * (this->dp" + fid + "))));\n";
      c += "const auto " + Phin + " = ";
      c += Phi_infn + " + ";
      c += "(1 - " + Phi_infn + ") * " + exp_mbpn + ";\n";
      c += "const auto " + dPhin + " = ";
      c += "(this->" + bn + ") * (this->theta) *" + Phi_infn + " * " +
           exp_mbpn + ";\n";
      c += "f" + an + " -= ";
      c += "(this->dp" + fid + ") * (n" + fid + " - (this->" + Dn + ") * " +
           Phin + " * " + Psin + " * " + an + "_);\n";
    } else {
      c += "f" + an + " -= ";
      c += "(this->dp" + fid + ") * (n" + fid + " - (this->" + Dn + ") * " +
           Psin + " * " + an + "_);\n";
    }
    if (b) {
      c += "const auto dJ" + an + "_d" + an + " = ";
      c += "3 * (this->" + Dn + ") * deviator(" + an + "_) / ";
      c += "(2 * " + DJan + ");\n";
      if (!this->bp.empty()) {
        c += "df" + an + "_ddp" + fid + " = - (";
        c += "n" + fid + "- (this->" + Dn + ") * " + Phin + " * " + Psin +
             " * " + an + "_);\n";
        c += "df" + an + "_ddp" + fid + " += ";
        c += "(this->dp" + fid + ") *";
        c += "(this->" + Dn + ") * " + dPhin + " * " + Psin + " * " + an +
             "_);\n";
      } else {
        c += "df" + an + "_ddp" + fid + " = -(";
        c +=
            "n" + fid + "- (this->" + Dn + ") * " + Psin + " * " + an + "_);\n";
      }
      // opposite of dfa_ds
      const auto mdf_ds = "(this->dp" + fid + ") * dn" + fid + "_ds" + fid;
      c += sp.generateImplicitEquationDerivatives(
          bd, "StrainStensor", an, "-" + mdf_ds, fc.isNormalDeviatoric());
      auto kid2 = decltype(khrs.size()){};
      for (const auto& khr : khrs) {
        c += khr->generateImplicitEquationDerivatives(an, mdf_ds, fid,
                                                      std::to_string(kid2));
        ++kid2;
      }
      c += "df" + an + "_dd" + an + " += ";
      if (!this->bp.empty()) {
        c += Phin + " * ";
      }
      c += "(this->" + Dn + ") * (this->theta) * (this->dp" + fid + ") * ";
      c += "(" + Psin + " * Stensor4::Id() + " + dPsin + " * (" + an + "_^dJ" +
           an + "_d" + an + "));\n";
    }
    c += "} else {\n";
    // simple Prager hardening rule
    c += "f" + an + " -= ";
    c += "(this->dp" + fid + ") * n" + fid + ";\n";
    if (b) {
      // opposite of dfa_ds
      const auto mdf_ds = "(this->dp" + fid + ")*dn" + fid + "_ds" + fid;
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
    c += "}\n";
    return c;
  }  // end of buildBackStrainImplicitEquations

  Chaboche2012KinematicHardeningRule::~Chaboche2012KinematicHardeningRule() =
      default;

}  // end of namespace mfront::bbrick
