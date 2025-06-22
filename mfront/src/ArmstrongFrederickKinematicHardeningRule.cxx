/*!
 * \file   ArmstrongFrederickKinematicHardeningRule.cxx
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
#include "MFront/BehaviourBrick/ArmstrongFrederickKinematicHardeningRule.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription>
  ArmstrongFrederickKinematicHardeningRule::getOptions() const {
    auto opts = KinematicHardeningRuleBase::getOptions();
    opts.emplace_back("D", "back-strain callback coefficient",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions()

  void ArmstrongFrederickKinematicHardeningRule::initialize(
      BehaviourDescription& bd,
      AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& kid,
      const DataMap& d) {
    KinematicHardeningRuleBase::initialize(bd, dsl, fid, kid, d);
    const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
    // kinematic moduli
    tfel::raise_if(d.count("D") == 0,
                   "ArmstrongFrederickKinematicHardeningRule::initialize: "
                   "material property 'D' is not defined");
    this->D = getBehaviourDescriptionMaterialProperty(dsl, "D", d.at("D"));
    declareParameterOrLocalVariable(bd, this->D, "real", Dn);
  }  // end of initialize

  void ArmstrongFrederickKinematicHardeningRule::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& kid) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    KinematicHardeningRuleBase::endTreatment(bd, dsl, fid, kid);
    const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
    const auto c =
        generateMaterialPropertyInitializationCode(dsl, bd, Dn, this->D);
    if (!c.empty()) {
      CodeBlock i;
      i.code = c;
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  std::string
  ArmstrongFrederickKinematicHardeningRule::buildBackStrainImplicitEquations(
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
    auto c = std::string{};
    c = "f" + an + " -= ";
    c += "(this->dp" + fid + ")*(n" + fid + "-(this->" + Dn + ")*" + an +
         "_);\n";
    if (b) {
      c += "df" + an + "_ddp" + fid + " = -(n" + fid + "-(this->" + Dn + ")*" +
           an + "_);\n";
      // opposite of the derivative of fa with respect to s
      const auto mdf_ds = "(this->dp" + fid + ")*dn" + fid + "_ds" + fid;
      c += sp.generateImplicitEquationDerivatives(
          bd, "StrainStensor", an, "-" + mdf_ds, fc.isNormalDeviatoric());
      // term specific to this back strain
      c += "df" + an + "_dd" + an + " += ";
      c += "(this->theta)*(this->dp" + fid + ")*((this->" + Dn +
           ")*Stensor4::Id());\n";
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

  ArmstrongFrederickKinematicHardeningRule::
      ~ArmstrongFrederickKinematicHardeningRule() = default;

}  // end of namespace mfront::bbrick
