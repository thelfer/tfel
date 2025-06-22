/*!
 * \file   SingleStructureScheme.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09/12/2015
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
#include "MTest/SmallStrainTridimensionalBehaviourWrapper.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/StudyCurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/SingleStructureScheme.hxx"

namespace mtest {

  SingleStructureScheme::SingleStructureScheme()
      : dmpv(new EvolutionManager()) {}  // end of SingleStructureScheme

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
  }  // end of setMaterialProperty

  void SingleStructureScheme::setExternalStateVariable(const std::string& n,
                                                       const EvolutionPtr p,
                                                       const bool check) {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::setExternalStateVariable: "
                   "no behaviour defined");
    const auto& evsnames = this->b->expandExternalStateVariablesNames();
    tfel::raise_if(find(evsnames.begin(), evsnames.end(), n) == evsnames.end(),
                   "SingleStructureScheme::setExternalStateVariable: "
                   "the behaviour does not declare an "
                   "external state variable named '" +
                       n + "'");
    this->addEvolution(n, p, false, check);
  }  // end of setExternalStateVariable

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
    if (w == "LogarithmicStrain1D") {
      auto bp = Behaviour::getBehaviour(i, l, f, d, this->hypothesis);
      auto wp = std::make_shared<LogarithmicStrain1DBehaviourWrapper>(bp);
      this->setBehaviour(wp);
    } else if (w == "SmallStrainTridimensionalBehaviourWrapper") {
      auto bp = Behaviour::getBehaviour(i, l, f, d,
                                        ModellingHypothesis::TRIDIMENSIONAL);
      auto wp = std::make_shared<SmallStrainTridimensionalBehaviourWrapper>(
          bp, this->hypothesis);
      this->setBehaviour(wp);
    } else {
      tfel::raise("SingleStructureScheme::setBehaviour: unknown wrapper '" + w +
                  "'");
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
    this->declareVariables(this->b->expandExternalStateVariablesNames(), true);
    this->ivfullnames = this->b->expandInternalStateVariablesNames();
    for (const auto& n : this->ivfullnames) {
      this->declareVariable(n, true);
    }
    // declaring behaviour variables
    this->declareVariables(this->b->getGradientsComponents(), true);
    this->declareVariables(this->b->getThermodynamicForcesComponents(), true);
    // adding parameters
    const auto& n = this->b->getBehaviourName();
    for (const auto& p : this->b->getParametersNames()) {
      const auto v = this->b->getRealParameterDefaultValue(p);
      if (mfront::getVerboseMode() >= mfront::VERBOSE_DEBUG) {
        mfront::getLogStream()
            << "Adding evolution '" << n << "::" << p << "' from parameter '"
            << p << "' with value '" << v << "'\n";
      }
      this->addEvolution(n + "::" + p, std::make_shared<ConstantEvolution>(v),
                         true, true);
    }
  }  // end of setBehaviour

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

  std::shared_ptr<const Behaviour> SingleStructureScheme::getBehaviour() const {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::getBehaviour: "
                   "no behaviour defined");
    return this->b;
  }

  void SingleStructureScheme::completeInitialisation() {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::completeInitialisation: "
                   "no behaviour defined");
    SchemeBase::completeInitialisation();
    // check if material properties and external state variables are declared
    const auto mpnames = this->b->getMaterialPropertiesNames();
    const auto esvnames = this->b->expandExternalStateVariablesNames();
    this->b->setOptionalMaterialPropertiesDefaultValues(*(this->dmpv),
                                                        *(this->evm));
    checkIfDeclared(mpnames, *(this->evm), *(this->dmpv), "material property");
    checkIfDeclared(esvnames, *(this->evm), "external state variable");
  }  // end of completeInitialisation

  bool SingleStructureScheme::doPackagingStep(StudyCurrentState& state) const {
    auto& scs = state.getStructureCurrentState("");
    auto& bwk = scs.getBehaviourWorkSpace();
    for (auto& s : scs.istates) {
      if (!this->b->doPackagingStep(s, bwk)) {
        return false;
      }
    }
    return true;
  }  // end of doPackagingStep

  StiffnessMatrixType SingleStructureScheme::getDefaultStiffnessMatrixType()
      const {
    tfel::raise_if(this->b == nullptr,
                   "SingleStructureScheme::getDefaultStiffnessMatrixType: "
                   "no behaviour defined");
    return this->b->getDefaultStiffnessMatrixType();
  }  // end of getDefaultStiffnessMatrixType

  void SingleStructureScheme::setInternalStateVariableInitialValue(
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

  void SingleStructureScheme::setInternalStateVariableInitialValue(
      const std::string& n, const std::vector<real>& v) {
    tfel::raise_if(
        this->b == nullptr,
        "SingleStructureScheme::setInternalStateVariableInitialValue: "
        "no behaviour defined");
    const auto& ivsnames = this->b->getInternalStateVariablesNames();
    tfel::raise_if(
        std::find(ivsnames.begin(), ivsnames.end(), n) == ivsnames.end(),
        "SingleStructureScheme::setInternalStateVariableInitialValue: "
        "the behaviour don't declare an internal "
        "state variable named '" +
            n + "'");
    const auto pos = this->b->getInternalStateVariablePosition(n);
    const auto s = getVariableSize(this->b->getInternalStateVariableType(n),
                                   this->b->getHypothesis());
    tfel::raise_if(
        v.size() != s,
        "SingleStructureScheme::setInternalStateVariableInitialValue: "
        "invalid values size");
    if (this->iv_t0.size() < pos + s) {
      this->iv_t0.resize(pos + s, 0.);
    }
    std::copy(v.begin(), v.end(), this->iv_t0.begin() + pos);
  }  // end of setInternalStateVariableInitialValue

  void SingleStructureScheme::setScalarInternalStateVariableInitialValue(
      const std::string& n, const real v) {
    this->setInternalStateVariableInitialValue(n, v);
  }

  void SingleStructureScheme::setStensorInternalStateVariableInitialValues(
      const std::string& n, const std::vector<real>& v) {
    this->setInternalStateVariableInitialValue(n, v);
  }  // end of setStensorInternalStateVariableInitialValue

  void SingleStructureScheme::setTensorInternalStateVariableInitialValues(
      const std::string& n, const std::vector<real>& v) {
    this->setInternalStateVariableInitialValue(n, v);
  }  // end of setTensorInternalStateVariableInitialValues

  std::pair<bool, real> SingleStructureScheme::prepare(StudyCurrentState& state,
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
          s, *(this->evm), this->b->expandExternalStateVariablesNames(), t, dt);
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
    return {true, 1};
  }  // end of prepare

  void SingleStructureScheme::setHandleThermalExpansion(const bool b1) {
    tfel::raise_if(!this->handleThermalExpansion,
                   "SingleStructureScheme::setHandleThermalExpansion: "
                   "thermal expansion is not handled");
    this->handleThermalExpansion = b1;
  }  // end of setHandleThermalExpansion

  SingleStructureScheme::~SingleStructureScheme() = default;

}  // end of namespace mtest
