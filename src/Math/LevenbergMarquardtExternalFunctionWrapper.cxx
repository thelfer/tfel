/*!
 * \file  src/Math/LevenbergMarquardtExternalFunctionWrapper.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 24 fév 2010
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
#include "TFEL/Math/LevenbergMarquardt/LevenbergMarquardtExternalFunctionWrapper.hxx"

namespace tfel {

  namespace math {

    LevenbergMarquardtExternalFunctionWrapper::
        LevenbergMarquardtExternalFunctionWrapper(
            std::shared_ptr<tfel::math::parser::ExternalFunction> ev_,
            const LevenbergMarquardtExternalFunctionWrapper::size_type nv_,
            const LevenbergMarquardtExternalFunctionWrapper::size_type np_)
        : ev(std::move(ev_)), dev(np_), nv(nv_), np(np_) {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      using tfel::math::vector;
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
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      using tfel::math::vector;
      vector<double>::const_iterator p;
      vector<shared_ptr<ExternalFunction>>::iterator pdev;
      vector<double>::size_type i;
      raise_if(variables.size() != this->getNumberOfVariables(),
               "LevenbergMarquardtExternalFunctionWrapper::operator(): "
               "invalid number of variables");
      raise_if(params.size() != this->getNumberOfParameters(),
               "LevenbergMarquardtExternalFunctionWrapper::operator(): "
               "invalid number of parameters");
      i = 0;
      g.resize(this->getNumberOfParameters());
      for (p = variables.begin(); p != variables.end(); ++p, ++i) {
        this->ev->setVariableValue(i, *p);
        for (pdev = dev.begin(); pdev != dev.end(); ++pdev) {
          (*pdev)->setVariableValue(i, *p);
        }
      }
      for (p = params.begin(); p != params.end(); ++p, ++i) {
        this->ev->setVariableValue(i, *p);
        for (pdev = dev.begin(); pdev != dev.end(); ++pdev) {
          (*pdev)->setVariableValue(i, *p);
        }
      }
      r = this->ev->getValue();
      for (i = 0; i != dev.size(); ++i) {
        g[i] = dev[i]->getValue();
      }
    }  // end of LevenbergMarquardtExternalFunctionWrapper::operator()

  }  // end of namespace math

}  // end of namespace tfel
