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

#include "TFELCheck/TFELCheckConfig.hxx"
#include "TFELCheck/Comparison.hxx"

namespace tfel_check {

  struct TFELCHECK_VISIBILITY_EXPORT RelativeComparison final
      : public Comparison {
    RelativeComparison();
    virtual void compare() override;
    virtual ~RelativeComparison();
  };

} /* namespace tfel_check */

#endif /* LIB_TFELCHECK_RELATIVECOMPARISON_HXX */
