/*!
 * \file   mfront/src/Drucker1949StressCriterion.cxx
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
#include "MFront/BehaviourBrick/Drucker1949StressCriterion.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription> Drucker1949StressCriterion::getOptions()
      const {
    auto opts = StressCriterionBase::getOptions();
    opts.emplace_back("c", "", OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  std::vector<mfront::BehaviourSymmetryType>
  Drucker1949StressCriterion::getSupportedBehaviourSymmetries() const {
    return {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
  }  // end of getSupportedBehaviourSymmetries

  void Drucker1949StressCriterion::initialize(BehaviourDescription& bd,
                                              AbstractBehaviourDSL& dsl,
                                              const std::string& id,
                                              const DataMap& d,
                                              const Role r) {
    StressCriterionBase::initialize(bd, dsl, id, d, r);
    const auto cn = StressCriterion::getVariableId("c", id, r);
    tfel::raise_if(d.count("c") == 0,
                   "Drucker1949StressCriterion::initialize: "
                   "material property 'c' is not defined");
    bd.appendToIncludes(
        "#include\"TFEL/Material/Drucker1949YieldCriterion.hxx\"");
    this->cp = getBehaviourDescriptionMaterialProperty(dsl, "c", d.at("c"));
    declareParameterOrLocalVariable(bd, this->cp, "real", cn);
  }  // end of initialize

  void Drucker1949StressCriterion::endTreatment(BehaviourDescription& bd,
                                                const AbstractBehaviourDSL& dsl,
                                                const std::string& id,
                                                const Role r) {
    constexpr auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto cn = StressCriterion::getVariableId("c", id, r);
    auto c = generateMaterialPropertyInitializationCode(dsl, bd, cn, this->cp);
    if (!c.empty()) {
      CodeBlock i;
      i.code = c;
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  std::string Drucker1949StressCriterion::computeElasticPrediction(
      const std::string& id,
      const BehaviourDescription&,
      const StressPotential&) const {
    const auto cn = StressCriterion::getVariableId(
        "c", id, StressCriterion::STRESSCRITERION);
    return "const auto seqel" + id +
           " = computeDrucker1949StressCriterion(sel" + id + ",this->" + cn +
           ");\n";
  }  // end of computeElasticPrediction

  std::string Drucker1949StressCriterion::computeCriterion(
      const std::string& id,
      const BehaviourDescription&,
      const StressPotential&) const {
    const auto cn = StressCriterion::getVariableId(
        "c", id, StressCriterion::STRESSCRITERION);
    return "const auto seq" + id + " = computeDrucker1949StressCriterion(s" +
           id + ",this->" + cn + ");\n";
  }  // end of computeCriterion

  std::string Drucker1949StressCriterion::computeNormal(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    const auto cn = StressCriterion::getVariableId("c", id, r);
    auto c = std::string{};
    if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
      c += "const auto [seq" + id + ",dseq" + id + "_ds" + id + "] = ";
      c += "computeDrucker1949StressCriterionNormal(s" + id + ", this->" + cn +
           "," + sp.getEquivalentStressLowerBound(bd) + ");\n";
    }
    if (r == STRESSANDFLOWCRITERION) {
      c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
    }
    if (r == FLOWCRITERION) {
      c += "const auto [seqf" + id + ", n" + id + "] = ";
      c += "computeDrucker1949StressCriterionNormal(s" + id + ", this->" + cn +
           "," + sp.getEquivalentStressLowerBound(bd) + ");\n";
    }
    return c;
  }  // end of computeNormal

  std::string Drucker1949StressCriterion::computeNormalDerivative(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    const auto cn = StressCriterion::getVariableId("c", id, r);
    auto c = std::string{};
    if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
      c += "const auto [seq" + id + ",dseq" + id + "_ds" + id + ",d2seq" + id +
           "_ds" + id + "ds" + id + "] = ";
      c += "computeDrucker1949StressCriterionSecondDerivative(s" + id +
           ", this->" + cn + "," + sp.getEquivalentStressLowerBound(bd) +
           ");\n";
    }
    if (r == STRESSANDFLOWCRITERION) {
      c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
      c += "const auto& dn" + id + "_ds" + id + " = ";
      c += "d2seq" + id + "_ds" + id + "ds" + id + ";\n";
    }
    if (r == FLOWCRITERION) {
      c += "const auto [seqf" + id + ", n" + id + ", dn" + id + "_ds" + id +
           "] = ";
      c += "computeDrucker1949StressCriterionSecondDerivative(s" + id +
           ",this->" + cn + "," + sp.getEquivalentStressLowerBound(bd) + ");\n";
    }
    return c;
  }  // end of computeNormalDerivative

  bool Drucker1949StressCriterion::isCoupledWithPorosityEvolution() const {
    return false;
  }  // end of isCoupledWithPorosityEvolution

  bool Drucker1949StressCriterion::isNormalDeviatoric() const {
    return true;
  }  // end of isNormalDeviatoric

  StressCriterion::PorosityEffectOnFlowRule
  Drucker1949StressCriterion::getPorosityEffectOnEquivalentPlasticStrain()
      const {
    return StressCriterion::NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN;
  }  // end of getPorosityEffectOnEquivalentPlasticStrain

  Drucker1949StressCriterion::~Drucker1949StressCriterion() = default;

}  // end of namespace mfront::bbrick
