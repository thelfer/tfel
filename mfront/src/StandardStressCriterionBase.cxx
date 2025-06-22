/*!
 * \file   mfront/src/StandardStressCriterionBase.cxx
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
#include "MFront/BehaviourBrick/StandardStressCriterionBase.hxx"

namespace mfront {

  namespace bbrick {

    StandardStressCriterionBase::StandardStressCriterionBase(
        const std::string& n)
        : name(n) {
    }  // end of StandardStressCriterionBase::StandardStressCriterionBase

    void StandardStressCriterionBase::initialize(BehaviourDescription& bd,
                                                 AbstractBehaviourDSL& dsl,
                                                 const std::string& id,
                                                 const DataMap& d,
                                                 const Role r) {
      StressCriterionBase::initialize(bd, dsl, id, d, r);
      const auto mhs = this->getSupportedBehaviourSymmetries();
      if (std::find(mhs.begin(), mhs.end(), mfront::ISOTROPIC) == mhs.end()) {
        tfel::raise(
            "StandardStressCriterionBase::initialize: "
            "expected an isotropic criterion");
      }
      bd.appendToIncludes("#include \"TFEL/Material/" + name +
                          "StressCriterion.hxx\"\n");
      const auto params =
          StressCriterion::getVariableId("sscb_parameters", id, r);
      for (const auto& mp : this->getOptions()) {
        if (mp.type != OptionDescription::MATERIALPROPERTY) {
          break;
        }
        const auto n = StressCriterion::getVariableId(mp.name, id, r);
        if (d.count(mp.name) == 0) {
          tfel::raise(
              "StandardStressCriterionBase::initialize: "
              "material property '" +
              mp.name + "' is not defined");
        }
        auto mpd = getBehaviourDescriptionMaterialProperty(dsl, mp.name,
                                                           d.at(mp.name));
        declareParameterOrLocalVariable(bd, mpd, "real", n);
        this->mps.insert({mp.name, mpd});
      }
      addLocalVariable(bd,
                       this->name + "StressCriterionParameters<StressStensor>",
                       params, 1u);
    }  // end of StandardStressCriterionBase::initialize

    bool StandardStressCriterionBase::isCoupledWithPorosityEvolution() const {
      return false;
    }  // end of StandardStressCriterionBase::isCoupledWithPorosityEvolution()

    void StandardStressCriterionBase::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const std::string& id,
        const Role r) {
      constexpr const auto uh =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      StressCriterionBase::endTreatment(bd, dsl, id, r);
      const auto params =
          StressCriterion::getVariableId("sscb_parameters", id, r);
      auto c = std::string{};
      for (const auto& mp : this->getOptions()) {
        if (mp.type != OptionDescription::MATERIALPROPERTY) {
          break;
        }
        const auto n = StressCriterion::getVariableId(mp.name, id, r);
        c += generateMaterialPropertyInitializationCode(dsl, bd, n,
                                                        this->mps.at(mp.name));
        c += params + "." + mp.name + " = this->" + n + ";\n";
      }
      if (!c.empty()) {
        CodeBlock i;
        i.code = c;
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of StandardStressCriterionBase::endTreatment

    std::string StandardStressCriterionBase::computeElasticPrediction(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp) const {
      const auto params = StressCriterion::getVariableId(
          "sscb_parameters", id, StressCriterion::STRESSCRITERION);
      return "const auto seqel" + id + " = compute" + this->name +
             "Stress(sel" + id + ",this->" + params + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
    }  // end of StandardStressCriterionBase::computeElasticPrediction

    std::string StandardStressCriterionBase::computeCriterion(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp) const {
      const auto params = StressCriterion::getVariableId(
          "sscb_parameters", id, StressCriterion::STRESSCRITERION);
      return "const auto seq" + id + " = compute" + this->name + "Stress(s" +
             id + ",this->" + params + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
    }  // end of StandardStressCriterionBase::computeCriterion

    std::string StandardStressCriterionBase::computeNormal(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const Role r) const {
      const auto params =
          StressCriterion::getVariableId("sscb_parameters", id, r);
      auto c = std::string{};
      if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
#if __cplusplus >= 201703L
        c += "const auto [seq" + id + ",dseq" + id + "_ds" + id + "] = ";
        c += "compute" + this->name + "StressNormal(s" + id + ", this->" +
             params + "," + sp.getEquivalentStressLowerBound(bd) + ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seq" + id + ";\n";
        c += "Stensor dseq" + id + "_ds" + id + ";\n";
        c += "std::tie(seq" + id + ",dseq" + id + "_ds" + id + ") = ";
        c += "compute" + this->name + "StressNormal(s" + id + ",this->" +
             params + "," + sp.getEquivalentStressLowerBound(bd) + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      if (r == STRESSANDFLOWCRITERION) {
        c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
      }
      if (r == FLOWCRITERION) {
#if __cplusplus >= 201703L
        c += "const auto [seqf" + id + ", n" + id + "] = ";
        c += "compute" + this->name + "StressNormal(s" + id + ",this->" +
             params + "," + sp.getEquivalentStressLowerBound(bd) + ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seqf" + id + ";\n";
        c += "Stensor n" + id + ";\n";
        c += "std::tie(seqf" + id + ",n" + id + ") = ";
        c += "compute" + this->name + "StressNormal(s" + id + ",this->" +
             params + "," + sp.getEquivalentStressLowerBound(bd) + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      return c;
    }  // end of StandardStressCriterionBase::computeNormal

    std::string StandardStressCriterionBase::computeNormalDerivative(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const Role r) const {
      const auto params =
          StressCriterion::getVariableId("sscb_parameters", id, r);
      auto c = std::string{};
      if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
#if __cplusplus >= 201703L
        c += "const auto [seq" + id + ",dseq" + id + "_ds" + id + ",d2seq" +
             id + "_ds" + id + "ds" + id + "] = ";
        c += "compute" + this->name + "StressSecondDerivative(s" + id +
             ",this->" + params + "," + sp.getEquivalentStressLowerBound(bd) +
             ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seq" + id + ";\n";
        c += "Stensor dseq" + id + "_ds" + id + ";\n";
        c += "Stensor4 d2seq" + id + "_ds" + id + "ds" + id + ";\n";
        c += "std::tie(seq" + id + ",dseq" + id + "_ds" + id + ",d2seq" + id +
             "_ds" + id + "ds" + id + ") = ";
        c += "compute" + this->name + "StressSecondDerivative(s" + id +
             ",this->" + params + "," + sp.getEquivalentStressLowerBound(bd) +
             ");\n";
#endif /* __cplusplus >= 201703L */
      }
      if (r == STRESSANDFLOWCRITERION) {
        c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
        c += "const auto& dn" + id + "_ds" + id + " = ";
        c += "d2seq" + id + "_ds" + id + "ds" + id + ";\n";
      }
      if (r == FLOWCRITERION) {
#if __cplusplus >= 201703L
        c += "const auto [seqf" + id + ", n" + id + ", dn" + id + "_ds" + id +
             "] = ";
        c += "compute" + this->name + "StressSecondDerivative(s" + id +
             ",this->" + params + "," + sp.getEquivalentStressLowerBound(bd) +
             ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seqf" + id + ";\n";
        c += "Stensor n" + id + ";\n";
        c += "Stensor4 dn" + id + "_ds" + id + ";\n";
        c +=
            "std::tie(seqf" + id + ",n" + id + ",dn" + id + "_ds" + id + ") = ";
        c += "compute" + this->name + "StressSecondDerivative(s" + id +
             ",this->" + params + "," + sp.getEquivalentStressLowerBound(bd) +
             ");\n";
#endif /* __cplusplus >= 201703L */
      }
      return c;
    }  // end of StandardStressCriterionBase::computeNormalDerivative

    StressCriterion::PorosityEffectOnFlowRule
    StandardStressCriterionBase::getPorosityEffectOnEquivalentPlasticStrain()
        const {
      return StressCriterion::NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN;
    }  // end of
       // StandardStressCriterionBase::getPorosityEffectOnEquivalentPlasticStrain()

    StandardStressCriterionBase::~StandardStressCriterionBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
