/*!
 * \file   KinematicHardeningRuleBase.cxx
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
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRuleBase.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription> KinematicHardeningRuleBase::getOptions()
      const {
    auto opts = std::vector<OptionDescription>{};
    opts.emplace_back("C", "kinematic moduli",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  void KinematicHardeningRuleBase::initialize(BehaviourDescription& bd,
                                              AbstractBehaviourDSL& dsl,
                                              const std::string& fid,
                                              const std::string& kid,
                                              const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
    const auto Xn = KinematicHardeningRule::getVariableId("X", fid, kid);
    const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
    const auto en =
        KinematicHardeningRule::getVariableId("BackStrain", fid, kid);
    addStateVariable(bd, "StrainStensor", an, en);
    // kinematic moduli
    tfel::raise_if(d.count("C") == 0,
                   "KinematicHardeningRuleBase::initialize: "
                   "material property 'C' is not defined");
    this->C = getBehaviourDescriptionMaterialProperty(dsl, "C", d.at("C"));
    declareParameterOrLocalVariable(bd, this->C, "stress", Cn);
    // kinematic hardening at the beginning of the time step
    bd.reserveName(uh, Xn);
    // kinematic hardening at the middle of the time step
    bd.reserveName(uh, Xn + '_');
    // back strain at the middle of the time step
    bd.reserveName(uh, an + '_');
  }  // end of initialize

  std::string KinematicHardeningRuleBase::getBackStrainVariable(
      const std::string& fid, const std::string& kid) const {
    return KinematicHardeningRule::getVariableId("a", fid, kid);
  }  // end of getBackStrainVariable

  std::string KinematicHardeningRuleBase::getBackStressDerivative(
      const std::string& fid, const std::string& kid) const {
    const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
    return "((2 * (this->theta) * ((this->" + Cn + ")/3)) * (Stensor4::Id()))";
  }  // end of getBackStressDerivative

  std::string KinematicHardeningRuleBase::generateImplicitEquationDerivatives(
      const std::string& v,
      const std::string& mdfv_ds,
      const std::string& fid,
      const std::string& kid) const {
    const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
    const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
    auto c = std::string{};
    c += "df" + v + "_dd" + an + " += ";
    c += "(2*(this->theta)*((this->" + Cn + ")/3))*(" + mdfv_ds + ");\n";
    return c;
  }

  void KinematicHardeningRuleBase::endTreatment(BehaviourDescription& bd,
                                                const AbstractBehaviourDSL& dsl,
                                                const std::string& fid,
                                                const std::string& kid) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto mts = getMiddleOfTimeStepModifier(bd);
    if (!this->C.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
      CodeBlock i;
      std::ostringstream mps;
      mps << "this->" + Cn + " = ";
      dsl.writeMaterialPropertyEvaluation(mps, this->C, mts);
      mps << ";\n";
      i.code += mps.str();
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  std::vector<std::string>
  KinematicHardeningRuleBase::getKinematicHardeningsVariables(
      const std::string& fid, const std::string& kid) const {
    return {KinematicHardeningRule::getVariableId("X", fid, kid)};
  }  // end of getKinematicHardeningsVariables

  std::string
  KinematicHardeningRuleBase::computeKinematicHardeningsInitialValues(
      const std::string& fid, const std::string& kid) const {
    const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
    const auto Xn = KinematicHardeningRule::getVariableId("X", fid, kid);
    const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
    return "const auto " + Xn + "  = eval(2*(this->" + Cn + ")*(this->" + an +
           ")/3);\n";
  }  // end of
     // KinematicHardeningRuleBase::computeKinematicHardeningsInitialValues

  std::string KinematicHardeningRuleBase::computeKinematicHardenings(
      const std::string& fid, const std::string& kid) const {
    const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
    const auto Xn = KinematicHardeningRule::getVariableId("X", fid, kid);
    const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
    auto c = std::string{};
    c += "const auto " + an + "_  = eval(this->" + an +
         "+(this->theta)*(this->d" + an + "));\n";
    c +=
        "const auto " + Xn + "_  = eval(2*(this->" + Cn + ")*" + an + "_/3);\n";
    return c;
  }  // end of computeKinematicHardenings

  KinematicHardeningRuleBase::~KinematicHardeningRuleBase() = default;

}  // end of namespace mfront::bbrick
