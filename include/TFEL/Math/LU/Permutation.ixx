/*!
 * \file   include/TFEL/Math/LU/Permutation.ixx
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

#ifndef LIB_TFEL_MATH_PERMUTATION_IXX
#define LIB_TFEL_MATH_PERMUTATION_IXX

#include <algorithm>

#include "TFEL/Math/MathException.hxx"

namespace tfel {

  namespace math {

    template <typename T>
    Permutation<T>::Permutation(const typename vector<T>::size_type n)
        : vector<T>(n) {
      this->reset();
    }

    template <typename T>
    bool Permutation<T>::isIdentity() const {
      return this->is_identity;
    }  // end of Permutation<T>::isIdendity

    template <typename T>
    void Permutation<T>::swap(const typename vector<T>::size_type i,
                              const typename vector<T>::size_type j) {
      std::swap(vector<T>::operator[](i), vector<T>::operator[](j));
      this->is_identity = false;
    }

    template <typename T>
    void Permutation<T>::reset() {
      for (decltype(this->size()) i = 0; i != this->size(); ++i) {
        vector<T>::operator[](i) = i;
      }
      this->is_identity = true;
    }

    template <typename T>
    void Permutation<T>::resize(const typename vector<T>::size_type s) {
      vector<T>::resize(s);
      this->reset();
    }

    template <typename T>
    Permutation<T>::~Permutation() {}  // end of Permutation<T>::~Permutation

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_PERMUTATION_IXX */
