/*!
 * \file   include/TFEL/Math/Array/StridedCoalescedViewsArrayIO.hxx
 * \brief
 * \author Tristan Chenaille
 * \date   15/01/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STRIDEDCOALESCEDVIEWSARRAYIO_HXX
#define LIB_TFEL_MATH_STRIDEDCOALESCEDVIEWSARRAYIO_HXX

#include <ostream>
#include "TFEL/Math/Array/StridedCoalescedViewsArray.hxx"

namespace tfel::math {

  template <typename MappedType,
            typename MemoryIndexingPolicyType,
            typename ViewIndexingPolicyType>
  std::ostream& operator<<(
      std::ostream& os,
      const StridedCoalescedViewsArray<MappedType,
                                       MemoryIndexingPolicyType,
                                       ViewIndexingPolicyType>& a) {
    using StridedCoalescedViewsArrayType =
        StridedCoalescedViewsArray<MappedType, MemoryIndexingPolicyType,
                                   ViewIndexingPolicyType>;
    if constexpr (StridedCoalescedViewsArrayType::arity == 1u) {
      os << "[";
      for (decltype(a.size()) i = 0; i != a.size(); ++i) {
        os << " " << a(i);
      }
      os << " ]";
    } else if constexpr (StridedCoalescedViewsArrayType::arity == 2u) {
      os << "[";
      for (decltype(a.size(0)) i = 0; i != a.size(0);) {
        os << "[";
        for (decltype(a.size(1)) j = 0; j != a.size(1); ++j) {
          os << " " << a(i, j);
        }
        if (++i != a.size(0)) {
          os << " ], ";
        } else {
          os << " ]";
        }
      }
      os << " ]";
    } else {
      os << "[unsupported StridedCoalescedViewsArray object (arity greater "
            "than 2)]";
    }
    return os;
  }  // end of operator<<

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STRIDEDCOALESCEDVIEWSARRAYIO_HXX */
