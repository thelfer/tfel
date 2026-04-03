/*!
 * \file  src/Math/LevenbergMarquardtExternalFunctionWrapper.cxx
 * \brief
 * \author Thomas Helfer
 * \date 24/02/2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/LevenbergMarquardtExternalFunctionWrapper.hxx"

namespace tfel::math::parser {

  LevenbergMarquardtExternalFunctionWrapper::
      LevenbergMarquardtExternalFunctionWrapper(
          std::shared_ptr<tfel::math::parser::ExternalFunction> ev_,
          const LevenbergMarquardtExternalFunctionWrapper::size_type nv_,
          const LevenbergMarquardtExternalFunctionWrapper::size_type np_)
      : ev(std::move(ev_)), dev(np_), nv(nv_), np(np_) {
    using namespace tfel::math;
    using namespace tfel::math::parser;
    vector<double>::size_type i;
    raise_if(this->ev->getNumberOfVariables() !=
                 this->getNumberOfVariables() + this->getNumberOfParameters(),
             "LevenbergMarquardtExternalFunctionWrapper::"
             "LevenbergMarquardtExternalFunctionWrapper: "
             "the number of variables declared in the "
             "specified externalFunction is not equal "
             "to the sum of the number of variables and "
             "the number of paramters");
    raise_if(this->getNumberOfVariables() == 0,
             "LevenbergMarquardtExternalFunctionWrapper::"
             "LevenbergMarquardtExternalFunctionWrapper: "
             "null variable number");
    raise_if(this->getNumberOfParameters() == 0,
             "LevenbergMarquardtExternalFunctionWrapper::"
             "LevenbergMarquardtExternalFunctionWrapper: "
             "null parameters number");
    for (i = 0; i != this->getNumberOfParameters(); ++i) {
      dev[i] = this->ev->differentiate(this->getNumberOfVariables() + i);
    }
  }  // end of
     // LevenbergMarquardtExternalFunctionWrapper::LevenbergMarquardtExternalFunctionWrapper

  LevenbergMarquardtExternalFunctionWrapper::size_type
  LevenbergMarquardtExternalFunctionWrapper::getNumberOfVariables() const {
    return this->nv;
  }  // end of
     // LevenbergMarquardtExternalFunctionWrapper::getNumberOfVariables()

  LevenbergMarquardtExternalFunctionWrapper::size_type
  LevenbergMarquardtExternalFunctionWrapper::getNumberOfParameters() const {
    return this->np;
  }  // end of
     // LevenbergMarquardtExternalFunctionWrapper::getNumberOfParameters()

  void LevenbergMarquardtExternalFunctionWrapper::operator()(
      double& r,
      tfel::math::vector<double>& g,
      const tfel::math::vector<double>& variables,
      const tfel::math::vector<double>& params) {
    using namespace tfel::math;
    using namespace tfel::math::parser;
    raise_if(variables.size() != this->getNumberOfVariables(),
             "LevenbergMarquardtExternalFunctionWrapper::operator(): "
             "invalid number of variables");
    raise_if(params.size() != this->getNumberOfParameters(),
             "LevenbergMarquardtExternalFunctionWrapper::operator(): "
             "invalid number of parameters");
    g.resize(this->getNumberOfParameters());
    vector<double>::size_type i = 0;
    for (const auto a : variables) {
      this->ev->setVariableValue(i, a);
      for (auto pdev = dev.begin(); pdev != dev.end(); ++pdev) {
        (*pdev)->setVariableValue(i, a);
      }
      ++i;
    }
    for (const auto& p : params) {
      this->ev->setVariableValue(i, p);
      for (auto pdev = dev.begin(); pdev != dev.end(); ++pdev) {
        (*pdev)->setVariableValue(i, p);
      }
      ++i;
    }
    r = this->ev->getValue();
    for (i = 0; i != dev.size(); ++i) {
      g[i] = dev[i]->getValue();
    }
  }  // end of LevenbergMarquardtExternalFunctionWrapper::operator()

}  // end of namespace tfel::math::parser
