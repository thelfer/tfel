/*!
 * \file   mfront/src/InelasticFlowBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/StressCriterion.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressCriterionFactory.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRuleFactory.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRuleFactory.hxx"
#include "MFront/BehaviourBrick/InelasticFlowBase.hxx"

namespace mfront {

  namespace bbrick {

    void InelasticFlowBase::initialize(BehaviourDescription& bd,
                                       AbstractBehaviourDSL& dsl,
                                       const std::string& id,
                                       const DataMap& d) {
      auto raise = [](const std::string& m) {
        tfel::raise("InelasticFlowBase::initialize: " + m);
      };  // end of raise
      auto getDataStructure = [&raise](const std::string& n, const Data& ds) {
        if (ds.is<std::string>()) {
          tfel::utilities::DataStructure nds;
          nds.name = ds.get<std::string>();
          return nds;
        }
        if (!ds.is<tfel::utilities::DataStructure>()) {
          raise("invalid data type for entry '" + n + "'");
        }
        return ds.get<tfel::utilities::DataStructure>();
      };  // end of getDataStructure
      // checking options
      mfront::bbrick::check(d, this->getOptions());
      // parsing options
      for (const auto& e : d) {
        if (e.first == "criterion") {
          if (this->sc != nullptr) {
            raise("criterion has already been defined");
          }
          const auto ds = getDataStructure(e.first, e.second);
          auto& cf = StressCriterionFactory::getFactory();
          this->sc = cf.generate(ds.name);
          this->sc->initialize(bd, dsl, id, ds.data);
        } else if (e.first == "isotropic_hardening") {
          if (this->ihr != nullptr) {
            raise("isotropic hardening has already been defined");
          }
          const auto ds = getDataStructure(e.first, e.second);
          auto& cf = IsotropicHardeningRuleFactory::getFactory();          
          this->ihr = cf.generate(ds.name);
          this->ihr->initialize(bd, dsl, id, ds.data);
        } else if (e.first == "kinematic_hardening") {
          auto add_kinematic_hardening_rule =
              [this, &bd, &dsl, &id](const tfel::utilities::DataStructure& ds) {
                auto& kf = KinematicHardeningRuleFactory::getFactory();
                const auto k = kf.generate(ds.name);
                k->initialize(bd, dsl, id, std::to_string(this->khrs.size()),
                              ds.data);
                this->khrs.push_back(k);
              };
          if (e.second.is<std::vector<Data>>()) {
            for (const auto krd : e.second.get<std::vector<Data>>()) {
              add_kinematic_hardening_rule(getDataStructure(e.first, krd));
            }
          } else {
            add_kinematic_hardening_rule(getDataStructure(e.first, e.second));
          }
        }
      }
      if (this->sc == nullptr) {
        raise("criterion has not been defined");
      }
      if (this->ihr != nullptr) {
        addLocalVariable(bd, "bool", "bpl" + id);
      }
    }  // end of InelasticFlowBase::initialize

    std::vector<OptionDescription> InelasticFlowBase::getOptions() const {
      std::vector<OptionDescription> opts;
      opts.emplace_back(
          "criterion",
          "stress criterion to be used (Mises, Hill, Hosford, Barlat)",
          OptionDescription::DATASTRUCTURE);
      opts.emplace_back(
          "isotropic_hardening",
          "choice of the isotropic hardening rule",
          OptionDescription::DATASTRUCTURE);
      opts.emplace_back(
          "kinematic_hardening",
          "description of an hardening rule",
          OptionDescription::DATASTRUCTURES);
      return opts;
    }  // end of InelasticFlowBase::getOptions()

    void InelasticFlowBase::completeVariableDeclaration(
        BehaviourDescription&,
        const AbstractBehaviourDSL&,
        const std::string&) const {
    }  // end of InelasticFlowBase::completeVariableDeclaration

    bool InelasticFlowBase::requiresActivationState() const {
      return this->ihr != nullptr;
    }  // end of InelasticFlowBase::requiresActivationState

    void InelasticFlowBase::computeInitialActivationState(
        BehaviourDescription& bd, const std::string& id) const {
      if (this->ihr != nullptr) {
        CodeBlock i;
        if (this->khrs.empty()) {
          i.code += "const auto& sel" + id + " = sigel;\n";
        } else {
          auto kid = decltype(khrs.size()){};
          for (const auto khr : khrs) {
            i.code += khr->computeKinematicHardeningsInitialValues(
                id, std::to_string(kid));
            ++kid;
          }
          // effective stress
          i.code += "const auto sel" + id + " = eval(sigel";
          kid = decltype(khrs.size()){};
          for (const auto& khr : khrs) {
            for (const auto& X : khr->getKinematicHardeningsVariables(
                     id, std::to_string(kid))) {
              i.code += "-" + X;
              ++kid;
            }
          }
          i.code += ");\n";
        }
        i.code += this->sc->computeElasticPrediction(id);
        i.code += this->ihr->computeElasticPrediction(id);
        i.code += "this->bpl" + id + " = seqel" + id + " > Rel" + id + ";\n";
        bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of InelasticFlowBase::computeInitialActivationState

    std::string InelasticFlowBase::computeEffectiveStress(
        const std::string& id) const {
      if (this->khrs.empty()) {
        return "const auto& s" + id + " = this->sig;\n";
      }
      auto c = std::string{};
      auto kid = decltype(khrs.size()){};
      for (const auto khr : khrs) {
        c += khr->computeKinematicHardenings(id, std::to_string(kid));
        ++kid;
      }
      // effective stress
      c += "const auto s" + id + " = eval(this->sig";
      kid = decltype(khrs.size()){};
      for (const auto& khr : khrs) {
        for (const auto& X :
             khr->getKinematicHardeningsVariables(id, std::to_string(kid))) {
          c += "-" + X + "_";
        }
        ++kid;
      }
      c += ");\n";
      return c;
    } // end of InelasticFlowBase::computeEffectiveStress

    void InelasticFlowBase::endTreatment(BehaviourDescription& bd,
                                         const AbstractBehaviourDSL& dsl,
                                         const StressPotential& sp,
                                         const std::string& id) const {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
      const auto requiresAnalyticalJacobian =
          ((idsl.getSolver().usesJacobian()) &&
           (!idsl.getSolver().requiresNumericalJacobian()));
      auto kid = decltype(khrs.size()){};
      // implicit equation associated with the elastic strain
      CodeBlock ib;
      if (this->ihr != nullptr) {
        ib.code += "if(this->bpl" + id + "){\n";
      }
      ib.code += this->computeEffectiveStress(id);
      if (requiresAnalyticalJacobian) {
        ib.code += this->sc->computeNormalDerivative(id);
      } else {
        ib.code += this->sc->computeNormal(id);
      }
      // elasticity
      ib.code += "feel += this->dp" + id + "* dseq" + id + "_ds" + id + ";\n";
      if (requiresAnalyticalJacobian) {
        // jacobian terms
        ib.code += "dfeel_ddp" + id + " = dseq" + id + "_ds" + id + ";\n";
        ib.code +=
            sp.computeDerivatives(bd, "eel", "(this->dp" + id + ")*d2seq" + id +
                                                 "_ds" + id + "ds" + id);
        kid = decltype(khrs.size()){};
        for (const auto& khr : khrs) {
          ib.code += khr->computeDerivatives(
              "eel",
              "-(this->dp" + id + ") * d2seq" + id + "_ds" + id + "ds" + id, id,
              std::to_string(kid));
        }
      }
      // inelastic flow
      ib.code += this->buildFlowImplicitEquations(bd, sp, id,
                                                  requiresAnalyticalJacobian);
      // hardening rules
      kid = decltype(khrs.size()){};
      for (const auto& khr : khrs) {
        ib.code += khr->buildBackStrainImplicitEquations(
            bd, sp, id, std::to_string(kid), requiresAnalyticalJacobian);
        ++kid;
      }
      if (this->ihr != nullptr) {
        ib.code += "} // end if(this->bpl" + id + ")\n";
      }
      bd.setCode(uh, BehaviourData::Integrator, ib,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      // additional checks
      if (this->ihr != nullptr) {
        CodeBlock acc;
        acc.code += "if (converged) {\n";
        acc.code += "// checking status consistency\n";
        acc.code += "if (this->bpl" + id + ") {\n";
        acc.code += "if (dp" + id + " < 0) {\n";
        acc.code += "// desactivating this system\n";
        acc.code += "converged = this->bpl" + id + " = false;\n";
        acc.code += "}\n";
        acc.code += "} else {\n";
        acc.code += this->computeEffectiveStress(id);
        acc.code += this->sc->computeCriterion(id);
        acc.code += this->ihr->computeElasticLimit(id);
        acc.code += "if(seq" + id + " > R" + id + ") {\n";
        acc.code += "converged = false;\n";
        acc.code += "this->bpl" + id + " = true;\n";
        acc.code += "}\n";
        acc.code += "}\n";
        acc.code += "} // end of if(converged)\n";
        bd.setCode(uh, BehaviourData::AdditionalConvergenceChecks, acc,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    } // end of InelasticFlowBase::endTreatment

    InelasticFlowBase::~InelasticFlowBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront