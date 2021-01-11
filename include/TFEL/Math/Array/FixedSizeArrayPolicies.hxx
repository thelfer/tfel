/*!
 * \file  include/TFEL/Math/Array/FixedSizeArrayPolicies.hxx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYPOLICIES_HXX
#define LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYPOLICIES_HXX

#include "TFEL/Math/Array/ArrayPolicies.hxx"
#include "TFEL/Math/Array/FixedSizeIndexingPolicies.hxx"

namespace tfel::math {

  /*!
   * \brief a array policy based on the fixed size vector indexing policy
   */
  template <unsigned short N, typename ValueType>
  struct FixedSizeVectorPolicy : StandardArrayPolicyAliases<ValueType> {
    //! \brief the underlying indexing policy
    using IndexingPolicy = FixedSizeVectorIndexingPolicy<unsigned short, N>;
  };

  /*!
   * \brief a array policy describing a fixed size matrix stored in row major
   * format based on the fixed size matrix indexing policy.
   */
  template <unsigned short N, unsigned short M, typename ValueType>
  struct FixedSizeRowMajorMatrixPolicy : StandardArrayPolicyAliases<ValueType> {
    //! \brief the underlying indexing policy
    using IndexingPolicy =
        FixedSizeRowMajorMatrixIndexingPolicy<unsigned short, N, M>;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYPOLICIES_HXX */
