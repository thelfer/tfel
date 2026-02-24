/*!
 * \file AreaComparison.hxx
 *
 *
 *  \class AreaComparison
 *  \brief Does an integration of the absolute difference of the two curves, and
 * compares it to the expected error \date 12 juin 2013 \author Remy Petkantchin
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_AREACOMPARISON_HXX
#define LIB_TFELCHECK_AREACOMPARISON_HXX

#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/Comparison.hxx"

namespace tfel::check {

  struct TFELCHECK_VISIBILITY_EXPORT AreaComparison : public Comparison {
    AreaComparison();
    void compare() override;
    ~AreaComparison() override;
  };

}  // end of namespace tfel::check

#endif /* LIB_TFELCHECK_AREACOMPARISON_HXX */
