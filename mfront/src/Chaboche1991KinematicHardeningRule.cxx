/*!
 * \file   Chaboche1991KinematicHardeningRule.cxx
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
#include "MFront/BehaviourBrick/Chaboche1991KinematicHardeningRule.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription> Chaboche1991KinematicHardeningRule::getOptions()
        const {
      auto opts = KinematicHardeningRuleBase::getOptions();
      opts.emplace_back("D", "back-strain callback coefficient",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("m1", "", OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("m2", "", OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("w", "", OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of Chaboche1991KinematicHardeningRule::getOptions()

    void Chaboche1991KinematicHardeningRule::initialize(
        BehaviourDescription& bd,
        AbstractBehaviourDSL& dsl,
        const std::string& fid,
        const std::string& kid,
        const DataMap& d) {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      // `this` must be captured in gcc 4.7.2
      auto get_mp = [&bd, &dsl, &d, &fid, &kid,this](
          BehaviourDescription::MaterialProperty& mp, const std::string& n) {
        const auto nid = KinematicHardeningRule::getVariableId(n, fid, kid);
        if (d.count(n) == 0) {
          tfel::raise(
              "Chaboche1991KinematicHardeningRule::initialize: "
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
      // reserved named
      const auto DJan = KinematicHardeningRule::getVariableId("DJa", fid, kid);
      const auto iDJan = KinematicHardeningRule::getVariableId("iDJa", fid, kid);
      const auto dDJan = KinematicHardeningRule::getVariableId("dDJa", fid, kid);
      const auto Phin = KinematicHardeningRule::getVariableId("Phi", fid, kid);
      const auto dPhin =
          KinematicHardeningRule::getVariableId("dPhi", fid, kid) + "_d" +
          KinematicHardeningRule::getVariableId("DJa", fid, kid);
      bd.reserveName(uh, Phin);
      bd.reserveName(uh, dPhin);
      bd.reserveName(uh, DJan);
      bd.reserveName(uh, iDJan);
      bd.reserveName(uh, dDJan);
      bd.reserveName(uh, "r" + DJan + "_m1_1");
      bd.reserveName(uh, "r" + DJan + "_m1");
      bd.reserveName(uh, DJan + "_m2_1");
      bd.reserveName(uh, DJan + "_m2");
    }  // end of Chaboche1991KinematicHardeningRule::initialize

    void Chaboche1991KinematicHardeningRule::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const std::string& fid,
        const std::string& kid) const {
      KinematicHardeningRuleBase::endTreatment(bd, dsl, fid, kid);
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
      const auto m1n = KinematicHardeningRule::getVariableId("m1", fid, kid);
      const auto m2n = KinematicHardeningRule::getVariableId("m2", fid, kid);
      const auto wn = KinematicHardeningRule::getVariableId("w", fid, kid);
      auto c = generateMaterialPropertyInitializationCode(dsl, bd, Dn, this->D);
      c += generateMaterialPropertyInitializationCode(dsl, bd, m1n, this->m1);
      c += generateMaterialPropertyInitializationCode(dsl, bd, m2n, this->m2);
      c += generateMaterialPropertyInitializationCode(dsl, bd, wn, this->w);
      if (!c.empty()) {
        CodeBlock i;
        i.code = c;
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of Chaboche1991KinematicHardeningRule::endTreatment

    std::string
    Chaboche1991KinematicHardeningRule::buildBackStrainImplicitEquations(
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
      const auto m1n = KinematicHardeningRule::getVariableId("m1", fid, kid);
      const auto m2n = KinematicHardeningRule::getVariableId("m2", fid, kid);
      const auto wn = KinematicHardeningRule::getVariableId("w", fid, kid);
      const auto DJan = KinematicHardeningRule::getVariableId("DJa", fid, kid);
      const auto iDJan = KinematicHardeningRule::getVariableId("iDJa", fid, kid);
      const auto dDJan = KinematicHardeningRule::getVariableId("dDJa", fid, kid);
      const auto Phin = KinematicHardeningRule::getVariableId("Phi", fid, kid);
      const auto dPhin =
          KinematicHardeningRule::getVariableId("dPhi", fid, kid) + "_d" +
          KinematicHardeningRule::getVariableId("DJa", fid, kid);
      const auto n = "n" + fid;
      auto c = std::string{};
      c += "const auto " + DJan + " = ";
      c += "(this->" + Dn + ")*sigmaeq(this->" + an + ");\n";
      c += "const auto " + iDJan + " = 1/(std::max("+DJan+",1e-12));\n";
      if (b) {
#if __cplusplus >= 201703L
        c += "const auto [" + Phin + "," + dPhin + "] = ";
#else  /* __cplusplus >= 201703L */
        c += "auto " + Phin + " = real{};\n";
        c += "auto " + dPhin + " = real{};\n";
        c += "std::tie(" + Phin + "," + dPhin + ") = ";
