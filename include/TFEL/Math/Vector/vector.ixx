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
#include <algorithm>

namespace tfel::math {

  //!
  template <typename ValueType>
  vector<ValueType>::vector(const typename vector::size_type s,
                            const ValueType& v) {
    this->resize(s, v);
  }  // end of vector

  template <typename ValueType>
  template <typename InputIterator>
  vector<ValueType>::vector(const InputIterator b, const InputIterator e) {
    this->insert(this->begin(), b, e);
  }

  template <typename ValueType>
  template <typename InputIterator>
  void vector<ValueType>::copy(const InputIterator b, const InputIterator e) {
    std::copy(b, e, this->v);
  }

  //   template <typename T>
  //   void vector<T>::swap(vector<T>& a) {
  // #ifndef TFEL_NO_RUNTIME_CHECK_BOUNDS
  //     RunTimeCheck<RunTimeProperties>::exe(this->size(), a.size());
  // #endif /* LIB_TFEL_VECTORIXX */
  //     std::vector<T>::swap(a);
  //   }

  template <typename ValueType>
  void vector<ValueType>::push_back(const ValueType& v) {
    using IndexingPolicy = typename vector::indexing_policy;
    this->data_values.push_back(v);
    static_cast<IndexingPolicy&>(*this) =
        IndexingPolicy{this->data_values.size()};
  }  // end of push_back

  template <typename ValueType>
  void vector<ValueType>::resize(const typename vector::size_type s,
                                 const ValueType& v) {
    using IndexingPolicy = typename vector::indexing_policy;
    this->data_values.resize(s, v);
    static_cast<IndexingPolicy&>(*this) =
        IndexingPolicy{this->data_values.size()};
  }  // end of resize

  template <typename ValueType>
  template <typename InputIterator>
  void vector<ValueType>::insert(const typename vector::const_iterator p,
                                 const InputIterator b,
                                 const InputIterator e) {
    using IndexingPolicy = typename vector::indexing_policy;
    const auto pos = p - this->begin();
    this->data_values.insert(this->data_values.begin() + pos, b, e);
    static_cast<IndexingPolicy&>(*this) =
        IndexingPolicy{this->data_values.size()};
  }

  template <typename ValueType>
  void vector<ValueType>::swap(vector<ValueType>& v) {
    using IndexingPolicy = typename vector::indexing_policy;
    this->data_values.swap(v.data_values);
    static_cast<IndexingPolicy&>(*this) =
        IndexingPolicy{this->data_values.size()};
    static_cast<IndexingPolicy&>(v) = IndexingPolicy{v.data_values.size()};
  }  // end of swap

  template <typename ValueType>
  vector<ValueType>::~vector() noexcept = default;

  template <ScalarConcept ValueType>
  TFEL_HOST_DEVICE auto norm(const vector<ValueType>& vec) noexcept {
    auto n = ValueType{} * ValueType{};
    for (const auto& v : vec) {
      n += v * v;
    }
    return power<1, 2>(real(n));
  }  // end of norm

}  // end of namespace tfel::math

#include "TFEL/Math/Vector/vectorResultType.hxx"

#endif /* LIB_TFEL_MATH_VECTOR_IXX */
