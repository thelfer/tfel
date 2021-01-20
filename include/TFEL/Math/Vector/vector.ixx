/*!
 * \file   include/TFEL/Math/Vector/vector.ixx
 * \brief
 * \author Thomas Helfer
 * \date   05 oct 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_VECTOR_IXX
#define LIB_TFEL_MATH_VECTOR_IXX

#include <cassert>

namespace tfel::math {

  template <typename T>
  template <typename InputIterator>
  void vector<T>::copy(const InputIterator b, const InputIterator e) {
    std::copy(b, e, this->v);
  }

  //   template <typename T>
  //   void vector<T>::swap(vector<T>& a) {
  // #ifndef NO_RUNTIME_CHECK_BOUNDS
  //     RunTimeCheck<RunTimeProperties>::exe(this->size(), a.size());
  // #endif /* LIB_TFEL_VECTORIXX */
  //     std::vector<T>::swap(a);
  //   }

  template <typename T>
  vector<T>::~vector() noexcept = default;

  template <typename T>
  std::enable_if_t<isScalar<T>(),
                   typename tfel::typetraits::RealPartType<T>::type>
  norm(const vector<T>& vec) {
    auto n = T{} * T{};
    for (const auto& v : vec) {
      n += v * v;
    }
    return std::sqrt(real(n));
  }  // end of norm

}  // end of namespace tfel::math

#include "TFEL/Math/Vector/vectorResultType.hxx"

#endif /* LIB_TFEL_MATH_VECTOR_IXX */
