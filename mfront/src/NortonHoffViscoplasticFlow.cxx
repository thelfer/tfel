/*!
 * \file   NortonHoffViscoplasticFlow.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressCriterion.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/NortonHoffViscoplasticFlow.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription> NortonHoffViscoplasticFlow::getOptions()
      const {
    auto opts = ViscoplasticFlowBase::getOptions();
    opts.emplace_back("A", "Norton coefficient (optional)",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("K", "Stress normalisation factor",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("Ksf",
                      "stress thresold factor. If the seq-R is greater "
                      "than Ksf*K, the newton step is rejected",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("n", "Norton exponent",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  void NortonHoffViscoplasticFlow::initialize(BehaviourDescription& bd,
                                              AbstractBehaviourDSL& dsl,
                                              const std::string& id,
                                              const DataMap& d) {
    auto get_mp = [&dsl, &bd, &id, &d](const std::string& mpn,
                                       const std::string& t,
                                       const std::string& vn) {
      if (d.count(mpn) == 0) {
        tfel::raise(
            "NortonHoffViscoplasticFlow::"
            "initialize: "
            "material property '" +
            mpn + "' is not defined");
      }
      auto mp = getBehaviourDescriptionMaterialProperty(dsl, mpn, d.at(mpn));
      declareParameterOrLocalVariable(bd, mp, t, vn + id);
      return mp;
    };
    // checking options
    mfront::bbrick::check(d, this->getOptions());
    // the base class
    ViscoplasticFlowBase::initialize(bd, dsl, id, d);
    // Norton flow options
    this->K = get_mp("K", "stress", "K");
    this->n = get_mp("n", "real", "E");
    if (d.count("A") != 0) {
      this->A = get_mp("A", "strainrate", "A");
    } else {
      BehaviourDescription::ConstantMaterialProperty cmp;
      cmp.value = 1;
      this->A = cmp;
      declareParameterOrLocalVariable(bd, this->A, "strainrate", "A" + id);
    }
    if (d.count("Ksf") != 0) {
      this->Ksf = get_mp("Ksf", "real", "Ksf");
    }
  }  // end of initialize

  void NortonHoffViscoplasticFlow::endTreatment(BehaviourDescription& bd,
                                                const AbstractBehaviourDSL& dsl,
                                                const StressPotential& sp,
                                                const std::string& id) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    ViscoplasticFlowBase::endTreatment(bd, dsl, sp, id);
    auto mts = getMiddleOfTimeStepModifier(bd);
    CodeBlock i;
    i.code +=
        generateMaterialPropertyInitializationCode(dsl, bd, "A" + id, this->A);
    i.code +=
        generateMaterialPropertyInitializationCode(dsl, bd, "K" + id, this->K);
    i.code +=
        generateMaterialPropertyInitializationCode(dsl, bd, "E" + id, this->n);
    if (!this->Ksf.empty()) {
      i.code += generateMaterialPropertyInitializationCode(dsl, bd, "Ksf" + id,
                                                           this->Ksf);
    }
    if (!i.code.empty()) {
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  bool NortonHoffViscoplasticFlow::describesStrainHardeningExplicitly() const {
    return false;
  }  // end of describesStrainHardeningExplicitly

  std::string NortonHoffViscoplasticFlow::computeFlowRate(
      const BehaviourDescription&,
      const StressPotential&,
      const std::string& id) const {
    auto c = std::string{};
    if (this->ihrs.empty()) {
      if (!this->Ksf.empty()) {
        c += "if(seq" + id + ">(this->Ksf" + id + ")*this->K" + id + "){\n";
        c += "return false;\n";
        c += "}\n";
      }
      c += "const auto vp" + id + " = ";
      c += "(this->A" + id + ")*pow(std::max((seq" + id + ")/(this->K" + id +
           "),real(0)),this->E" + id + ");\n";
    } else {
      if (!this->Ksf.empty()) {
        c += "if((seq" + id + "-R" + id + ")>";
        c += "(this->Ksf" + id + ")*this->K" + id + "){\n";
        c += "return false;\n";
        c += "}\n";
      }
      c += "const auto vp" + id + " = ";
      c += "(this->A" + id + ")*pow(std::max((seq" + id + "-R" + id +
           ")/(this->K" + id + "),real(0)),this->E" + id + ");\n";
    }
    return c;
  }  // end of computeFlowRate

  std::string NortonHoffViscoplasticFlow::computeFlowRateAndDerivative(
      const BehaviourDescription&,
      const StressPotential&,
      const std::string& id) const {
    auto c = std::string{};
    if (this->ihrs.empty()) {
      if (!this->Ksf.empty()) {
        c += "if(seq" + id + " > (this->Ksf" + id + ") * (this->K" + id +
             ")){\n";
        c += "return false;\n";
        c += "}\n";
      }
      const auto rseq = "(seq" + id + ") / (this->K" + id + ")";
      const auto bseq = "std::max(" + rseq + ", real(0))";
      c += "const auto vp" + id + " = ";
      c += "(this->A" + id + ") * pow(" + bseq + ", this->E" + id + ");\n";
      c += "const auto dvp" + id + "_dseqe" + id + " = ";
      const auto seps = "(this->epsilon) * (this->K" + id + ")";
      const auto bseq2 = "(std::max(seq" + id + ", " + seps + "))";
      c += "(this->E" + id + ") * vp" + id + " / " + bseq2 + ";\n";
    } else {
      if (!this->Ksf.empty()) {
        c += "if((seq" + id + " - R" + id + ")>";
        c += "(this->Ksf" + id + ")*this->K" + id + "){\n";
        c += "return false;\n";
        c += "}\n";
      }
      const auto rseq = "(seq" + id + " - R" + id + ") / (this->K" + id + ")";
      const auto bseq = "std::max(" + rseq + ", real(0))";
      c += "const auto vp" + id + " = ";
      c += "(this->A" + id + ") * pow(" + bseq + ", this->E" + id + ");\n";
      c += "const auto dvp" + id + "_dseqe" + id + " = ";
      const auto seps = "(this->epsilon) * (this->K" + id + ")";
      const auto bseq2 =
          "(std::max(seq" + id + " - R" + id + ", " + seps + "))";
      c += "(this->E" + id + ") * vp" + id + " / " + bseq2 + ";\n";
    }
    return c;
  }  // end of computeFlowRateAndDerivative

  NortonHoffViscoplasticFlow::~NortonHoffViscoplasticFlow() = default;

}  // end of namespace mfront::bbrick
