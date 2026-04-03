/*!
 * \file   include/TFEL/Math/General/LogicalOperations.hxx
 * \brief  This file implements the OpOr and OpAnd classes.
 * \author Thomas Helfer
 * \date   06 mai 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_LOGICALOPERATIONS_HXX
#define LIB_TFEL_MATH_LOGICALOPERATIONS_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::math {

  template <bool a, bool b>
  struct OpOr {
    static constexpr bool cond = a || b;
  };

  template <bool a, bool b>
  struct OpAnd {
    static constexpr bool cond = a && b;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_LOGICALOPERATIONS_HXX */
