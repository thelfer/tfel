/*!
 * \file   NortonInelasticFlow.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
#include "MFront/BehaviourBrick/NortonInelasticFlow.hxx"

namespace mfront {

  namespace bbrick {

    void NortonInelasticFlow::initialize(BehaviourDescription& bd,
                                         AbstractBehaviourDSL& dsl,
                                         const std::string& id,
                                         const DataMap& d) {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      auto get_mp = [&dsl, &bd, &id, &d](const std::string& mpn,
                                         const std::string& vn) {
        if (d.count(mpn) == 0) {
          tfel::raise(
              "NortonInelasticFlow::initialize: "
              "material property '" +
              mpn + "' is not defined");
        }
        auto mp = getBehaviourDescriptionMaterialProperty(dsl, mpn, d.at(mpn));
        declareParameterOrLocalVariable(bd, mp, vn + id);
        return mp;
      };
      // checking options
      mfront::bbrick::check(d, this->getOptions());
      // the base class
      ViscoplasticFlowBase::initialize(bd, dsl, id, d);
      // Norton flow options
      this->K = get_mp("K", "K");
      this->n = get_mp("n", "E");
      if (d.count("A") != 0) {
        this->A = get_mp("A", "A");
      } else {
        BehaviourDescription::ConstantMaterialProperty cmp;
        cmp.value = 1;
        this->A = cmp;
        declareParameterOrLocalVariable(bd, this->A, "A" + id);
      }
      if (d.count("Ksf") != 0) {
        this->Ksf = get_mp("Ksf", "Ksf");
      }
      bd.reserveName(uh, "seqe" + id + "_K__n_1");
    }  // end of NortonInelasticFlow::initialize

    void NortonInelasticFlow::endTreatment(BehaviourDescription& bd,
                                           const AbstractBehaviourDSL& dsl,
                                           const StressPotential& sp,
                                           const std::string& id) const {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      ViscoplasticFlowBase::endTreatment(bd, dsl, sp, id);
      if ((!this->A.is<BehaviourDescription::ConstantMaterialProperty>()) ||
          (!this->K.is<BehaviourDescription::ConstantMaterialProperty>()) ||
          (!this->n.is<BehaviourDescription::ConstantMaterialProperty>())){
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
        eval(this->n, "E" + id);
        if (!this->Ksf.empty()) {
          eval(this->Ksf, "Ksf" + id);
        }
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of KinematicHardeningRuleBase::endTreatment

    std::vector<OptionDescription> NortonInelasticFlow::getOptions() const {
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
    }  // end of NortonInelasticFlow::getOptions

    std::string NortonInelasticFlow::computeFlowRate(
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
    } // end of NortonInelasticFlow::computeFlowRate

    std::string NortonInelasticFlow::computeFlowRateAndDerivative(
        const std::string& id) const {
      auto c = std::string{};
      if (this->ihrs.empty()) {
        if (!this->Ksf.empty()) {
          c += "if(seq" + id + ">(this->Ksf" + id + ")*this->K" + id + "){\n";
          c += "return false;\n";
          c += "}\n";
        }
        c += "const auto seqe" + id + "_K__n_1 = pow(std::max((seq" + id +
             ")/(this->K" + id + "),this->epsilon),this->E" + id + "-1);\n";
        c += "const auto dvp" + id + "_dseqe" + id + " = ";
        c += "(this->E" + id + ")*(this->A" + id + ")*seqe" + id +
             "_K__n_1/(this->K" + id + ");\n";
        c += "const auto vp" + id + " = ";
        c += "(this->A" + id + ")*seqe" + id + "_K__n_1*(seq" + id +
             ")/(this->K" + id + ");\n";
      } else {
        if (!this->Ksf.empty()) {
          c += "if((seq" + id + "-R" + id + ")>";
          c += "(this->Ksf" + id + ")*this->K" + id + "){\n";
          c += "return false;\n";
          c += "}\n";
        }
        c += "const auto seqe" + id + "_K__n_1 = pow(std::max((seq" + id +
             "-R" + id + ")/(this->K" + id + "),this->epsilon),this->E" + id +
             "-1);\n";
        c += "const auto dvp" + id + "_dseqe" + id + " = ";
        c += "(this->E" + id + ")*(this->A" + id + ")*seqe" + id + "_K__n_1/(this->K" + id + ");\n";
        c += "const auto vp" + id + " = ";
        c += "(this->A" + id + ")*seqe" + id + "_K__n_1*(seq" + id + "-R" + id +
             ")/(this->K" + id + ");\n";
      }
      return c;
    }  // end of NortonInelasticFlow::computeFlowRateAndDerivative

    NortonInelasticFlow::~NortonInelasticFlow() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
