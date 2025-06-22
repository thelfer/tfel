/*!
 * \file   mfront/src/InelasticFlowBase.cxx
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
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
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
          if (d.count("flow_criterion") != 0) {
            this->sc->initialize(bd, dsl, id, ds.data,
                                 StressCriterion::STRESSCRITERION);
          } else {
            this->sc->initialize(bd, dsl, id, ds.data,
                                 StressCriterion::STRESSANDFLOWCRITERION);
          }
        } else if (e.first == "flow_criterion") {
          if (this->fc != nullptr) {
            raise("criterion has already been defined");
          }
          const auto ds = getDataStructure(e.first, e.second);
          auto& cf = StressCriterionFactory::getFactory();
          this->fc = cf.generate(ds.name);
          this->fc->initialize(bd, dsl, id, ds.data,
                               StressCriterion::FLOWCRITERION);
        } else if (e.first == "isotropic_hardening") {
          auto add_isotropic_hardening_rule =
              [this, &bd, &dsl, &id](const tfel::utilities::DataStructure& ds) {
                auto& rf = IsotropicHardeningRuleFactory::getFactory();
                const auto ihr = rf.generate(ds.name);
                ihr->initialize(bd, dsl, id, std::to_string(this->ihrs.size()),
                                ds.data);
                this->ihrs.push_back(ihr);
              };
          if (e.second.is<std::vector<Data>>()) {
            for (const auto& ird : e.second.get<std::vector<Data>>()) {
              add_isotropic_hardening_rule(getDataStructure(e.first, ird));
            }
          } else {
            auto& rf = IsotropicHardeningRuleFactory::getFactory();
            const auto ds = getDataStructure(e.first, e.second);
            const auto ihr = rf.generate(ds.name);
            ihr->initialize(bd, dsl, id, "", ds.data);
            this->ihrs.push_back(ihr);
          }
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
            for (const auto& krd : e.second.get<std::vector<Data>>()) {
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
      if (!this->ihrs.empty()) {
        addLocalVariable(bd, "bool", "bpl" + id);
        if (this->ihrs.size() != 1) {
          const auto Rel = "Rel" + id;
          const auto R = "R" + id;
          const auto dR = "d" + R + "_ddp" + id;
          bd.reserveName(uh, Rel);
          bd.reserveName(uh, R);
          bd.reserveName(uh, dR);
        }
      }
    }  // end of InelasticFlowBase::initialize

    std::vector<OptionDescription> InelasticFlowBase::getOptions() const {
      std::vector<OptionDescription> opts;
      opts.emplace_back(
          "criterion",
          "stress criterion to be used (Mises, Hill, Hosford, Barlat)",
          OptionDescription::DATASTRUCTURE);
      opts.emplace_back("flow_criterion",
                        "stress criterion used to build the plastic potential "
                        "(Mises, Hill, Hosford, Barlat)",
                        OptionDescription::DATASTRUCTURE);
      opts.emplace_back("isotropic_hardening",
                        "choice of an isotropic hardening rule",
                        OptionDescription::DATASTRUCTURES);
      opts.emplace_back("kinematic_hardening",
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
      return !this->ihrs.empty();
    }  // end of InelasticFlowBase::requiresActivationState

    void InelasticFlowBase::computeInitialActivationState(
        BehaviourDescription& bd,
        const StressPotential& sp,
        const std::string& id) const {
      if (!this->ihrs.empty()) {
        CodeBlock i;
        if (this->khrs.empty()) {
          i.code += "const auto& sel" + id + " = sigel;\n";
        } else {
          auto kid = decltype(khrs.size()){};
          for (const auto& khr : khrs) {
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
        i.code += this->sc->computeElasticPrediction(id, bd, sp);
        i.code += computeElasticLimitInitialValue(this->ihrs, id);
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
      for (const auto& khr : khrs) {
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
    }  // end of InelasticFlowBase::computeEffectiveStress

    void InelasticFlowBase::endTreatment(BehaviourDescription& bd,
                                         const AbstractBehaviourDSL& dsl,
                                         const StressPotential& sp,
                                         const std::string& id) const {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
      if (this->fc != nullptr) {
        this->sc->endTreatment(bd, dsl, id, StressCriterion::STRESSCRITERION);
        this->fc->endTreatment(bd, dsl, id, StressCriterion::FLOWCRITERION);
      } else {
        this->sc->endTreatment(bd, dsl, id,
                               StressCriterion::STRESSANDFLOWCRITERION);
      }
      const bool is_deviatoric = (this->fc != nullptr)
                                     ? this->fc->isNormalDeviatoric()
                                     : this->sc->isNormalDeviatoric();
      auto iid = decltype(ihrs.size()){};
      if (this->ihrs.size() == 1) {
        ihrs[0]->endTreatment(bd, dsl, id, "");
      } else {
        for (const auto& ihr : this->ihrs) {
          ihr->endTreatment(bd, dsl, id, std::to_string(iid));
          ++iid;
        }
      }
      auto kid = decltype(khrs.size()){};
      for (const auto& khr : this->khrs) {
        khr->endTreatment(bd, dsl, id, std::to_string(kid));
        ++kid;
      }
      const auto requiresAnalyticalJacobian =
          ((idsl.getSolver().usesJacobian()) &&
           (!idsl.getSolver().requiresNumericalJacobian()));
      // implicit equation associated with the elastic strain
      CodeBlock ib;
      if (!this->ihrs.empty()) {
        ib.code += "if(this->bpl" + id + "){\n";
      }
      if (idsl.getSolver().usesJacobian()) {
        ib.code += "if(!perturbatedSystemEvaluation){\n";
      }
      ib.code += "this->dp" + id + " = max(this->dp" + id + ", strain(0));\n";
      if (idsl.getSolver().usesJacobian()) {
        ib.code += "}\n";
      }
      ib.code += this->computeEffectiveStress(id);
      if (requiresAnalyticalJacobian) {
        if (this->fc == nullptr) {
          ib.code += this->sc->computeNormalDerivative(
              id, bd, sp, StressCriterion::STRESSANDFLOWCRITERION);
        } else {
          ib.code += this->sc->computeNormal(id, bd, sp,
                                             StressCriterion::STRESSCRITERION);
          ib.code += this->fc->computeNormalDerivative(
              id, bd, sp, StressCriterion::FLOWCRITERION);
        }
      } else {
        if (this->fc == nullptr) {
          ib.code += this->sc->computeNormal(
              id, bd, sp, StressCriterion::STRESSANDFLOWCRITERION);
        } else {
          ib.code += this->sc->computeCriterion(id, bd, sp);
          ib.code += this->fc->computeNormal(id, bd, sp,
                                             StressCriterion::FLOWCRITERION);
        }
      }
      // elasticity
      ib.code += "feel += this->dp" + id + "* n" + id + ";\n";
      if (requiresAnalyticalJacobian) {
        // jacobian terms
        ib.code += "dfeel_ddp" + id + " = n" + id + ";\n";
        ib.code += sp.computeDerivatives(
            bd, "StrainStensor", "eel",
            "(this->dp" + id + ")*dn" + id + "_ds" + id, is_deviatoric);
        kid = decltype(khrs.size()){};
        for (const auto& khr : khrs) {
          ib.code += khr->computeDerivatives(
              "eel", "-(this->dp" + id + ") * dn" + id + "_ds" + id, id,
              std::to_string(kid));
          ++kid;
        }
      }
      // inelastic flow
      ib.code += this->buildFlowImplicitEquations(bd, sp, id,
                                                  requiresAnalyticalJacobian);
      // hardening rules
      kid = decltype(khrs.size()){};
      for (const auto& khr : khrs) {
        if (this->fc != nullptr) {
          ib.code += khr->buildBackStrainImplicitEquations(
              bd, sp, *(this->fc), this->khrs, id, std::to_string(kid),
              requiresAnalyticalJacobian);
        } else {
          ib.code += khr->buildBackStrainImplicitEquations(
              bd, sp, *(this->sc), this->khrs, id, std::to_string(kid),
              requiresAnalyticalJacobian);
        }
        ++kid;
      }
      if (!this->ihrs.empty()) {
        ib.code += "} // end if(this->bpl" + id + ")\n";
      }
      bd.setCode(uh, BehaviourData::Integrator, ib,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      // additional checks
      if (!this->ihrs.empty()) {
        CodeBlock acc;
        acc.code += "if (converged) {\n";
        acc.code += "// checking status consistency\n";
        acc.code += "if (this->bpl" + id + ") {\n";
        acc.code += "if (this->dp" + id + " < -2*this->epsilon) {\n";
        acc.code += "// desactivating this system\n";
        acc.code += "converged = this->bpl" + id + " = false;\n";
        acc.code += "} else {\n";
        acc.code += "}\n";
        acc.code += "} else {\n";
        acc.code += this->computeEffectiveStress(id);
        acc.code += this->sc->computeCriterion(id, bd, sp);
        acc.code += computeElasticLimit(this->ihrs, id);
        acc.code += "if(seq" + id + " > R" + id + ") {\n";
        acc.code += "converged = false;\n";
        acc.code += "this->bpl" + id + " = true;\n";
        acc.code += "}\n";
        acc.code += "}\n";
        acc.code += "} // end of if(converged)\n";
        bd.setCode(uh, BehaviourData::AdditionalConvergenceChecks, acc,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of InelasticFlowBase::endTreatment

    InelasticFlowBase::~InelasticFlowBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
