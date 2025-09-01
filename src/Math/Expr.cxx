/*!
 * \file   src/Math/Expr.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/Parser/BinaryOperator.hxx"
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel::math::parser {

  Expr::~Expr() = default;

  void mergeVariablesNames(std::vector<std::string>& v,
                           const std::vector<std::string>& nv) {
    for (const auto& vn : nv) {
      if (std::find(v.begin(), v.end(), vn) == v.end()) {
        v.push_back(vn);
      }
    }
  }  // end of mergeVariablesNames

  std::shared_ptr<Expr> applyChainRule(const std::shared_ptr<Expr> d1,
                                       const std::shared_ptr<Expr> d2) {
    if (d2->isConstant()) {
      const auto v = d2->getValue() - 1;
      if (tfel::math::ieee754::fpclassify(v) == FP_ZERO) {
        return d1;
      }
    }
    return std::make_shared<BinaryOperation<OpMult>>(d1, d2);
  }  // end of applyChainRule

}  // end of namespace tfel::math::parser
