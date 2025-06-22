/*!
 * \file
 * include/TFEL/Math/LevenbergMarquardt/LevenbergMarquardtEvaluatorWrapper.hxx
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

#ifndef LIB_LEVENBERGMARQUARDTEVALUATORWRAPPER_HXX
#define LIB_LEVENBERGMARQUARDTEVALUATORWRAPPER_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include <memory>
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/Evaluator.hxx"

namespace tfel {

  namespace math {

    struct TFELMATHPARSER_VISIBILITY_EXPORT LevenbergMarquardtEvaluatorWrapper {
      typedef tfel::math::vector<double>::size_type size_type;
      typedef double NumericType;

      LevenbergMarquardtEvaluatorWrapper(std::shared_ptr<tfel::math::Evaluator>,
                                         const size_type,
                                         const size_type);

      size_type getNumberOfVariables() const;

      size_type getNumberOfParameters() const;

      void operator()(double&,
                      tfel::math::vector<double>&,
                      const tfel::math::vector<double>&,
                      const tfel::math::vector<double>&);

      ~LevenbergMarquardtEvaluatorWrapper();

     private:
      std::shared_ptr<tfel::math::Evaluator> ev;
      tfel::math::vector<std::shared_ptr<tfel::math::parser::ExternalFunction>>
          dev;
      size_type nv;
      size_type np;
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_LEVENBERGMARQUARDTEVALUATORWRAPPER_HXX */
