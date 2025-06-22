/*!
 * \file RelativeAndAbsoluteComparison.hxx
 *
 *  Created on: 28 mai 2013
 *      Author: rp238441
 *
 *  \class RelativeAndAbsoluteComparison
 *
 *  \brief class that does a relative first, and then an absolute comparison if
 *         the relative didn't pass, between two columns
 *
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_RELATIVEANDABSOLUTECOMPARISON_HXX
#define LIB_TFELCHECK_RELATIVEANDABSOLUTECOMPARISON_HXX

#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/Comparison.hxx"

namespace tfel {

  namespace check {

    struct TFELCHECK_VISIBILITY_EXPORT RelativeAndAbsoluteComparison final
        : public Comparison {
      RelativeAndAbsoluteComparison();
      void compare() override;
      ~RelativeAndAbsoluteComparison() override;
    };

  }  // end of namespace check

}  // end of namespace tfel

#endif /* LIB_TFELCHECK_RELATIVEANDABSOLUTECOMPARISON_HXX */
