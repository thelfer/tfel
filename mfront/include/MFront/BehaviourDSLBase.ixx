/*!
 * \file   mfront/include/MFront/BehaviourDSLBase.ixx
 * \brief
 * \author Helfer Thomas
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
  void BehaviourDSLBase<Child>::registerDefaultCallBacks(void) {
    this->registerNewCallBack(";", &Child::treatLonelySeparator);
    this->registerNewCallBack("@DSL", &Child::treatParser);
    this->registerNewCallBack("@Parser", &Child::treatParser);
    this->registerNewCallBack("@Model", &Child::treatModel);
    this->registerNewCallBack("@Brick", &Child::treatBrick);
    this->registerNewCallBack("@ModellingHypothesis",
                              &Child::treatModellingHypothesis);
    this->registerNewCallBack("@ModellingHypotheses",
                              &Child::treatModellingHypotheses);
    this->registerNewCallBack("@Import", &Child::treatImport);
    this->registerNewCallBack("@Material", &Child::treatMaterial);
    this->registerNewCallBack("@Library", &Child::treatLibrary);
    this->registerNewCallBack("@Profiling", &Child::treatProfiling);
    this->registerNewCallBack("@Behaviour", &Child::treatBehaviour);
    this->registerNewCallBack("@Author", &Child::treatAuthor);
    this->registerNewCallBack("@Date", &Child::treatDate);
    this->registerNewCallBack("@MFront", &Child::treatMFront);
    this->registerNewCallBack("@Link", &Child::treatLink);
    this->registerNewCallBack("@Includes", &Child::treatIncludes);
    this->registerNewCallBack("@Members", &Child::treatMembers);
    this->registerNewCallBack("@Coef", &Child::treatCoef);
    this->registerNewCallBack("@MaterialProperty", &Child::treatCoef);
    this->registerNewCallBack("@LocalVar", &Child::treatLocalVar);
    this->registerNewCallBack("@LocalVariable", &Child::treatLocalVar);
    this->registerNewCallBack("@Parameter", &Child::treatParameter);
    this->registerNewCallBack("@StateVar", &Child::treatStateVariable);
    this->registerNewCallBack("@StateVariable", &Child::treatStateVariable);
    this->registerNewCallBack("@AuxiliaryStateVar",
                              &Child::treatAuxiliaryStateVariable);
    this->registerNewCallBack("@AuxiliaryStateVariable",
                              &Child::treatAuxiliaryStateVariable);
    this->registerNewCallBack("@ExternalStateVar",
                              &Child::treatExternalStateVariable);
    this->registerNewCallBack("@ExternalStateVariable",
                              &Child::treatExternalStateVariable);
    this->registerNewCallBack("@InitLocalVars",
                              &Child::treatInitLocalVariables);
    this->registerNewCallBack("@InitLocalVariables",
                              &Child::treatInitLocalVariables);
    this->registerNewCallBack("@InitializeLocalVariables",
                              &Child::treatInitLocalVariables);
    this->registerNewCallBack("@MinimalTimeStepScalingFactor",
                              &Child::treatMinimalTimeStepScalingFactor);
    this->registerNewCallBack("@MaximalTimeStepScalingFactor",
                              &Child::treatMaximalTimeStepScalingFactor);
    this->registerNewCallBack("@APrioriTimeStepScalingFactor",
                              &Child::treatAPrioriTimeStepScalingFactor);
    this->registerNewCallBack("@Integrator", &Child::treatIntegrator);
    this->registerNewCallBack("@APosterioriTimeStepScalingFactor",
                              &Child::treatAPosterioriTimeStepScalingFactor);
    this->registerNewCallBack("@Interface", &Child::treatInterface);
    this->registerNewCallBack("@StaticVar", &Child::treatStaticVar);
    this->registerNewCallBack("@StaticVariable", &Child::treatStaticVar);
    this->registerNewCallBack("@IntegerConstant", &Child::treatIntegerConstant);
    this->registerNewCallBack("@UseQt", &Child::treatUseQt);
    this->registerNewCallBack("@Description", &Child::treatDescription);
    this->registerNewCallBack("@Bounds", &Child::treatBounds);
    this->registerNewCallBack("@PhysicalBounds", &Child::treatPhysicalBounds);
    this->registerNewCallBack("@RequireStiffnessOperator",
                              &Child::treatRequireStiffnessOperator);
    this->registerNewCallBack("@RequireStiffnessTensor",
                              &Child::treatRequireStiffnessTensor);
    this->registerNewCallBack(
        "@RequireThermalExpansionCoefficientTensor",
        &Child::treatRequireThermalExpansionCoefficientTensor);
    this->registerNewCallBack("@OrthotropicBehaviour",
                              &Child::treatOrthotropicBehaviour);
    this->registerNewCallBack("@IsotropicElasticBehaviour",
                              &Child::treatIsotropicElasticBehaviour);
    this->registerNewCallBack("@IsotropicBehaviour",
                              &Child::treatIsotropicBehaviour);
    this->registerNewCallBack("@PredictionOperator",
                              &Child::treatPredictionOperator);
    this->registerNewCallBack("@Private", &Child::treatPrivate);
    this->registerNewCallBack("@Sources", &Child::treatSources);
    this->registerNewCallBack("@UpdateAuxiliaryStateVars",
                              &Child::treatUpdateAuxiliaryStateVariables);
    this->registerNewCallBack("@UpdateAuxiliaryStateVariables",
                              &Child::treatUpdateAuxiliaryStateVariables);
    this->registerNewCallBack("@ComputeThermalExpansion",
                              &Child::treatComputeThermalExpansion);
    this->registerNewCallBack("@ComputeStressFreeExpansion",
                              &Child::treatComputeStressFreeExpansion);
    this->registerNewCallBack("@Swelling", &Child::treatSwelling);
    this->registerNewCallBack("@AxialGrowth", &Child::treatAxialGrowth);
    this->registerNewCallBack("@Relocation", &Child::treatRelocation);
    this->registerNewCallBack("@InternalEnergy", &Child::treatInternalEnergy);
    this->registerNewCallBack("@DissipatedEnergy",
                              &Child::treatDissipatedEnergy);
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
  BehaviourDSLBase<Child>::BehaviourDSLBase() : BehaviourDSLCommon() {
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
          } catch (std::exception& e) {
            std::ostringstream msg;
            msg << "BehaviourDSLBase::analyse: "
                << "error while treating keyword '" << k << "' at line '" << l
                << "' of file '" << this->fileName << "'.\n"
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
      const std::string& fileName_,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->fileName = fileName_;
    this->openFile(this->fileName, ecmds, s);
    this->analyse();
  }  // end of BehaviourDSLBase<Child>::importFile

  template <typename Child>
  void BehaviourDSLBase<Child>::analyseString(const std::string& s) {
    this->fileName = "user defined string";
    this->parseString(s);
    this->analyse();
  }  // end of BehaviourDSLCommon::analyseString

  template <typename Child>
  BehaviourDSLBase<Child>::~BehaviourDSLBase() = default;

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURDSLBASE_IXX */
