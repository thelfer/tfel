/*!
 * \file   tfel-check/src/L2Comparison.cxx
 * \brief  comparison of two curves by the L2 norm of their difference
 * \date   20/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Check/L2Comparison.hxx"

namespace tfel::check {

  L2Comparison::L2Comparison() : AreaComparison("l2", Norm::L2) {}

  L2Comparison::~L2Comparison() = default;

}  // end of namespace tfel::check
