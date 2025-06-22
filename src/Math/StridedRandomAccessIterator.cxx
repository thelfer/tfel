/*!
 * \file   StridedRandomAccessIterator.cxx
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
#include "TFEL/Math/General/StridedRandomAccessIterator.hxx"

namespace tfel::math {

  void StridedRandomAccessIteratorBase::throwNullStrideException() {
    raise(
        "StridedRandomAccessIteratorBase::"
        "throwNullStrideException: "
        "null stride");
  }  // end of StridedRandomAccessIteratorBase::throwNullStrideException

  void StridedRandomAccessIteratorBase::throwUmatchedStrideException() {
    raise(
        "StridedRandomAccessIteratorBase::"
        "throwUmatchedStrideException: "
        "unmatched stride");
  }  // end of StridedRandomAccessIteratorBase::throwUmatchedStrideException

}  // end of namespace tfel::math
