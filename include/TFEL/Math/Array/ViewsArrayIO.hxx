/*!
 * \file   include/TFEL/Math/Array/ViewsArrayIO.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21/05/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_VIEWSARRAYIO_HXX
#define LIB_TFEL_MATH_VIEWSARRAYIO_HXX

#include <ostream>
#include "TFEL/Math/Array/ViewsArray.hxx"

namespace tfel::math {

  template <typename MappedType,
            typename MemoryIndexingPolicyType,
            typename ViewIndexingPolicyType>
  std::ostream& operator<<(std::ostream& os,
                           const ViewsArray<MappedType,
                                            MemoryIndexingPolicyType,
                                            ViewIndexingPolicyType>& a) {
    using ViewsArrayType = ViewsArray<MappedType, MemoryIndexingPolicyType,
                                      ViewIndexingPolicyType>;
    if constexpr (ViewsArrayType::arity == 1u) {
      os << "[";
      for (decltype(a.size()) i = 0; i != a.size(); ++i) {
        os << " " << a(i);
      }
      os << " ]";
    } else if constexpr (ViewsArrayType::arity == 2u) {
      os << "[";
      for (decltype(a.size(0)) i = 0; i != a.size(0);) {
        os << "[";
        for (decltype(a.size(1)) j = 0; j != a.size(1); ++j) {
          os << " " << a(i);
        }
        if (++i != a.size(0)) {
          os << " ], ";
        } else {
          os << " ]";
        }
      }
      os << " ]";
    } else {
      os << "[unsupported ViewsArray object (arity greater than 2)]";
    }
    return os;
  }  // end of operator<<

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_VIEWSARRAYIO_HXX */
