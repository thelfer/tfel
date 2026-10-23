/*!
 * \file   tfel-check/include/TFEL/Check/L2Comparison.hxx
 * \brief  comparison of two curves by the L2 norm of their difference
 * \date   20/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_L2COMPARISON_HXX
#define LIB_TFELCHECK_L2COMPARISON_HXX

#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/AreaComparison.hxx"

namespace tfel::check {

  /*!
   * \brief comparison of two curves by the L2 norm of their difference,
   * normalized by the maximum absolute value of the reference. This is
   * the counterpart of the Area comparison, which uses the L1 norm.
   */
  struct TFELCHECK_VISIBILITY_EXPORT L2Comparison : public AreaComparison {
    L2Comparison();
    ~L2Comparison() override;
  };

}  // end of namespace tfel::check

#endif /* LIB_TFELCHECK_L2COMPARISON_HXX */
