/*!
 * \file RelativeComparison.hxx
 *
 *  Created on: 28 mai 2013
 *      Author: rp238441
 *
 *  \brief class that does a relative comparison between two columns
 *  \class RelativeComparison
 *
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_RELATIVECOMPARISON_HXX
#define LIB_TFELCHECK_RELATIVECOMPARISON_HXX

#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/Comparison.hxx"

namespace tfel {

  namespace check {

    struct TFELCHECK_VISIBILITY_EXPORT RelativeComparison final
        : public Comparison {
      RelativeComparison();
      void compare() override;
      ~RelativeComparison() override;
    };

  }  // end of namespace check

}  // end of namespace tfel

#endif /* LIB_TFELCHECK_RELATIVECOMPARISON_HXX */
