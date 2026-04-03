/*!
 * \file   mfront/src/StandardPorousStressCriterionBase.cxx
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
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StandardPorousStressCriterionBase.hxx"

namespace mfront::bbrick {

  StandardPorousStressCriterionBase::StandardPorousStressCriterionBase(
      const std::string& n)
      : name(n) {}  // end of StandardPorousStressCriterionBase

  void StandardPorousStressCriterionBase::initialize(BehaviourDescription& bd,
                                                     AbstractBehaviourDSL& dsl,
                                                     const std::string& id,
                                                     const DataMap& d,
                                                     const Role r) {
    StressCriterionBase::initialize(bd, dsl, id, d, r);
    const auto mhs = this->getSupportedBehaviourSymmetries();
    if (std::find(mhs.begin(), mhs.end(), mfront::ISOTROPIC) == mhs.end()) {
      tfel::raise(
          "StandardPorousStressCriterionBase::initialize: "
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
        this->initializeMissingMaterialProperty(mp.name);
      }
      auto mpd =
          getBehaviourDescriptionMaterialProperty(dsl, mp.name, d.at(mp.name));
      declareParameterOrLocalVariable(bd, mpd, "real", n);
      this->mps.insert({mp.name, mpd});
    }
    addLocalVariable(bd,
                     this->name + "StressCriterionParameters<StressStensor>",
                     params, 1u);
  }  // end of initialize

  void StandardPorousStressCriterionBase::initializeMissingMaterialProperty(
      const std::string& n) {
    tfel::raise(
        "StandardPorousStressCriterionBase::initializeMissingMaterialProperty: "
        "material property '" +
        n + "' is not defined");
  }  // end of initializeMissingMaterialProperty

  bool StandardPorousStressCriterionBase::isCoupledWithPorosityEvolution()
      const {
    return true;
  }  // end of isCoupledWithPorosityEvolution

  bool StandardPorousStressCriterionBase::isNormalDeviatoric() const {
    return false;
  }  // end of isNormalDeviatoric

  void StandardPorousStressCriterionBase::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& id,
      const Role r) {
    constexpr auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    StressCriterionBase::endTreatment(bd, dsl, id, r);
    const auto params =
        StressCriterion::getVariableId("sscb_parameters", id, r);
    auto c = std::string{};
    for (const auto& mp : this->getOptions()) {
      if (mp.type != OptionDescription::MATERIALPROPERTY) {
        break;
      }
      if (this->mps.count(mp.name) != 0) {
        const auto n = StressCriterion::getVariableId(mp.name, id, r);
        c += generateMaterialPropertyInitializationCode(dsl, bd, n,
                                                        this->mps.at(mp.name));
        c += params + "." + mp.name + " = this->" + n + ";\n";
      }
    }
    for (const auto& mp : this->getOptions()) {
      if (mp.type != OptionDescription::MATERIALPROPERTY) {
        break;
      }
      if (this->mps.count(mp.name) == 0) {
        c += this->generateMissingMaterialPropertyInitializationCode(
            bd, dsl, id, r, mp.name);
      }
    }
    if (!c.empty()) {
      CodeBlock i;
      i.code = c;
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  std::string StandardPorousStressCriterionBase::
      generateMissingMaterialPropertyInitializationCode(
          BehaviourDescription&,
          const AbstractBehaviourDSL&,
          const std::string&,
          const Role,
          const std::string& n) {
    tfel::raise(
        "StandardPorousStressCriterionBase::"
        "generateMissingMaterialPropertyInitializationCode: "
        "material property '" +
        n + "' handled");
  }  // end of generateMissingMaterialPropertyInitializationCode

  std::string StandardPorousStressCriterionBase::computeElasticPrediction(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp) const {
    constexpr auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto params = StressCriterion::getVariableId(
        "sscb_parameters", id, StressCriterion::STRESSCRITERION);
    const auto& f =
        bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
            tfel::glossary::Glossary::Porosity);
    return "const auto seqel" + id + " = compute" + this->name + "Stress(sel" +
           id + ", this->" + f.name + ", this->" + params + ", " +
           sp.getEquivalentStressLowerBound(bd) + ");\n";
  }  // end of computeElasticPrediction

  std::string StandardPorousStressCriterionBase::computeCriterion(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp) const {
    const auto params = StressCriterion::getVariableId(
        "sscb_parameters", id, StressCriterion::STRESSCRITERION);
    constexpr auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto& f =
        bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
            tfel::glossary::Glossary::Porosity);
    return "const auto seq" + id + " = compute" + this->name + "Stress(s" + id +
           ", " + f.name + "_ ,this->" + params + "," +
           sp.getEquivalentStressLowerBound(bd) + ");\n";
  }  // end of computeCriterion

  std::string StandardPorousStressCriterionBase::computeNormal(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    constexpr auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto params =
        StressCriterion::getVariableId("sscb_parameters", id, r);
    const auto& f =
        bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
            tfel::glossary::Glossary::Porosity);
    const auto n = "n" + id;
    const auto s = "s" + id;
    const auto seq = "seq" + id;
    const auto seqf = "seqf" + id;
    const auto f_ = f.name + "_";
    auto c = std::string{};
    if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
      c += "const auto [" + seq + ",d" + seq + "_d" + s + "] = ";
      c += "compute" + this->name + "StressNormal(" + s + ", " + f_ +
           ", this->" + params + ", " + sp.getEquivalentStressLowerBound(bd) +
           ");\n";
    }
    if (r == STRESSANDFLOWCRITERION) {
      c += "const auto& " + n + " = d" + seq + "_d" + s + ";\n";
    }
    if (r == FLOWCRITERION) {
      c += "const auto [" + seqf +  //
           ", " + n + ", " +        //
           ", d" + seqf + "_d" + f.name + "] = ";
      c += "compute" + this->name + "StressNormal(" + s + ", " + f_ +
           ", this->" + params + ", " + sp.getEquivalentStressLowerBound(bd) +
           ");\n";
    }
    return c;
  }  // end of computeNormal

  std::string StandardPorousStressCriterionBase::computeNormalDerivative(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    constexpr auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto params =
        StressCriterion::getVariableId("sscb_parameters", id, r);
    const auto& f =
        bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
            tfel::glossary::Glossary::Porosity);
    const auto n = "n" + id;
    const auto s = "s" + id;
    const auto seq = "seq" + id;
    const auto seqf = "seqf" + id;
    const auto f_ = f.name + "_";
    auto c = std::string{};
    if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
      c += "const auto [" + seq +              //
           ",d" + seq + "_d" + s +             //
           ",d" + seq + "_d" + f.name +        //
           ",d2" + seq + "_d" + s + "d" + s +  //
           ",d2" + seq + "_d" + s + "d" + f.name + "] = ";
      c += "compute" + this->name + "StressSecondDerivative(" + s + ", " + f_ +
           ", this->" + params + ", " + sp.getEquivalentStressLowerBound(bd) +
           ");\n";
    }
    if (r == STRESSANDFLOWCRITERION) {
      c += "const auto& " + n + " = d" + seq + "_d" + s + ";\n";
      c += "const auto& d" + n + "_d" + s + " = ";
      c += "d2" + seq + "_d" + s + "d" + s + ";\n";
      c += "const auto& d" + n + "_d" + f.name + " = ";
      c += "d2" + seq + "_d" + s + "d" + f.name + ";\n";
    }
    if (r == FLOWCRITERION) {
      c += "const auto [" + seqf +  //
           ", n" + id +             //
           ", dseqf_d" + f.name +   //
           ", d" + n + "_d" + s +   //
           ", d" + n + "_d" + f.name + "] = ";
      c += "compute" + this->name + "StressSecondDerivative(" + s + ", " + f_ +
           ", this->" + params + ", " + sp.getEquivalentStressLowerBound(bd) +
           ");\n";
    }
    return c;
  }  // end of computeNormalDerivative

  StandardPorousStressCriterionBase::~StandardPorousStressCriterionBase() =
      default;

}  // end of namespace mfront::bbrick
