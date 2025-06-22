/*!
 * \file   HyperbolicSineViscoplasticFlow.cxx
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
#include "MFront/BehaviourBrick/HyperbolicSineViscoplasticFlow.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription> HyperbolicSineViscoplasticFlow::getOptions()
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
    }  // end of HyperbolicSineViscoplasticFlow::getOptions

    void HyperbolicSineViscoplasticFlow::initialize(BehaviourDescription& bd,
                                                    AbstractBehaviourDSL& dsl,
                                                    const std::string& id,
                                                    const DataMap& d) {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      auto get_mp = [&dsl, &bd, &id, &d](const std::string& mpn,
                                         const std::string& t,
                                         const std::string& vn) {
        if (d.count(mpn) == 0) {
          tfel::raise(
              "HyperbolicSineViscoplasticFlow::"
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
      if (d.count("n") != 0) {
        this->n = get_mp("n", "real", "E");
      }
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
      bd.reserveName(uh, "seqe" + id + "_K");
      bd.reserveName(uh, "exp_seqe" + id + "_K");
      bd.reserveName(uh, "sinh_seqe" + id + "_K");
      bd.reserveName(uh, "sinh_seqe" + id + "_K__n_1");
      bd.reserveName(uh, "cosh_seqe" + id + "_K");
    }  // end of HyperbolicSineViscoplasticFlow::initialize

    void HyperbolicSineViscoplasticFlow::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const StressPotential& sp,
        const std::string& id) const {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      ViscoplasticFlowBase::endTreatment(bd, dsl, sp, id);
      if ((!this->A.is<BehaviourDescription::ConstantMaterialProperty>()) ||
          (!this->K.is<BehaviourDescription::ConstantMaterialProperty>()) ||
          (!this->n.is<BehaviourDescription::ConstantMaterialProperty>())) {
        auto mts = getMiddleOfTimeStepModifier(bd);
        CodeBlock i;
        auto eval = [&mts, &dsl, &i](
                        const BehaviourDescription::MaterialProperty& mp,
                        const std::string& mpn) {
          if (!mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
            std::ostringstream mps;
            mps << "this->" + mpn + " = ";
            dsl.writeMaterialPropertyEvaluation(mps, mp, mts);
            mps << ";\n";
            i.code += mps.str();
          }
        };
        eval(this->A, "A" + id);
        eval(this->K, "K" + id);
        if (!this->n.empty()) {
          eval(this->n, "E" + id);
        }
        if (!this->Ksf.empty()) {
          eval(this->Ksf, "Ksf" + id);
        }
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of KinematicHardeningRuleBase::endTreatment

    std::string HyperbolicSineViscoplasticFlow::computeFlowRate(
        const std::string& id) const {
      auto c = std::string{};
      if (this->ihrs.empty()) {
        if (!this->Ksf.empty()) {
          c += "if(seq" + id + ">(this->Ksf" + id + ")*this->K" + id + "){\n";
          c += "return false;\n";
          c += "}\n";
        }
        c += "const auto vp" + id + " = ";
        if (!this->n.empty()) {
          c += "(this->A" + id + ")*pow(sinh(std::max(seq" + id + "/(this->K" +
               id + "),real(0))),this->E" + id + ");\n";
        } else {
          c += "(this->A" + id + ")*sinh(std::max(seq" + id + "/(this->K" + id +
               "),real(0)));\n";
        }
      } else {
        if (!this->Ksf.empty()) {
          c += "if((seq" + id + "-R" + id + ")>";
          c += "(this->Ksf" + id + ")*this->K" + id + "){\n";
          c += "return false;\n";
          c += "}\n";
        }
        c += "const auto vp" + id + " = ";
        if (!this->n.empty()) {
          c += "(this->A" + id + ")*pow(sinh(std::max((seq" + id + "-R" + id +
               ")/(this->K" + id + "),real(0))),this->E" + id + ");\n";
        } else {
          c += "(this->A" + id + ")*sinh(std::max((seq" + id + "-R" + id +
               ")/(this->K" + id + "),real(0)));\n";
        }
      }
      return c;
    }  // end of HyperbolicSineViscoplasticFlow::computeFlowRate

    std::string HyperbolicSineViscoplasticFlow::computeFlowRateAndDerivative(
        const std::string& id) const {
      auto c = std::string{};
      const auto seq_n = "seq" + id;
      const auto R_n = "R" + id;
      const auto K_n = "this->K" + id;
      const auto Ksf_n = "this->Ksf" + id;
      const auto A_n = "this->A" + id;
      const auto E_n = "this->E" + id;
      if (this->ihrs.empty()) {
        if (!this->Ksf.empty()) {
          c += "if(" + seq_n + ">(" + Ksf_n + ")*" + K_n + "){\n";
          c += "return false;\n";
          c += "}\n";
        }
        c += "const auto seqe" + id + "_K = " + seq_n + "/(" + K_n + ");\n";
      } else {
        if (!this->Ksf.empty()) {
          c += "if((" + seq_n + " - " + R_n + ")> (" + Ksf_n + ") * " + K_n +
               "){\n";
          c += "return false;\n";
          c += "}\n";
        }
        c += "const auto seqe" + id + "_K = ";
        c += "(" + seq_n + " - " + R_n + ")/(" + K_n + ");\n";
      }
      c += "const auto exp_seqe" + id + "_K = ";
      c += "exp(seqe" + id + "_K);\n";
      c += "const auto sinh_seqe" + id + "_K = ";
      c += "(exp_seqe" + id + "_K-1/exp_seqe" + id + "_K)/2;\n";
      c += "const auto cosh_seqe" + id + "_K = ";
      c += "(exp_seqe" + id + "_K+1/exp_seqe" + id + "_K)/2;\n";
      if (!this->n.empty()) {
        c += "const auto sinh_seqe" + id + "_K__n_1 = ";
        c += "std::pow(sinh_seqe" + id + "_K," + E_n + "-1);\n";
        c += "const auto vp" + id + " = ";
        c += A_n + " * sinh_seqe" + id + "_K__n_1 * sinh_seqe" + id + "_K;\n";
        c += "const auto dvp" + id + "_dseqe" + id + " = ";
        c += "(" + E_n + ") * " + A_n + "*sinh_seqe" + id + "_K__n_1 * ";
        c += "cosh_seqe" + id + "_K/(" + K_n + ");\n";
      } else {
        c += "const auto vp" + id + " = ";
        c += A_n + " * sinh_seqe" + id + "_K;\n";
        c += "const auto dvp" + id + "_dseqe" + id + " = ";
        c += A_n + "* cosh_seqe" + id + "_K/(" + K_n + ");\n";
      }
      return c;
    }  // end of HyperbolicSineViscoplasticFlow::computeFlowRateAndDerivative

    HyperbolicSineViscoplasticFlow::~HyperbolicSineViscoplasticFlow() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
