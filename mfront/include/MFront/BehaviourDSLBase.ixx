/*!
 * \file   mfront/include/MFront/BehaviourDSLBase.ixx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURDSLBASE_IXX
#define LIB_MFRONT_BEHAVIOURDSLBASE_IXX

#include <sstream>
#include <stdexcept>
#include <algorithm>

#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/MFrontLogStream.hxx"

namespace mfront {

  template <typename Child>
  void BehaviourDSLBase<Child>::registerNewCallBack(const std::string& keyword,
                                                    const MemberFuncPtr f) {
    this->callBacks.insert(make_pair(keyword, f));
    this->registredKeyWords.insert(keyword);
  }  // end of registerNewCallBack

  template <typename Child>
  void BehaviourDSLBase<Child>::registerDefaultCallBacks() {
    auto add = [this](const std::string& k, const MemberFuncPtr f) {
      this->registerNewCallBack(k, f);
    };
    add(";", &Child::treatLonelySeparator);
    add("@DSL", &Child::treatParser);
    add("@Parser", &Child::treatParser);
    add("@Model", &Child::treatModel);
    add("@Brick", &Child::treatBrick);
    add("@ModellingHypothesis", &Child::treatModellingHypothesis);
    add("@ModellingHypotheses", &Child::treatModellingHypotheses);
    add("@Import", &Child::treatImport);
    add("@Material", &Child::treatMaterial);
    add("@Library", &Child::treatLibrary);
    add("@Profiling", &Child::treatProfiling);
    add("@Behaviour", &Child::treatBehaviour);
    add("@StrainMeasure", &Child::treatStrainMeasure);
    add("@Author", &Child::treatAuthor);
    add("@Date", &Child::treatDate);
    add("@MFront", &Child::treatMFront);
    add("@Link", &Child::treatLink);
    add("@Includes", &Child::treatIncludes);
    add("@Members", &Child::treatMembers);
    add("@Coef", &Child::treatCoef);
    add("@MaterialProperty", &Child::treatCoef);
    add("@LocalVar", &Child::treatLocalVar);
    add("@LocalVariable", &Child::treatLocalVar);
    add("@Parameter", &Child::treatParameter);
    add("@StateVar", &Child::treatStateVariable);
    add("@StateVariable", &Child::treatStateVariable);
    add("@AuxiliaryStateVar", &Child::treatAuxiliaryStateVariable);
    add("@AuxiliaryStateVariable", &Child::treatAuxiliaryStateVariable);
    add("@ExternalStateVar", &Child::treatExternalStateVariable);
    add("@ExternalStateVariable", &Child::treatExternalStateVariable);
    add("@InitLocalVars", &Child::treatInitLocalVariables);
    add("@InitLocalVariables", &Child::treatInitLocalVariables);
    add("@InitializeLocalVariables", &Child::treatInitLocalVariables);
    add("@MinimalTimeStepScalingFactor",
        &Child::treatMinimalTimeStepScalingFactor);
    add("@MaximalTimeStepScalingFactor",
        &Child::treatMaximalTimeStepScalingFactor);
    add("@APrioriTimeStepScalingFactor",
        &Child::treatAPrioriTimeStepScalingFactor);
    add("@Integrator", &Child::treatIntegrator);
    add("@APosterioriTimeStepScalingFactor",
        &Child::treatAPosterioriTimeStepScalingFactor);
    add("@Interface", &Child::treatInterface);
    add("@StaticVar", &Child::treatStaticVar);
    add("@StaticVariable", &Child::treatStaticVar);
    add("@IntegerConstant", &Child::treatIntegerConstant);
    add("@UseQt", &Child::treatUseQt);
    add("@Description", &Child::treatDescription);
    add("@Bounds", &Child::treatBounds);
    add("@PhysicalBounds", &Child::treatPhysicalBounds);
    add("@RequireStiffnessOperator", &Child::treatRequireStiffnessOperator);
    add("@RequireStiffnessTensor", &Child::treatRequireStiffnessTensor);
    add("@RequireThermalExpansionCoefficientTensor",
        &Child::treatRequireThermalExpansionCoefficientTensor);
    add("@OrthotropicBehaviour", &Child::treatOrthotropicBehaviour);
    add("@IsotropicElasticBehaviour", &Child::treatIsotropicElasticBehaviour);
    add("@IsotropicBehaviour", &Child::treatIsotropicBehaviour);
    add("@PredictionOperator", &Child::treatPredictionOperator);
    add("@Private", &Child::treatPrivate);
    add("@Sources", &Child::treatSources);
    add("@UpdateAuxiliaryStateVars",
        &Child::treatUpdateAuxiliaryStateVariables);
    add("@UpdateAuxiliaryStateVariables",
        &Child::treatUpdateAuxiliaryStateVariables);
    add("@ComputeThermalExpansion", &Child::treatComputeThermalExpansion);
    add("@ComputeStressFreeExpansion", &Child::treatComputeStressFreeExpansion);
    add("@Swelling", &Child::treatSwelling);
    add("@AxialGrowth", &Child::treatAxialGrowth);
    add("@Relocation", &Child::treatRelocation);
    add("@InternalEnergy", &Child::treatInternalEnergy);
    add("@DissipatedEnergy", &Child::treatDissipatedEnergy);
    add("@CrystalStructure", &Child::treatCrystalStructure);
    add("@SlipSystem", &Child::treatSlipSystem);
    add("@GlidingSystem", &Child::treatSlipSystem);
    add("@SlidingSystem", &Child::treatSlipSystem);
    add("@SlipSystems", &Child::treatSlipSystems);
    add("@GlidingSystems", &Child::treatSlipSystems);
    add("@SlidingSystems", &Child::treatSlipSystems);
    add("@InteractionMatrix", &Child::treatInteractionMatrix);
    add("@DislocationsMeanFreePathInteractionMatrix",
        &Child::treatDislocationsMeanFreePathInteractionMatrix);
  }  // end of BehaviourDSLBase<Child>::registerDefaultCallBacks

  template <typename Child>
  void BehaviourDSLBase<Child>::getKeywordsList(
      std::vector<std::string>& k) const {
    for (const auto& c : this->callBacks) {
      k.push_back(c.first);
    }
  }  // end of BehaviourDSLBase<Child>::getKeywordsList

  template <typename Child>
  void BehaviourDSLBase<Child>::treatDisabledCallBack() {
    --(this->current);
    throw(std::runtime_error("The keyword: '" + this->current->value +
                             "' has been disabled"));
  }  // end of treatDisabledCallBack

  template <typename Child>
  void BehaviourDSLBase<Child>::disableCallBack(const std::string& name) {
    auto p = this->callBacks.find(name);
    if (p == this->callBacks.end()) {
      this->registerNewCallBack(name, &Child::treatDisabledCallBack);
      return;
    }
    p->second = &Child::treatDisabledCallBack;
  }  // end of disableCallBack

  template <typename Child>
  BehaviourDSLBase<Child>::BehaviourDSLBase() {
    // Register callbacks
    this->registerDefaultCallBacks();
  }  // end of BehaviourDSLBase<Child>::BehaviourDSLBase

  template <typename Child>
  void BehaviourDSLBase<Child>::analyse() {
    const auto& mh = ModellingHypothesis::getModellingHypotheses();
    std::vector<std::string> hn(mh.size());
    std::vector<Hypothesis>::const_iterator pmh;
    std::vector<std::string>::iterator phn;
    for (pmh = mh.begin(), phn = hn.begin(); pmh != mh.end(); ++pmh, ++phn) {
      *phn = ModellingHypothesis::toString(*pmh);
    }
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->tokens.begin();
    while (this->current != this->tokens.end()) {
      if (find(hn.begin(), hn.end(), this->current->value) != hn.end()) {
        const auto h = ModellingHypothesis::fromString(this->current->value);
        ++(this->current);
        this->checkNotEndOfFile("BehaviourDSLBase<Child>::analyse");
        this->readSpecifiedToken("BehaviourDSLBase<Child>::analyse", "::");
        if (!this->isCallableVariable(h, this->current->value)) {
          throw(
              std::runtime_error("BehaviourDSLBase<Child>::analyse : "
                                 "no variable named '" +
                                 this->current->value +
                                 "' "
                                 "for hypothesis '" +
                                 ModellingHypothesis::toString(h) + "'"));
        }
        if (this->mb.isParameterName(h, this->current->value)) {
          this->treatParameterMethod(h);
        } else {
          this->treatVariableMethod(h);
        }
      } else {
        const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
        if (this->isCallableVariable(h, this->current->value)) {
          if (this->mb.isParameterName(h, this->current->value)) {
            this->treatParameterMethod(h);
          } else {
            this->treatVariableMethod(h);
          }
        } else {
          auto k = this->current->value;
          auto l = this->current->line;
          MemberFuncPtr handler;
          auto p = this->callBacks.find(k);
          if (p == this->callBacks.end()) {
            if (getVerboseMode() >= VERBOSE_DEBUG) {
              auto& log = getLogStream();
              log << "treating unknown keyword\n";
            }
            handler = &Child::treatUnknownKeyword;
          } else {
            if (getVerboseMode() >= VERBOSE_DEBUG) {
              auto& log = getLogStream();
              log << "treating keyword : " << this->current->value << '\n';
            }
            handler = p->second;
          }
          this->currentComment = this->current->comment;
          ++(this->current);
          try {
            ((static_cast<Child*>(this))->*handler)();
            const auto ph = this->hooks.find(k);
            if (ph != this->hooks.end()) {
              for (auto& hook : ph->second) {
                hook();
              }
            }
          } catch (std::exception& e) {
            std::ostringstream msg;
            msg << "BehaviourDSLBase::analyse: "
                << "error while treating keyword '" << k << "' at line '" << l
                << "' of file '" << this->fd.fileName << "'.\n"
                << e.what();
            throw(std::runtime_error(msg.str()));
          } catch (...) {
            this->currentComment.clear();
            throw;
          }
          this->currentComment.clear();
        }
      }
    }
  }

  template <typename Child>
  void BehaviourDSLBase<Child>::importFile(
      const std::string& fn,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->fd.fileName = fn;
    this->openFile(this->fd.fileName, ecmds, s);
    this->analyse();
  }  // end of BehaviourDSLBase<Child>::importFile

  template <typename Child>
  void BehaviourDSLBase<Child>::analyseString(const std::string& s) {
    this->fd.fileName = "user defined string";
    this->parseString(s);
    this->analyse();
  }  // end of BehaviourDSLCommon::analyseString

  template <typename Child>
  void BehaviourDSLBase<Child>::addHook(const std::string& k, const Hook h) {
    if (this->callBacks.find(k) == this->callBacks.end()) {
      this->throwRuntimeError("BehaviourDSLBase::addHook",
                              "no callback called '" + k + "'");
    }
    this->hooks[k].push_back(h);
  }  // end of BehaviourDSLBase<Child>::addHook

  template <typename Child>
  BehaviourDSLBase<Child>::~BehaviourDSLBase() = default;

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURDSLBASE_IXX */
