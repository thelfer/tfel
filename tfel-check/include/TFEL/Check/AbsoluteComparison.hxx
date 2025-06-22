/*!
 * \file AbsoluteComparison.hxx
 *
 *  Created on: 27 mai 2013
 *      Author: rp238441
 *
 *  \class AbsoluteComparison
 *
 *  \brief class that does an absolute comparison between two columns
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_ABSOLUTECOMPARISON_HXX
#define LIB_TFELCHECK_ABSOLUTECOMPARISON_HXX

#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/Comparison.hxx"

namespace tfel {

  namespace check {

    struct TFELCHECK_VISIBILITY_EXPORT AbsoluteComparison : public Comparison {
      AbsoluteComparison();
      virtual void compare() override;
      virtual ~AbsoluteComparison();
    };

  }  // end of namespace check

}  // end of namespace tfel

#endif /* LIB_TFELCHECK_ABSOLUTECOMPARISON_HXX */
