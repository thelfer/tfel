/*!
 * \file   OhnoWangKinematicHardeningRule.cxx
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
#include "MFront/BehaviourBrick/OhnoWangKinematicHardeningRule.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription> OhnoWangKinematicHardeningRule::getOptions()
        const {
      auto opts = KinematicHardeningRuleBase::getOptions();
      opts.emplace_back("D", "back-strain callback coefficient",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("m1", "Ohno-Wang parameter",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("m2", "Ohno-Wang parameter",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("w", "Ohno-Wang parameter",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of OhnoWangKinematicHardeningRule::getOptions()

    void OhnoWangKinematicHardeningRule::initialize(
        BehaviourDescription& bd,
        AbstractBehaviourDSL& dsl,
        const std::string& fid,
        const std::string& kid,
        const DataMap& d) {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      auto get_mp = [&bd, &dsl, &d, &fid, &kid](
          BehaviourDescription::MaterialProperty& mp, const std::string& n) {
        const auto nid = KinematicHardeningRule::getVariableId(n, fid, kid);
        if (d.count(n) == 0) {
          tfel::raise(
              "OhnoWangKinematicHardeningRule::initialize: "
              "material property '" +
              n + "' is not defined");
        }
        mp = getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
        declareParameterOrLocalVariable(bd, mp, nid);
      };
      KinematicHardeningRuleBase::initialize(bd, dsl, fid, kid, d);
      get_mp(this->D, "D");
      get_mp(this->m1, "m1");
      get_mp(this->m2, "m2");
      get_mp(this->w, "w");
      bd.reserveName(
          uh, KinematicHardeningRule::getVariableId("DXeq", fid, kid) + "_");
      bd.reserveName(uh,
                     KinematicHardeningRule::getVariableId("OWPhi", fid, kid));
      bd.reserveName(
          uh, KinematicHardeningRule::getVariableId("dOWPhi", fid, kid) + "_d" +
                  KinematicHardeningRule::getVariableId("X", fid, kid));
    }  // end of OhnoWangKinematicHardeningRule::initialize

    void OhnoWangKinematicHardeningRule::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const std::string& fid,
        const std::string& kid) const {
      KinematicHardeningRuleBase::endTreatment(bd, dsl, fid, kid);
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
      const auto m2n = KinematicHardeningRule::getVariableId("m1", fid, kid);
      const auto m1n = KinematicHardeningRule::getVariableId("m2", fid, kid);
      const auto wn = KinematicHardeningRule::getVariableId("w", fid, kid);
      auto c = std::string{};
      c += generateMaterialPropertyInitializationCode(dsl, bd, Dn, this->D);
      c += generateMaterialPropertyInitializationCode(dsl, bd, m1n, this->m1);
      c += generateMaterialPropertyInitializationCode(dsl, bd, m2n, this->m2);
      c += generateMaterialPropertyInitializationCode(dsl, bd, wn, this->w);
      if (!c.empty()) {
        CodeBlock i;
        i.code = c;
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of OhnoWangKinematicHardeningRule::endTreatment

    std::string
    OhnoWangKinematicHardeningRule::buildBackStrainImplicitEquations(
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const std::vector<std::shared_ptr<KinematicHardeningRule>>&,
        const std::string& fid,
        const std::string& kid,
        const bool b) const {
      const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
      const auto Xn = KinematicHardeningRule::getVariableId("X", fid, kid)+"_";
      const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
      const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
      const auto m1n = KinematicHardeningRule::getVariableId("m1", fid, kid);
      const auto m2n = KinematicHardeningRule::getVariableId("m2", fid, kid);
      const auto wn = KinematicHardeningRule::getVariableId("w", fid, kid);
      const auto DXeq =
          KinematicHardeningRule::getVariableId("DXeq", fid, kid) + "_";
      const auto Phi = KinematicHardeningRule::getVariableId("OWPhi", fid, kid);
      const auto dPhi =
          KinematicHardeningRule::getVariableId("dOWPhi", fid, kid) + "_d" +
          KinematicHardeningRule::getVariableId("X", fid, kid);
      tfel::raise("OhnoWangKinematicHardeningRule::"
		  "buildBackStrainImplicitEquations: "
		  "unimplemented feature");
      auto c = std::string{};
      c += "const auto " + DXeq + " = (this->" + Dn + ")*sqrt(3*(" + Xn + "|" +
           Xn + ")/2);\n";
      c += "const auto " + Phi + " = pow(std::max(" + DXeq + "-(this->" + wn +
           ")*(this->" + Cn + "),stress(0))/(1-(this->" + wn + ")),this->" +
           m1n + ")/(pow(std::max(" + DXeq + ",1e-12*(this->young)),this->" +
           m2n + "));\n";
      if (b) {
        c += "const auto " + dPhi + " = Stensor::Id();\n";
      }
      c += "f" + an + " -= ";
      c += "(this->dp" + fid + ")*(n" + fid + "-" + Phi + "*(this->" + Dn +
           ")*((this->" + an + ")+(this->theta)*(this->d" + an + ")));\n";
      if (b) {
        c += "df" + an + "_ddp" + fid + " = -(n" + fid + "-(this->" + Dn +
             ")*((this->" + an + ")+(this->theta)*(this->d" + an + ")));\n";
        c += sp.computeDerivatives(
            bd, an, "-(this->dp" + fid + ")*dn" + fid + "_ds" + fid);
        c += "df" + an + "_dd" + an + " += ";
        c += "(this->theta)*(this->dp" + fid + ")*((2*(this->" + Cn +
             ")/3)*dn" + fid + "_ds" + fid + "+(this->" + Dn +
             ")*(Stensor4::Id()+(this->" + Cn + ")*(" + an + "_^(" + dPhi +
             "))));\n";
        }
        return c;
    }  // end of
       // OhnoWangKinematicHardeningRule::buildBackStrainImplicitEquations

    OhnoWangKinematicHardeningRule::~OhnoWangKinematicHardeningRule() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
