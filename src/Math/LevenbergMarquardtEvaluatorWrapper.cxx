/*!
 * \file  src/Math/LevenbergMarquardtEvaluatorWrapper.cxx
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
#include "TFEL/Math/LevenbergMarquardt/LevenbergMarquardtEvaluatorWrapper.hxx"

namespace tfel {

  namespace math {

    LevenbergMarquardtEvaluatorWrapper::LevenbergMarquardtEvaluatorWrapper(
        std::shared_ptr<tfel::math::Evaluator> ev_,
        const LevenbergMarquardtEvaluatorWrapper::size_type nv_,
        const LevenbergMarquardtEvaluatorWrapper::size_type np_)
        : ev(std::move(ev_)), dev(np_), nv(nv_), np(np_) {
      using tfel::math::vector;
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "EvaluatorBase::splitAtTokenSeperator: " + m);
      };
      throw_if(
          this->ev->getNumberOfVariables() !=
              this->getNumberOfVariables() + this->getNumberOfParameters(),
          "the number of variables declared in the specified evaluator is not "
          "equal "
          "to the sum of the number of variables and the number of paramters");
      throw_if(this->getNumberOfVariables() == 0, "null variable number");
      throw_if(this->getNumberOfParameters() == 0, "null parameters number");
      for (vector<double>::size_type i = 0; i != this->getNumberOfParameters();
           ++i) {
        this->dev[i] =
            this->ev->differentiate(this->getNumberOfVariables() + i);
      }
    }  // end of
       // LevenbergMarquardtEvaluatorWrapper::LevenbergMarquardtEvaluatorWrapper

    LevenbergMarquardtEvaluatorWrapper::size_type
    LevenbergMarquardtEvaluatorWrapper::getNumberOfVariables() const {
      return this->nv;
    }  // end of LevenbergMarquardtEvaluatorWrapper::getNumberOfVariables()

    LevenbergMarquardtEvaluatorWrapper::size_type
    LevenbergMarquardtEvaluatorWrapper::getNumberOfParameters() const {
      return this->np;
    }  // end of LevenbergMarquardtEvaluatorWrapper::getNumberOfParameters()

    void LevenbergMarquardtEvaluatorWrapper::operator()(
        double& r,
        tfel::math::vector<double>& g,
        const tfel::math::vector<double>& variables,
        const tfel::math::vector<double>& params) {
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "EvaluatorBase::splitAtTokenSeperator: " + m);
      };
      throw_if(variables.size() != this->getNumberOfVariables(),
               "invalid number of variables");
      throw_if(params.size() != this->getNumberOfParameters(),
               "invalid number of parameters");
      std::vector<double>::size_type i = 0;
      g.resize(this->getNumberOfParameters());
      for (const auto& v : variables) {
        this->ev->setVariableValue(i, v);
        for (const auto& d : this->dev) {
          d->setVariableValue(i, v);
        }
        ++i;
      }
      for (const auto& p : params) {
        this->ev->setVariableValue(i, p);
        for (const auto& d : this->dev) {
          d->setVariableValue(i, p);
        }
        ++i;
      }
      r = this->ev->getValue();
      for (i = 0; i != dev.size(); ++i) {
        g[i] = dev[i]->getValue();
      }
    }  // end of LevenbergMarquardtEvaluatorWrapper::operator()

    LevenbergMarquardtEvaluatorWrapper::~LevenbergMarquardtEvaluatorWrapper() =
        default;

  }  // end of namespace math

}  // end of namespace tfel
