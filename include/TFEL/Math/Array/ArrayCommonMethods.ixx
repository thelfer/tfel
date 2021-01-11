/*!
 * \file  include/TFEL/Math/Array/ArrayCommonMethods.ixx
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

#ifndef LIB_TFEL_MATH_ARRAY_ARRAYCOMMONMETHODS_IXX
#define LIB_TFEL_MATH_ARRAY_ARRAYCOMMONMETHODS_IXX

#include "TFEL/ContractViolation.hxx"

namespace tfel::math {

  template <typename Child, typename ArrayPolicy>
  constexpr typename ArrayPolicy::const_reference
      ConstArrayCommonMethods<Child, ArrayPolicy>::operator[](
          const typename ArrayPolicy::IndexingPolicy::size_type i) const
      noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::arity == 1u, "invalid call");
    const auto& child = static_cast<const Child&>(*this);
    const auto* const d = child.data();
    return d[child.getIndex(i)];
  }  // end of operator[]

  template <typename Child, typename ArrayPolicy>
  template <typename... Indices>
  constexpr typename ArrayPolicy::const_reference
  ConstArrayCommonMethods<Child, ArrayPolicy>::operator()(
      const Indices... i) const noexcept {
    checkIndicesValiditity<typename ArrayPolicy::IndexingPolicy, Indices...>();
    const auto& child = static_cast<const Child&>(*this);
    const auto* const d = child.data();
    return d[child.getIndex(i...)];
  }  // end of operator[]

  template <typename Child, typename ArrayPolicy>
  constexpr typename ArrayPolicy::reference
      MutableArrayCommonMethods<Child, ArrayPolicy>::operator[](
          const typename ArrayPolicy::IndexingPolicy::size_type i) noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::arity == 1u, "invalid call");
    auto& child = static_cast<Child&>(*this);
    auto* const d = child.data();
    return d[child.getIndex(i)];
  }  // end of operator[]

  template <typename Child, typename ArrayPolicy>
  template <typename... Indices>
  constexpr typename ArrayPolicy::reference
  MutableArrayCommonMethods<Child, ArrayPolicy>::operator()(
      const Indices... i) noexcept {
    checkIndicesValiditity<typename ArrayPolicy::IndexingPolicy, Indices...>();
    auto& child = static_cast<Child&>(*this);
    auto* const d = child.data();
    return d[child.getIndex(i...)];
  }

  template <typename Child, typename ArrayPolicy>
  template <typename OtherArray>
  constexpr void MutableArrayCommonMethods<Child, ArrayPolicy>::assign(
      const OtherArray& src) noexcept {
    auto& child = static_cast<Child&>(*this);
    const auto f = makeMultiIndicesBinaryOperatorFunctor(
        [](auto& a, const auto& b) { a = b; }, child, src);
    child.iterate(f);
  }  // end of MutableArrayCommonMethods<Child, ArrayPolicy>

  template <typename Child, typename ArrayPolicy>
  template <typename OtherArray>
  constexpr void MutableArrayCommonMethods<Child, ArrayPolicy>::addAndAssign(
      const OtherArray& src) noexcept {
    auto& child = static_cast<Child&>(*this);
    const auto f = makeMultiIndicesBinaryOperatorFunctor(
        [](auto& a, const auto& b) { a += b; }, child, src);
    child.iterate(f);
  }  // end of MutableArrayCommonMethods<Child, ArrayPolicy>

  template <typename Child, typename ArrayPolicy>
  template <typename OtherArray>
  constexpr void
  MutableArrayCommonMethods<Child, ArrayPolicy>::substractAndAssign(
      const OtherArray& src) noexcept {
    auto& child = static_cast<Child&>(*this);
    const auto f = makeMultiIndicesBinaryOperatorFunctor(
        [](auto& a, const auto& b) { a -= b; }, child, src);
    child.iterate(f);
  }  // end of MutableArrayCommonMethods<Child, ArrayPolicy>

  template <typename Child, typename ArrayPolicy>
  template <typename ValueType>
  constexpr std::enable_if_t<
      isAssignableTo<ValueType, typename ArrayPolicy::value_type>(),
      Child&>
  MutableArrayCommonMethods<Child, ArrayPolicy>::operator=(
      const std::initializer_list<ValueType>& values) noexcept {
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

  template <typename Child, typename ArrayPolicy>
  template <typename ImportIndexingPolicy, typename InputIterator>
  constexpr void MutableArrayCommonMethods<Child, ArrayPolicy>::import(
      const ImportIndexingPolicy& policy,
      const InputIterator p,
      const InputIterator pe) {
    auto& child = static_cast<Child&>(*this);
    const auto rsize =
        static_cast<typename ArrayPolicy::IndexingPolicy::size_type>(pe - p);
    if (child.size() != rsize) {
      tfel::reportContractViolation(
          "MutableArrayCommonMethods::import: invalid sequence size");
    }
    const auto src =
        MultiIndicesRandomAccessInteratorWrapper<ImportIndexingPolicy,
                                                 InputIterator>{policy, p};
    const auto f = makeMultiIndicesBinaryOperatorFunctor(
        [](auto& a, const auto& b) { a = b; }, child, src);
    child.iterate(f);
  }  // end of import

  template <typename Child, typename ArrayPolicy>
  template <typename ValueType2>
  constexpr std::enable_if_t<
      isAssignableTo<ValueType2, typename ArrayPolicy::value_type>(),
      void>
  MutableArrayCommonMethods<Child, ArrayPolicy>::fill(const ValueType2& v) {
    const auto f =
        makeMultiIndicesUnaryOperatorFunctor([v](auto& a) { a = v; }, *this);
    auto& child = static_cast<Child&>(*this);
    child.iterate(f);
  }  // end of fill

  template <typename Child, typename ArrayPolicy>
  template <typename ValueType2>
  constexpr std::enable_if_t<
      isAssignableTo<BinaryOperationResult<ValueType2,
                                           typename ArrayPolicy::value_type,
                                           OpMult>,
                     typename ArrayPolicy::value_type>(),
      void>
  MutableArrayCommonMethods<Child, ArrayPolicy>::multiplyByScalar(
      const ValueType2& s) {
    const auto f =
        makeMultiIndicesUnaryOperatorFunctor([s](auto& a) { a *= s; }, *this);
    auto& child = static_cast<Child&>(*this);
    child.iterate(f);
  }  // end of multiplyByScalar

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_ARRAYCOMMONMETHODS_IXX */
