/*!
 * \file MixedComparison.hxx
 *
 *  Created on: 28 mai 2013
 *      Author: rp238441
 *  \class MixedComparison
 *  \brief Class that does a mixed comparison between two columns.
 *
 *  Succeeds if absoluteError < relativePrecision * value - absolutePrecision
 *  where relativePrecision is prec and absolutePrecision is precision2.
 *
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_MIXEDCOMPARISON_HH_
#define LIB_TFELCHECK_MIXEDCOMPARISON_HH_

#include "TFELCheck/TFELCheckConfig.hxx"
#include "TFELCheck/Comparison.hxx"

namespace tfel_check {

  struct TFELCHECK_VISIBILITY_EXPORT MixedComparison final : public Comparison {
    //! constructor
    MixedComparison();
    virtual void compare() override;
    //! destructor
    virtual ~MixedComparison();
  };

} /* namespace tfel_check */

#endif /* LIB_TFELCHECK_MIXEDCOMPARISON_HH_ */
