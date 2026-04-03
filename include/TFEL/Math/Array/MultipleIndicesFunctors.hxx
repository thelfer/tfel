/*!
 * \file  include/TFEL/Math/Array/MultipleIndicesFunctors.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 30 août 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_MULTIPLEINDICESFUNCTORS_HXX
#define LIB_TFEL_MATH_ARRAY_MULTIPLEINDICESFUNCTORS_HXX

namespace tfel::math {

  template <typename UnaryOperator, typename FirstArgument>
  struct MultiIndicesUnaryOperatorFunctor {
    TFEL_HOST_DEVICE constexpr MultiIndicesUnaryOperatorFunctor(
        FirstArgument& a)
        : first_argument(a) {}  // end of MultiIndicesUnaryOperatorFunctor
    TFEL_HOST_DEVICE constexpr MultiIndicesUnaryOperatorFunctor(
        const UnaryOperator& o, FirstArgument& a)
        : unary_operator(o),
          first_argument(a) {}  // end of MultiIndicesUnaryOperatorFunctor

    template <typename... Indices>
    TFEL_HOST_DEVICE constexpr void operator()(const Indices... indices) const {
      unary_operator(this->first_argument(indices...));
    }  // end of operator()
   private:
    UnaryOperator unary_operator;
    FirstArgument& first_argument;
  };

  template <typename UnaryOperator, typename FirstArgument>
  TFEL_HOST_DEVICE constexpr MultiIndicesUnaryOperatorFunctor<UnaryOperator,
                                                              FirstArgument>
  makeMultiIndicesUnaryOperatorFunctor(FirstArgument& a) {
    return MultiIndicesUnaryOperatorFunctor<UnaryOperator, FirstArgument>(a);
  }  // end of makeMultiIndicesUnaryOperatorFunctor

  template <typename UnaryOperator, typename FirstArgument>
  TFEL_HOST_DEVICE constexpr MultiIndicesUnaryOperatorFunctor<UnaryOperator,
                                                              FirstArgument>
  makeMultiIndicesUnaryOperatorFunctor(const UnaryOperator& o,
                                       FirstArgument& a) {
    return MultiIndicesUnaryOperatorFunctor<UnaryOperator, FirstArgument>(o, a);
  }  // end of makeMultiIndicesUnaryOperatorFunctor

  template <typename BinaryOperator,
            typename FirstArgument,
            typename SecondArgument>
  struct MultiIndicesBinaryOperatorFunctor {
    TFEL_HOST_DEVICE constexpr MultiIndicesBinaryOperatorFunctor(
        FirstArgument& a, SecondArgument& b)
        : first_argument(a),
          second_argument(b) {}  // end of MultiIndicesBinaryOperatorFunctor
    TFEL_HOST_DEVICE constexpr MultiIndicesBinaryOperatorFunctor(
        const BinaryOperator& o, FirstArgument& a, const SecondArgument& b)
        : binary_operator(o),
          first_argument(a),
          second_argument(b) {}  // end of MultiIndicesBinaryOperatorFunctor

    template <typename... Indices>
    TFEL_HOST_DEVICE constexpr void operator()(const Indices... indices) const {
      binary_operator(this->first_argument(indices...),
                      this->second_argument(indices...));
    }  // end of operator()
   private:
    BinaryOperator binary_operator;
    FirstArgument& first_argument;
    const SecondArgument& second_argument;
  };

  template <typename BinaryOperator,
            typename FirstArgument,
            typename SecondArgument>
  TFEL_HOST_DEVICE constexpr MultiIndicesBinaryOperatorFunctor<BinaryOperator,
                                                               FirstArgument,
                                                               SecondArgument>
  makeMultiIndicesBinaryOperatorFunctor(FirstArgument& a, SecondArgument& b) {
    return MultiIndicesBinaryOperatorFunctor<BinaryOperator, FirstArgument,
                                             SecondArgument>(a, b);
  }  // end of makeMultiIndicesBinaryOperatorFunctor

  template <typename BinaryOperator,
            typename FirstArgument,
            typename SecondArgument>
  TFEL_HOST_DEVICE constexpr MultiIndicesBinaryOperatorFunctor<BinaryOperator,
                                                               FirstArgument,
                                                               SecondArgument>
  makeMultiIndicesBinaryOperatorFunctor(const BinaryOperator& o,
                                        FirstArgument& a,
                                        const SecondArgument& b) {
    return MultiIndicesBinaryOperatorFunctor<BinaryOperator, FirstArgument,
                                             SecondArgument>(o, a, b);
  }  // end of makeMultiIndicesBinaryOperatorFunctor

  template <typename IndexingPolicy, typename RandomAccessIterator>
  struct MultiIndicesRandomAccessInteratorWrapper : public IndexingPolicy {
    static_assert(std::is_same_v<typename std::iterator_traits<
                                     RandomAccessIterator>::iterator_category,
                                 std::random_access_iterator_tag>);
    //! \brief a simple alias
    using const_reference =
        const typename std::iterator_traits<RandomAccessIterator>::reference;
    //! \brief default constructor
    TFEL_HOST_DEVICE constexpr MultiIndicesRandomAccessInteratorWrapper(
        const IndexingPolicy& policy, const RandomAccessIterator& p) noexcept
        : IndexingPolicy(policy), iterator(p) {}
    constexpr MultiIndicesRandomAccessInteratorWrapper(
        const MultiIndicesRandomAccessInteratorWrapper&) noexcept = default;
    constexpr MultiIndicesRandomAccessInteratorWrapper(
        MultiIndicesRandomAccessInteratorWrapper&&) noexcept = default;
    constexpr MultiIndicesRandomAccessInteratorWrapper& operator=(
        const MultiIndicesRandomAccessInteratorWrapper&) noexcept = default;
    constexpr MultiIndicesRandomAccessInteratorWrapper& operator=(
        MultiIndicesRandomAccessInteratorWrapper&&) noexcept = default;
    /*!
     * \brief access iterator
     */
    template <typename... Indices>
    TFEL_HOST_DEVICE constexpr const_reference operator()(
        const Indices... i) const noexcept {
      checkIndicesValiditity<IndexingPolicy, Indices...>();
      return this->iterator[this->getIndex(i...)];
    }
    //! \brief destructor
    ~MultiIndicesRandomAccessInteratorWrapper() = default;

   private:
    //! \brief underlying iterator
    const RandomAccessIterator iterator;
  };

}  // namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_MULTIPLEINDICESFUNCTORS_HXX */
