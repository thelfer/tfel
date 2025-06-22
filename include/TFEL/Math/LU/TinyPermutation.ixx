/*!
 * \file   include/TFEL/Math/LU/TinyPermutation.ixx
 *
 * \brief
 * \author Helfer Thomas
 * \date   01 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYPERMUTATION_I_
#define LIB_TFEL_MATH_TINYPERMUTATION_I_

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N>
    TinyPermutation<N>::TinyPermutation() : is_identity(true) {
      using value_type = unsigned short;
      tfel::fsalgo::iota<N>::exe(this->begin(), value_type{0u});
    }

    template <unsigned short N>
    void TinyPermutation<N>::swap(const unsigned short i,
                                  const unsigned short j) {
      std::swap(tvector<N, unsigned short>::operator[](i),
                tvector<N, unsigned short>::operator[](j));
      this->is_identity = false;
    }

    template <unsigned short N>
    bool TinyPermutation<N>::isIdentity() const {
      return this->is_identity;
    }

    template <unsigned short N>
    template <typename T>
    void TinyPermutation<N>::exe(tvector<N, T>& v) const {
      const tvector<N, T> tmp{v};
      for (unsigned short i = 0; i < N; ++i) {
        v(i) = tmp(*(this->v + i));
      }
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TINYPERMUTATION_I_ */
