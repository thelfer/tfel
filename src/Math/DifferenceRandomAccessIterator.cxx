/*!
 * \file   DifferenceRandomAccessIterator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 avril 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/DifferenceRandomAccessIterator.hxx"

namespace tfel::math {

  void
  DifferenceRandomAccessIteratorBase::throwInconsistentIteratorDifference() {
    tfel::raise(
        "DifferenceRandomAccessIteratorBase::"
        "throwInconsistentIteratorDifference: "
        "inconsistent iterator difference");
  }  // end of DifferenceRandomAccessIteratorBase::throwNullStrideException

}  // end of namespace tfel::math
