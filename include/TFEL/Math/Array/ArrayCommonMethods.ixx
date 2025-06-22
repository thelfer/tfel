/*!
 * \file  include/TFEL/Math/Array/ArrayCommonMethods.ixx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_ARRAYCOMMONMETHODS_IXX
#define LIB_TFEL_MATH_ARRAY_ARRAYCOMMONMETHODS_IXX

#include "TFEL/ContractViolation.hxx"

namespace tfel::math {

  template <typename Child, typename ArrayPolicyType>
  constexpr typename ArrayPolicyType::const_reference
  ConstArrayCommonMethods<Child, ArrayPolicyType>::operator[](
      const typename ArrayPolicyType::IndexingPolicy::size_type i)
      const noexcept {
    static_assert(ArrayPolicyType::IndexingPolicy::arity == 1u, "invalid call");
    const auto& child = static_cast<const Child&>(*this);
    const auto* const d = child.data();
    if constexpr (ArrayPolicyType::isMakeConstReferenceTrivial) {
      return d[child.getIndex(i)];
    } else {
      return ArrayPolicyType::make_const_reference(d[child.getIndex(i)]);
    }
  }  // end of operator[]

  template <typename Child, typename ArrayPolicyType>
  constexpr typename ArrayPolicyType::const_reference
  ConstArrayCommonMethods<Child, ArrayPolicyType>::operator[](
      const std::array<typename ArrayPolicyType::IndexingPolicy::size_type,
                       ArrayPolicyType::IndexingPolicy::arity>& i)
      const noexcept {
    const auto& child = static_cast<const Child&>(*this);
    const auto* const d = child.data();
    if constexpr (ArrayPolicyType::isMakeConstReferenceTrivial) {
      return d[child.getIndex(i)];
    } else {
      return ArrayPolicyType::make_const_reference(d[child.getIndex(i)]);
    }
  }  // end of operator[]

  template <typename Child, typename ArrayPolicyType>
  template <typename... Indices>
  constexpr typename ArrayPolicyType::const_reference
  ConstArrayCommonMethods<Child, ArrayPolicyType>::operator()(
      const Indices... i) const noexcept {
    checkIndicesValiditity<typename ArrayPolicyType::IndexingPolicy,
                           Indices...>();
    using size_type = typename ArrayPolicyType::IndexingPolicy::size_type;
    const auto& child = static_cast<const Child&>(*this);
    const auto* const d = child.data();
    if constexpr (ArrayPolicyType::isMakeConstReferenceTrivial) {
      return d[child.getIndex(static_cast<size_type>(i)...)];
    } else {
      return ArrayPolicyType::make_const_reference(
          d[child.getIndex(static_cast<size_type>(i)...)]);
    }
  }  // end of operator[]

  template <typename Child, typename ArrayPolicyType>
  constexpr typename ArrayPolicyType::const_reference
  ConstArrayCommonMethods<Child, ArrayPolicyType>::operator()(
      const std::array<typename ArrayPolicyType::IndexingPolicy::size_type,
                       ArrayPolicyType::IndexingPolicy::arity>& i)
      const noexcept {
    const auto& child = static_cast<const Child&>(*this);
    const auto* const d = child.data();
    if constexpr (ArrayPolicyType::isMakeConstReferenceTrivial) {
      return d[child.getIndex(i)];
    } else {
      return ArrayPolicyType::make_const_reference(d[child.getIndex(i)]);
    }
  }  // end of operator()

  template <typename Child, typename ArrayPolicyType>
  constexpr typename ArrayPolicyType::reference
  MutableArrayCommonMethods<Child, ArrayPolicyType>::operator[](
      const typename ArrayPolicyType::IndexingPolicy::size_type i) noexcept {
    static_assert(ArrayPolicyType::IndexingPolicy::arity == 1u, "invalid call");
    auto& child = static_cast<Child&>(*this);
    auto* const d = child.data();
    if constexpr (ArrayPolicyType::isMakeReferenceTrivial) {
      return d[child.getIndex(i)];
    } else {
      return ArrayPolicyType::make_reference(d[child.getIndex(i)]);
    }
  }  // end of operator[]

  template <typename Child, typename ArrayPolicyType>
  constexpr typename ArrayPolicyType::reference
  MutableArrayCommonMethods<Child, ArrayPolicyType>::operator[](
      const std::array<typename ArrayPolicyType::IndexingPolicy::size_type,
                       ArrayPolicyType::IndexingPolicy::arity>& i) noexcept {
    auto& child = static_cast<Child&>(*this);
    auto* const d = child.data();
    if constexpr (ArrayPolicyType::isMakeReferenceTrivial) {
      return d[child.getIndex(i)];
    } else {
      return ArrayPolicyType::make_reference(d[child.getIndex(i)]);
    }
  }  // end of operator[]

  template <typename Child, typename ArrayPolicyType>
  template <typename... Indices>
  constexpr typename ArrayPolicyType::reference
  MutableArrayCommonMethods<Child, ArrayPolicyType>::operator()(
      const Indices... i) noexcept {
    using size_type = typename ArrayPolicyType::IndexingPolicy::size_type;
    checkIndicesValiditity<typename ArrayPolicyType::IndexingPolicy,
                           Indices...>();
    auto& child = static_cast<Child&>(*this);
    auto* const d = child.data();
    if constexpr (ArrayPolicyType::isMakeReferenceTrivial) {
      return d[child.getIndex(static_cast<size_type>(i)...)];
    } else {
      return ArrayPolicyType::make_reference(
          d[child.getIndex(static_cast<size_type>(i)...)]);
    }
  }  // end of operator()

  template <typename Child, typename ArrayPolicyType>
  constexpr typename ArrayPolicyType::reference
  MutableArrayCommonMethods<Child, ArrayPolicyType>::operator()(
      const std::array<typename ArrayPolicyType::IndexingPolicy::size_type,
                       ArrayPolicyType::IndexingPolicy::arity>& i) noexcept {
    auto& child = static_cast<Child&>(*this);
    auto* const d = child.data();
    if constexpr (ArrayPolicyType::isMakeReferenceTrivial) {
      return d[child.getIndex(i)];
    } else {
      return ArrayPolicyType::make_reference(d[child.getIndex(i)]);
    }
  }  // end of operator()

  template <typename Child, typename ArrayPolicyType>
  template <typename OtherArray>
  constexpr void MutableArrayCommonMethods<Child, ArrayPolicyType>::assign(
      const OtherArray& src) noexcept {
    auto& child = static_cast<Child&>(*this);
    const auto f = makeMultiIndicesBinaryOperatorFunctor(
        [](typename ArrayPolicyType::reference a, const auto& b) { a = b; },
        child, src);
    child.iterate(f);
  }  // end of MutableArrayCommonMethods<Child, ArrayPolicyType>

  template <typename Child, typename ArrayPolicyType>
  template <typename OtherArray>
  constexpr void
  MutableArrayCommonMethods<Child, ArrayPolicyType>::addAndAssign(
      const OtherArray& src) noexcept {
    auto& child = static_cast<Child&>(*this);
    const auto f = makeMultiIndicesBinaryOperatorFunctor(
        [](typename ArrayPolicyType::reference a, const auto& b) { a += b; },
        child, src);
    child.iterate(f);
  }  // end of MutableArrayCommonMethods<Child, ArrayPolicyType>

  template <typename Child, typename ArrayPolicyType>
  template <typename OtherArray>
  constexpr void
  MutableArrayCommonMethods<Child, ArrayPolicyType>::substractAndAssign(
      const OtherArray& src) noexcept {
    auto& child = static_cast<Child&>(*this);
    const auto f = makeMultiIndicesBinaryOperatorFunctor(
        [](typename ArrayPolicyType::reference a, const auto& b) { a -= b; },
        child, src);
    child.iterate(f);
  }  // end of MutableArrayCommonMethods<Child, ArrayPolicyType>

  template <typename Child, typename ArrayPolicyType>
  template <typename ValueType>
  constexpr Child& MutableArrayCommonMethods<Child, ArrayPolicyType>::operator=(
      const std::initializer_list<ValueType>& values) noexcept
      requires(
          isAssignableTo<ValueType, typename ArrayPolicyType::value_type>()) {
    auto& child = static_cast<Child&>(*this);
    if (values.size() != child.size()) {
      tfel::reportContractViolation(
          "FixedSizeMatrixIndexingPolicy: "
          "invalid dimension");
    }
    const auto& policy = child.getRowMajorIndexingPolicy();
    this->import(policy, values.begin(), values.end());
    return child;
  }  // end of operator=

  template <typename Child, typename ArrayPolicyType>
  template <typename ImportIndexingPolicy, typename InputIterator>
  constexpr void MutableArrayCommonMethods<Child, ArrayPolicyType>::import(
      const ImportIndexingPolicy& policy,
      const InputIterator p,
      const InputIterator pe) {
    auto& child = static_cast<Child&>(*this);
    const auto rsize =
        static_cast<typename ArrayPolicyType::IndexingPolicy::size_type>(pe -
                                                                         p);
    if (child.size() != rsize) {
      tfel::reportContractViolation(
          "MutableArrayCommonMethods::import: invalid sequence size");
    }
    const auto src =
        MultiIndicesRandomAccessInteratorWrapper<ImportIndexingPolicy,
                                                 InputIterator>{policy, p};
    const auto f = makeMultiIndicesBinaryOperatorFunctor(
        [](typename ArrayPolicyType::reference a, const auto& b) { a = b; },
        child, src);
    child.iterate(f);
  }  // end of import

  template <typename Child, typename ArrayPolicyType>
  template <typename ValueType2>
  constexpr void MutableArrayCommonMethods<Child, ArrayPolicyType>::
      fill(const ValueType2& v) requires(
          isAssignableTo<ValueType2, typename ArrayPolicyType::value_type>()) {
    const auto f = makeMultiIndicesUnaryOperatorFunctor(
        [v](typename ArrayPolicyType::reference a) { a = v; }, *this);
    auto& child = static_cast<Child&>(*this);
    child.iterate(f);
  }  // end of fill

  template <typename Child, typename ArrayPolicyType>
  template <typename ValueType2, typename ValueType3>
  constexpr void MutableArrayCommonMethods<Child, ArrayPolicyType>::clamp(
      const ValueType2& lower_bound,
      const ValueType3& upper_bound)  //
      requires(
          isAssignableTo<ValueType2, typename ArrayPolicyType::value_type>() &&
          isAssignableTo<ValueType3, typename ArrayPolicyType::value_type>()) {
    const auto f = makeMultiIndicesUnaryOperatorFunctor(
        [lower_bound, upper_bound](typename ArrayPolicyType::reference a) {
          if (a < lower_bound) {
            a = lower_bound;
          } else if (a > upper_bound) {
            a = upper_bound;
          }
        },
        *this);
    auto& child = static_cast<Child&>(*this);
    child.iterate(f);
  }  // end of fill

  template <typename Child, typename ArrayPolicyType>
  template <typename ValueType2>
  constexpr void
  MutableArrayCommonMethods<Child, ArrayPolicyType>::multiplyByScalar(
      const ValueType2& s)  //
      requires(isAssignableTo<
               BinaryOperationResult<ValueType2,
                                     typename ArrayPolicyType::value_type,
                                     OpMult>,
               typename ArrayPolicyType::value_type>()) {
    const auto f = makeMultiIndicesUnaryOperatorFunctor(
        [s](typename ArrayPolicyType::reference a) { a *= s; }, *this);
    auto& child = static_cast<Child&>(*this);
    child.iterate(f);
  }  // end of multiplyByScalar

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_ARRAYCOMMONMETHODS_IXX */
