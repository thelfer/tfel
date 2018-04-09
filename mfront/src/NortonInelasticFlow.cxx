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
      using namespace tfel::glossary;
      auto get_mp = [&dsl, &bd, &id, &d](const std::string& mpn) {
        if (d.count(mpn) == 0) {
          tfel::raise(
              "NortonInelasticFlow::initialize: "
              "material property '" +
              mpn + "' is not defined");
        }
        auto mp = getBehaviourDescriptionMaterialProperty(dsl, mpn, d.at(mpn));
        declareParameterOrLocalVariable(bd, mp, mpn + id);
        return mp;
      };
      // checking options
      mfront::bbrick::check(d, this->getOptions());
      // the base class
      InelasticFlowBase::initialize(bd, dsl, id, d);
      // Norton flow options
      this->K = get_mp("K");
      this->n = get_mp("n");
      if (d.count("A") != 0) {
        this->A = get_mp("A");
      } else {
        BehaviourDescription::ConstantMaterialProperty cmp;
        cmp.value = 1;
        this->A = cmp;
        declareParameterOrLocalVariable(bd, this->A, "A" + id);
      }
      if (id.empty()) {
        addStateVariable(bd, "strain", "p",
                         Glossary::EquivalentViscoplasticStrain);
      } else {
        addStateVariable(bd, "strain", "p" + id,
                         static_cast<const std::string&>(
                             Glossary::EquivalentViscoplasticStrain) +
                             id);
      }
    }  // end of NortonInelasticFlow::initialize

    void NortonInelasticFlow::endTreatment(BehaviourDescription& bd,
                                           const AbstractBehaviourDSL& dsl,
                                           const StressPotential& sp,
                                           const std::string& id) const {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      InelasticFlowBase::endTreatment(bd, dsl, sp, id);
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
        eval(this->n, "n" + id);
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of KinematicHardeningRuleBase::endTreatment

    std::vector<OptionDescription> NortonInelasticFlow::getOptions() const {
      auto opts = InelasticFlowBase::getOptions();
      opts.emplace_back("A", "Norton coefficient (optional)",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("K", "Stress normalisation factor",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("n", "Norton exponent",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of NortonInelasticFlow::getOptions

    std::string NortonInelasticFlow::buildFlowImplicitEquations(
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const std::string& id,
        const bool b) const {
      auto c = std::string{};
      if (this->ihr != nullptr) {
        if (b) {
          c += this->ihr->computeElasticLimitAndDerivative(id);
          c += "const auto dvp" + id + " = ";
          c += "(this->A" + id + ")*pow((seq" + id + "-R" + id + ")/(this->K" +
               id + "),this->n" + id + "-1)/this->K" + id + ";\n";
          c += "fp" + id + " -= dvp" + id + "*(seq" + id + "-R" + id +
               ")*(this->dt);\n";
          c += sp.computeDerivatives(bd, "p" + id, "-(this->n" + id + ")*dvp" +
                                                       id + "*(this->dt)*dseq" +
                                                       id + "_ds" + id);
          c += "dfp" + id + "_ddp" + id + " += (this->n" + id + ")*dvp" + id +
               "*(this->dt)*dR" + id + "_ddp" + id + ";\n";
          auto kid = decltype(khrs.size()){};
          for (const auto& khr : khrs) {
            c += khr->computeDerivatives("p", "(this->n" + id + ")*dvp" + id +
                                                  "*(this->dt)*dseq" + id +
                                                  "_ds" + id,
                                         id, std::to_string(kid));
            ++kid;
          }
        } else {
          c += this->ihr->computeElasticLimit(id);
          c += "fp" + id + " -= ";
          c += "(this->A" + id + ")*pow((seq" + id + "-R" + id + ")/(this->K" +
               id + "),this->n" + id + ")*(this->dt);\n";
        }
      } else {
        if (b) {
          c += "const auto dvp" + id + " = ";
          c += "(this->A" + id + ")*pow(seq" + id + "/(this->K" + id +
               "),this->n" + id + "-1)/this->K" + id + ";\n";
          c += "fp" + id + " -= dvp" + id + "*seq" + id + "*(this->dt);\n";
          c += sp.computeDerivatives(bd, "p" + id, "-(this->n" + id + ")*dvp" +
                                                       id + "*(this->dt)*dseq" +
                                                       id + "_ds" + id);
          auto kid = decltype(khrs.size()){};
          for (const auto& khr : khrs) {
            c += khr->computeDerivatives("p", "(this->n" + id + ")*dvp" + id +
                                                  "*(this->dt)*dseq" + id +
                                                  "_ds" + id,
                                         id, std::to_string(kid));
            ++kid;
          }
        } else {
          c += "fp" + id + " -= ";
          c += "(this->A" + id + ")*pow(seq" + id + "/(this->K" + id +
               "),this->n" + id + ")*(this->dt);\n";
        }
      }
      return c;
    }  // end of NortonInelasticFlow::buildFlowImplicitEquations

    NortonInelasticFlow::~NortonInelasticFlow() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
