/*!
 * \file   include/TFEL/Math/Evaluator.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   12 jan 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_EVALUATORIXX
#define LIB_TFEL_MATH_EVALUATORIXX

#include "TFEL/Math/Parser/Function.hxx"
#include "TFEL/Math/Parser/BinaryFunction.hxx"

namespace tfel {

  namespace math {

    template <double (*f)(const double)>
    std::shared_ptr<tfel::math::parser::Expr> StandardFctGenerator(
        std::shared_ptr<tfel::math::parser::Expr> e) {
      using namespace tfel::math::parser;
      using tfel::math::parser::Expr;
      return std::shared_ptr<Expr>(new StandardFunction<f>(e));
    }  // end of Evaluator::FctGenerator::StandardFctGenerator

    template <double (*f)(const double, const double)>
    std::shared_ptr<tfel::math::parser::Expr> StandardBinaryFctGenerator(
        std::shared_ptr<tfel::math::parser::Expr> e1,
        std::shared_ptr<tfel::math::parser::Expr> e2) {
      using namespace tfel::math::parser;
      using tfel::math::parser::Expr;
      return std::shared_ptr<Expr>(new StandardBinaryFunction<f>(e1, e2));
    }  // end of Evaluator::FctGenerator::StandardBinaryFctGenerator

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_EVALUATORIXX */