#endif /* __cplusplus >= 201703L */
        c += "[this,&" + DJan + ",&" + iDJan + "]() -> std::pair<real,real>{\n";
        c += "if(" + DJan + "-this->" + wn + "){\n";
        c += "const auto r" + DJan + " = (" + DJan + "-" + wn + ")/(1-" + wn +
             ");\n";
        c += "const auto r" + DJan + "_m1_1 = ";
        c += "pow(r" + DJan + ",this->" + m1n + "-1);\n";
        c += "const auto r" + DJan + "_m1 = ";
        c += "r" + DJan + "_m1_1*r" + DJan + ";\n";
        c += "const auto " + DJan + "_m2_1 = pow(" + DJan + ",-(this->" + m2n +
             ")-1);\n";
        c += "const auto " + DJan + "_m2 = " + DJan + "_m2_1*" + DJan + ";\n";
        c += "return {r" + DJan + "_m1*" + DJan + "_m2,\n";
        c += "(this->" + m1n + ")*r" + DJan + "_m1_1*" + DJan + "_m2-";
        c += "(this->" + m2n + ")*r" + DJan + "_m1*" + DJan + "_m2_1};\n";
        c += "}\n";
        c += "return {real{},real{}};";
        c += "}();\n";
      } else {
        c += "const auto " + Phin + " = [this,&" + DJan + ",&" + iDJan +
             "]() -> real {\n";
        c += "if(" + DJan + "-this->" + wn + "){\n";
        c += "return pow(" + DJan + ",this->" + m1n + ")*";
        c += "pow(" + iDJan + ",-(this->" + m2n + "));\n";
        c += "}\n";
        c += "return 0;";
        c += "}();\n";
      }
      const auto df_ddp =
          "-" + n + "+(this->" + Dn + ")*" + Phin + "*" + an + "_";
      if (b) {
        c += "df" + an + "_ddp" + fid + " = " + df_ddp + ";\n";
      } else {
        c += "const auto df" + an + "_ddp" + fid + " = eval(" + df_ddp + ");\n";
      }
      c += "f" + an + " += ";
      c += "(this->dp" + fid + ")*(df" + an + "_ddp" + fid + ");\n";
      if (b) {
        "const auto " + dDJan + " = 3*(this->" + Dn + ")*(this->" + Dn +
            ")*deviator(" + an + "_)/(+" + DJan + "+);\n";
        //         // opposite of the derivative of fa with respect to s
        //         const auto mdf_ds = "(this->dp" + fid + ")*(d" + n + "_ds" +
        //         fid +
        //                             "-(this->" + Dn + ")*(1-this->" + en +
        //                             ")*((" + an +
        //                             "_|" + n + ")*d" + n + "_ds" + fid + "+("
        //                             + n +
        //                             "^(" + an + "_|d" + n + "_ds" + fid +
        //                             "))))";
        //         c += sp.computeDerivatives(bd, an, "-" + mdf_ds);
        //         // term specific to this back strain
        //         c += "df" + an + "_dd" + an + " += ";
        //         c += "(this->theta)*(this->dp" + fid + ")*(this->" + Dn +
        //         ")*(";
        //         c += "(this->" + en + ")*Stensor4::Id()+";
        //         c += "(1-this->" + en + ")*(n"+fid + "^" + n + "));\n";
        //         // terms in common for all back strains
        //         auto kid2 = decltype(khrs.size()){};
        //         for (const auto khr : khrs) {
        //           c += khr->computeDerivatives(an, mdf_ds, fid,
        //                                        std::to_string(kid2));
        //           ++kid2;
        //         }
      }
      return c;
    }  // end of
    // Chaboche1991KinematicHardeningRule::buildBackStrainImplicitEquations

    Chaboche1991KinematicHardeningRule::~Chaboche1991KinematicHardeningRule() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
