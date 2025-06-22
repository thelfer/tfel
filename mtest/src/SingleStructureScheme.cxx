/*!
 * \file   SingleStructureScheme.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <sstream>

#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/LogarithmicStrain1DBehaviourWrapper.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/StudyCurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/SingleStructureScheme.hxx"

namespace mtest {

  static void checkIfDeclared(const std::vector<std::string>& names,
                              const EvolutionManager& m,
                              const std::string& type) {
    for (const auto& n : names) {
      tfel::raise_if(m.find(n) == m.end(), "no '" + type +
                                               "' named "
                                               "'" +
                                               n + "' declared");
    }
  }

  static void checkIfDeclared(const std::vector<std::string>& names,
                              const EvolutionManager& evm1,
                              const EvolutionManager& evm2,
                              const std::string& type) {
    for (const auto& n : names) {
      if (evm1.find(n) == evm1.end()) {
        tfel::raise_if(evm2.find(n) == evm2.end(), "no " + type +
                                                       " named "
                                                       "'" +
                                                       n + "' declared");
      }
    }
  }

  SingleStructureScheme::SingleStructureScheme()
      : dmpv(new EvolutionManager()) {
  }  // end of SingleStructureScheme::SingleStructureScheme

  void SingleStructureScheme::setMaterialProperty(const std::string& n,
                                                  const EvolutionPtr p,
                                                  const bool check) {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::setMaterialProperty: "
                   "no behaviour defined");
    const auto& mpnames = this->b->getMaterialPropertiesNames();
    const bool is_mp =
        std::find(mpnames.begin(), mpnames.end(), n) == mpnames.end();
    if ((is_mp) && (n != "ThermalExpansion") && (n != "ThermalExpansion1") &&
        (n != "ThermalExpansion2") && (n != "ThermalExpansion3")) {
      std::ostringstream msg;
      msg << "SingleStructureScheme::setMaterialProperty: "
          << "the behaviour does not declare a material property '" << n
          << "'.";
      if (!mpnames.empty()) {
        msg << "\nThe behaviour declares:";
        for (const auto& mp : mpnames) {
          msg << "\n- '" << mp << "'";
        }
      }
      tfel::raise(msg.str());
    }
    if ((n == "ThermalExpansion") || (n == "ThermalExpansion1") ||
        (n == "ThermalExpansion2") || (n == "ThermalExpansion3")) {
      if (is_mp) {
        this->addEvolution(n, p, true, check);
      } else {
        this->addEvolution(n, p, false, check);
      }
    } else {
      this->addEvolution(n, p, false, check);
    }
  }  // end of SingleStructureScheme::setMaterialProperty

  void SingleStructureScheme::setExternalStateVariable(const std::string& n,
                                                       const EvolutionPtr p,
                                                       const bool check) {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::setExternalStateVariable: "
                   "no behaviour defined");
    const auto& evsnames = this->b->getExternalStateVariablesNames();
    tfel::raise_if(find(evsnames.begin(), evsnames.end(), n) == evsnames.end(),
                   "SingleStructureScheme::setExternalStateVariable: "
                   "the behaviour does not declare an "
                   "external state variable named '" +
                       n + "'");
    this->addEvolution(n, p, false, check);
  }  // end of SingleStructureScheme::setExternalStateVariable

  void SingleStructureScheme::setOutOfBoundsPolicy(
      const tfel::material::OutOfBoundsPolicy p) {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::setOutOfBoundsPolicy: "
                   "no behaviour defined");
    this->b->setOutOfBoundsPolicy(p);
  }

  void SingleStructureScheme::setBehaviour(const std::string& i,
                                           const std::string& l,
                                           const std::string& f,
                                           const Parameters& d) {
    if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->setDefaultModellingHypothesis();
    }
    this->setBehaviour(Behaviour::getBehaviour(i, l, f, d, this->hypothesis));
  }

  void SingleStructureScheme::setBehaviour(const std::string& w,
                                           const std::string& i,
                                           const std::string& l,
                                           const std::string& f,
                                           const Parameters& d) {
    if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->setDefaultModellingHypothesis();
    }
    auto bp = Behaviour::getBehaviour(i, l, f, d, this->hypothesis);
    if (w == "LogarithmicStrain1D") {
      auto wp = std::shared_ptr<Behaviour>(
          new LogarithmicStrain1DBehaviourWrapper(bp));
      this->setBehaviour(wp);
    } else {
      tfel::raise(
          "SingleStructureScheme::setBehaviour: "
          "unknown wrapper '" +
          w + "'");
    }
  }

  void SingleStructureScheme::checkBehaviourConsistency(
      const std::shared_ptr<Behaviour>&) {}

  void SingleStructureScheme::setBehaviour(
      const std::shared_ptr<Behaviour>& bp) {
    tfel::raise_if(this->b != nullptr,
                   "SingleStructureScheme::setBehaviour: "
                   "behaviour already defined");
    this->checkBehaviourConsistency(bp);
    this->b = bp;
    this->declareVariables(this->b->getMaterialPropertiesNames(), true);
    this->declareVariables(this->b->getExternalStateVariablesNames(), true);
    this->ivfullnames = this->b->expandInternalStateVariablesNames();
    for (const auto& n : this->ivfullnames) {
      this->declareVariable(n, true);
    }
    // declaring behaviour variables
    this->declareVariables(this->b->getDrivingVariablesComponents(), true);
    this->declareVariables(this->b->getThermodynamicForcesComponents(), true);
  }  // end of SingleStructureScheme::setBehaviour

  void SingleStructureScheme::setParameter(const std::string& n,
                                           const double v) {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::setParameter: "
                   "no behaviour defined");
    this->b->setParameter(n, v);
  }

  void SingleStructureScheme::setIntegerParameter(const std::string& n,
                                                  const int v) {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::setIntegerParameter: "
                   "no behaviour defined");
    this->b->setIntegerParameter(n, v);
  }

  void SingleStructureScheme::setUnsignedIntegerParameter(
      const std::string& n, const unsigned int v) {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::setUnsignedIntegerParameter: "
                   "no behaviour defined");
    this->b->setUnsignedIntegerParameter(n, v);
  }

  tfel::material::MechanicalBehaviourBase::BehaviourType
  SingleStructureScheme::getBehaviourType() const {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::getBehaviourType: "
                   "no behaviour defined");
    return this->b->getBehaviourType();
  }

  tfel::material::MechanicalBehaviourBase::Kinematic
  SingleStructureScheme::getBehaviourKinematic() const {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::getBehaviourKinematic: "
                   "no behaviour defined");
    return this->b->getBehaviourKinematic();
  }

  std::shared_ptr<Behaviour> SingleStructureScheme::getBehaviour() {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::getBehaviour: "
                   "no behaviour defined");
    return this->b;
  }

  void SingleStructureScheme::completeInitialisation() {
    SchemeBase::completeInitialisation();
    tfel::raise_if(this->b == nullptr,
                   "MTest::completeInitialisation: "
                   "no behaviour defined");
    // check if material properties and external state variables are declared
    const auto mpnames = this->b->getMaterialPropertiesNames();
    const auto esvnames = this->b->getExternalStateVariablesNames();
    this->b->setOptionalMaterialPropertiesDefaultValues(*(this->dmpv),
                                                        *(this->evm));
    checkIfDeclared(mpnames, *(this->evm), *(this->dmpv), "material property");
    checkIfDeclared(esvnames, *(this->evm), "external state variable");
  }  // end of SingleStructureScheme::completeInitialisation

  bool SingleStructureScheme::doPackagingStep(StudyCurrentState& state) const {
    auto& scs = state.getStructureCurrentState("");
    auto& bwk = scs.getBehaviourWorkSpace();
    for (auto& s : scs.istates) {
      if (!this->b->doPackagingStep(s, bwk)) {
        return false;
      }
    }
    return true;
  }  // end of SingleStructureScheme::doPackagingStep

  StiffnessMatrixType SingleStructureScheme::getDefaultStiffnessMatrixType()
      const {
    return this->b->getDefaultStiffnessMatrixType();
  }  // end of SingleStructureScheme::getDefaultStiffnessMatrixType

  void SingleStructureScheme::setScalarInternalStateVariableInitialValue(
      const std::string& n, const real v) {
    tfel::raise_if(
        this->b == nullptr,
        "SingleStructureScheme::setScalarInternalStateVariableInitialValue: "
        "no behaviour defined");
    const auto& ivsnames = this->b->getInternalStateVariablesNames();
    tfel::raise_if(
        std::find(ivsnames.begin(), ivsnames.end(), n) == ivsnames.end(),
        "SingleStructureScheme::setScalarInternalStateVariableInitialValue: "
        "the behaviour does not declare an internal "
        "state variable named '" +
            n + "'");
    const auto type = this->b->getInternalStateVariableType(n);
    const auto pos = this->b->getInternalStateVariablePosition(n);
    tfel::raise_if(
        type != 0,
        "SingleStructureScheme::setScalarInternalStateVariableInitialValue: "
        "internal state variable '" +
            n + "' is not defined");
    if (this->iv_t0.size() <= pos) {
      this->iv_t0.resize(pos + 1, 0.);
    }
    this->iv_t0[pos] = v;
  }

  void SingleStructureScheme::setStensorInternalStateVariableInitialValues(
      const std::string& n, const std::vector<real>& v) {
    tfel::raise_if(
        this->b == nullptr,
        "SingleStructureScheme::setStensorInternalStateVariableInitialValue: "
        "no behaviour defined");
    const auto& ivsnames = this->b->getInternalStateVariablesNames();
    tfel::raise_if(
        std::find(ivsnames.begin(), ivsnames.end(), n) == ivsnames.end(),
        "SingleStructureScheme::setStensorInternalStateVariableInitialValue: "
        "the behaviour don't declare an internal "
        "state variable named '" +
            n + "'");
    const auto type = this->b->getInternalStateVariableType(n);
    const auto pos = this->b->getInternalStateVariablePosition(n);
    tfel::raise_if(type != 1,
                   "SingleStructureScheme::"
                   "setStensorInternalStateVariableInitialValue: "
                   "internal state variable '" +
                       n + "' is not defined");
    const auto N = tfel::material::getStensorSize(this->hypothesis);
    tfel::raise_if(
        v.size() != N,
        "SingleStructureScheme::setStensorInternalStateVariableInitialValues: "
        "invalid values size");
    if (this->iv_t0.size() < pos + N) {
      this->iv_t0.resize(pos + N, 0.);
    }
    std::copy(v.begin(), v.end(), this->iv_t0.begin() + pos);
  }  // end of
     // SingleStructureScheme::setStensorInternalStateVariableInitialValue

  void SingleStructureScheme::setTensorInternalStateVariableInitialValues(
      const std::string& n, const std::vector<real>& v) {
    tfel::raise_if(
        this->b == nullptr,
        "SingleStructureScheme::setTensorInternalStateVariableInitialValue: "
        "no behaviour defined");
    const auto& ivsnames = this->b->getInternalStateVariablesNames();
    tfel::raise_if(
        std::find(ivsnames.begin(), ivsnames.end(), n) == ivsnames.end(),
        "SingleStructureScheme::setTensorInternalStateVariableInitialValue: "
        "the behaviour does not declare an internal state "
        "variable named '" +
            n + "'");
    const auto type = this->b->getInternalStateVariableType(n);
    const auto pos = this->b->getInternalStateVariablePosition(n);
    tfel::raise_if(
        type != 3,
        "SingleStructureScheme::setTensorInternalStateVariableInitialValue: "
        "internal state variable '" +
            n + "' is not defined");
    const auto N = tfel::material::getTensorSize(this->hypothesis);
    tfel::raise_if(
        v.size() != N,
        "SingleStructureScheme::setTensorInternalStateVariableInitialValues: "
        "invalid values size");
    if (this->iv_t0.size() < pos + N) {
      this->iv_t0.resize(pos + N, 0.);
    }
    copy(v.begin(), v.end(), this->iv_t0.begin() + pos);
  }  // end of SingleStructureScheme::setTensorInternalStateVariableInitialValue

  void SingleStructureScheme::prepare(StudyCurrentState& state,
                                      const real t,
                                      const real dt) const {
    using namespace tfel::material;
    auto& scs = state.getStructureCurrentState("");
    // evaluations of the materials properties, state variables at the
    // end of the time step. Computation of thermal expansion if needed.
    for (auto& s : scs.istates) {
      this->setGaussPointPositionForEvolutionsEvaluation(s);
      computeMaterialProperties(s, *(this->evm), *(this->dmpv),
                                this->b->getMaterialPropertiesNames(), t, dt);
      computeExternalStateVariables(
          s, *(this->evm), this->b->getExternalStateVariablesNames(), t, dt);
      // thermal expansion
      if ((this->handleThermalExpansion) &&
          ((this->b->getBehaviourType() ==
            MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
           ((this->b->getBehaviourType() ==
             MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
            (this->b->getBehaviourKinematic() ==
             MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1)))) {
        if (this->b->getSymmetryType() == 0) {
          // isotropic case
          computeThermalExpansion(s, *(this->evm), t, dt);
        } else if (this->b->getSymmetryType() == 1) {
          // orthotropic case
          computeThermalExpansion(s, *(this->evm), t, dt,
                                  getSpaceDimension(this->hypothesis));
        } else {
          tfel::raise(
              "SingleStructureScheme::prepare: "
              "unsupported behaviour symmetry");
        }
      }
    }
    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
      auto& log = mfront::getLogStream();
      log << "resolution from " << t << " to " << t + dt << '\n';
    }
    if (this->residual) {
      this->residual << '\n'
                     << "#resolution from " << t << " to " << t + dt << '\n';
    }
  }  // end of SingleStructureScheme::prepare

  void SingleStructureScheme::setHandleThermalExpansion(const bool b1) {
    tfel::raise_if(!this->handleThermalExpansion,
                   "SingleStructureScheme::setHandleThermalExpansion: "
                   "thermal expansion is not handled");
    this->handleThermalExpansion = b1;
  }

  SingleStructureScheme::~SingleStructureScheme() = default;

}  // end of namespace mtest
