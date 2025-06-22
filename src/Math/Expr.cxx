/*!
 * \file   src/Math/Expr.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>

#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel {
  namespace math {

    namespace parser {

      Expr::~Expr() = default;

      void mergeVariablesNames(std::vector<std::string>& v,
                               const std::vector<std::string>& nv) {
        for (const auto& vn : nv) {
          if (std::find(v.begin(), v.end(), vn) == v.end()) {
            v.push_back(vn);
          }
        }
      }  // end of mergeVariablesNames

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
