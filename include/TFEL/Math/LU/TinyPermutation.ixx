/*!
 * \file   include/TFEL/Math/LU/TinyPermutation.ixx
 *
 * \brief
 * \author Thomas Helfer
 * \date   01 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYPERMUTATION_IXX
#define LIB_TFEL_MATH_TINYPERMUTATION_IXX

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

namespace tfel::math {

  template <unsigned short N>
  constexpr TinyPermutation<N>::TinyPermutation() : is_identity(true) {
    typename tvector<N, unsigned short>::value_type s{0u};
    tfel::fsalgo::iota<N>::exe(this->begin(), s);
  }

  template <unsigned short N>
  constexpr void TinyPermutation<N>::swap(const unsigned short i,
                                          const unsigned short j) {
    auto tmp = tvector<N, unsigned short>::operator[](i);
    tvector<N, unsigned short>::operator[](i) =
        tvector<N, unsigned short>::operator[](j);
    tvector<N, unsigned short>::operator[](j) = tmp;
    // std::swap(tvector<N, unsigned short>::operator[](i),
    //           tvector<N, unsigned short>::operator[](j));
    this->is_identity = false;
  }

  template <unsigned short N>
  constexpr bool TinyPermutation<N>::isIdentity() const {
    return this->is_identity;
  }

  template <unsigned short N>
  template <typename T>
  constexpr void TinyPermutation<N>::exe(tvector<N, T>& v) const {
    const tvector<N, T> tmp{v};
    for (unsigned short i = 0; i < N; ++i) {
      v(i) = tmp(*(this->v + i));
    }
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYPERMUTATION_IXX */
