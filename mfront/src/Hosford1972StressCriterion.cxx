/*!
 * \file   mfront/src/Hosford1972StressCriterion.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/Hosford1972StressCriterion.hxx"

namespace mfront::bbrick {

  std::vector<mfront::BehaviourSymmetryType>
  Hosford1972StressCriterion::getSupportedBehaviourSymmetries() const {
    return {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
  }  // end of Hosford1972StressCriterion::getSupportedBehaviourSymmetries()

  std::vector<OptionDescription> Hosford1972StressCriterion::getOptions()
      const {
    auto opts = StressCriterionBase::getOptions();
    opts.emplace_back("a", "Hosford exponent",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back(
        "eigen_solver",
        "Choice of the eigen solver. Valid value are 'default' and 'Jacoby'.",
        OptionDescription::STRING);
    return opts;
  }  // end of Hosford1972StressCriterion::getOptions()

  void Hosford1972StressCriterion::initialize(BehaviourDescription& bd,
                                              AbstractBehaviourDSL& dsl,
                                              const std::string& id,
                                              const DataMap& d,
                                              const Role r) {
    StressCriterionBase::initialize(bd, dsl, id, d, r);
    const auto an = StressCriterion::getVariableId("a", id, r);
    tfel::raise_if(d.count("a") == 0,
                   "Hosford1972StressCriterion::initialize: "
                   "material property 'a' is not defined");
    if (d.count("eigen_solver") != 0) {
      this->eigen_solver =
          handleEigenSolverOption(d.at("eigen_solver").get<std::string>());
    }
    this->a = getBehaviourDescriptionMaterialProperty(dsl, "a", d.at("a"));
    declareParameterOrLocalVariable(bd, this->a, "real", an);
  }  // end of Hosford1972StressCriterion::initialize

  void Hosford1972StressCriterion::endTreatment(BehaviourDescription& bd,
                                                const AbstractBehaviourDSL& dsl,
                                                const std::string& id,
                                                const Role r) {
    constexpr auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto an = StressCriterion::getVariableId("a", id, r);
    auto c = generateMaterialPropertyInitializationCode(dsl, bd, an, this->a);
    if (!c.empty()) {
      CodeBlock i;
      i.code = c;
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of Hosford1972StressCriterion::endTreatment

  std::string Hosford1972StressCriterion::computeElasticPrediction(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp) const {
    const auto an = StressCriterion::getVariableId(
        "a", id, StressCriterion::STRESSCRITERION);
    return "const auto seqel" + id + " = computeHosfordStress" +
           this->getTemplateParameters() + "(sel" + id + ",this->" + an + "," +
           sp.getEquivalentStressLowerBound(bd) + ");\n";
  }  // end of Hosford1972StressCriterion::computeElasticPrediction

  std::string Hosford1972StressCriterion::computeCriterion(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp) const {
    const auto an = StressCriterion::getVariableId(
        "a", id, StressCriterion::STRESSCRITERION);
    return "const auto seq" + id + " = computeHosfordStress" +
           this->getTemplateParameters() + "(s" + id + ",this->" + an + "," +
           sp.getEquivalentStressLowerBound(bd) + ");\n";
  }  // end of Hosford1972StressCriterion::computeCriterion

  std::string Hosford1972StressCriterion::computeNormal(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    const auto an = StressCriterion::getVariableId("a", id, r);
    auto c = std::string{};
    if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
      c += "const auto [seq" + id + ",dseq" + id + "_ds" + id + "] = ";
      c += "computeHosfordStressNormal" + this->getTemplateParameters() + "(s" +
           id + ", this->" + an + "," + sp.getEquivalentStressLowerBound(bd) +
           ");\n";
    }
    if (r == STRESSANDFLOWCRITERION) {
      c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
    }
    if (r == FLOWCRITERION) {
      c += "const auto [seqf" + id + ", n" + id + "] = ";
      c += "computeHosfordStressNormal" + this->getTemplateParameters() + "(s" +
           id + ",this->" + an + "," + sp.getEquivalentStressLowerBound(bd) +
           ");\n";
    }
    return c;
  }  // end of Hosford1972StressCriterion::computeNormal

  std::string Hosford1972StressCriterion::computeNormalDerivative(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    const auto an = StressCriterion::getVariableId("a", id, r);
    auto c = std::string{};
    if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
      c += "const auto [seq" + id + ",dseq" + id + "_ds" + id + ",d2seq" + id +
           "_ds" + id + "ds" + id + "] = ";
      c += "computeHosfordStressSecondDerivative" +
           this->getTemplateParameters() + "(s" + id + ",this->" + an + "," +
           sp.getEquivalentStressLowerBound(bd) + ");\n";
    }
    if (r == STRESSANDFLOWCRITERION) {
      c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
      c += "const auto& dn" + id + "_ds" + id + " = ";
      c += "d2seq" + id + "_ds" + id + "ds" + id + ";\n";
    }
    if (r == FLOWCRITERION) {
      c += "const auto ";
      c += "[seqf" + id + ", n" + id + ", dn" + id + "_ds" + id + "] = ";
      c += "computeHosfordStressSecondDerivative" +
           this->getTemplateParameters() + "(s" + id + ",this->" + an + "," +
           sp.getEquivalentStressLowerBound(bd) + ");\n";
    }
    return c;
  }  // end of Hosford1972StressCriterion::computeNormalDerivative

  std::string Hosford1972StressCriterion::getTemplateParameters() const {
    if (this->eigen_solver.empty()) {
      return "";
    }
    return "<StressStensor, real, " + this->eigen_solver + ">";
  }  // end of Hosford1972StressCriterion::getTemplateParameters

  bool Hosford1972StressCriterion::isCoupledWithPorosityEvolution() const {
    return false;
  }  // end of Hosford1972StressCriterion::isCoupledWithPorosityEvolution

  bool Hosford1972StressCriterion::isNormalDeviatoric() const {
    return true;
  }  // end of Hosford1972StressCriterion::isNormalDeviatoric

  StressCriterion::PorosityEffectOnFlowRule
  Hosford1972StressCriterion::getPorosityEffectOnEquivalentPlasticStrain()
      const {
    return StressCriterion::NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN;
  }  // end of
     // Hosford1972StressCriterion::getPorosityEffectOnEquivalentPlasticStrain()

  Hosford1972StressCriterion::~Hosford1972StressCriterion() = default;

}  // end of namespace mfront::bbrick
