/*!
 * \file   include/TFEL/Math/Array/ViewsArrayIO.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21/05/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
                                            ViewIndexingPolicyType>&) {
    return os;
  }  // end of operator<<

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_VIEWSARRAYIO_HXX */
