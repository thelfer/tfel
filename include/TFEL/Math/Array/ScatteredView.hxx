/*!
 * \file  include/TFEL/Math/Array/ScatteredView.hxx
 * \brief
 * \author Thomas Helfer
 * \date 24/12/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_SCATTEREDVIEW_HXX
#define LIB_TFEL_MATH_ARRAY_SCATTEREDVIEW_HXX

#include <span>
#include <array>
#include <utility>
#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/Forward/qt.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Array/IndexingPolicies.hxx"
#include "TFEL/Math/Array/MultipleIndicesFunctors.hxx"
#include "TFEL/Math/Array/ConstFixedSizeArrayBase.hxx"
#include "TFEL/Math/Array/ConstRuntimeArrayBase.hxx"
#include "TFEL/Math/Array/View.hxx"

namespace tfel::math {

  //
  template <typename MathObjectType>
  concept MappableMutableMathObjectUsingScatteredViewConcept =
      MathObjectConcept<MathObjectType> &&
      MathObjectType::indexing_policy::hasFixedSizes;

  template <typename MathObjectType>
  concept MappableImmutableMathObjectUsingScatteredViewConcept =
      std::is_const_v<MathObjectType> &&
      MathObjectConcept<std::remove_cv_t<MathObjectType>> &&
      MathObjectType::indexing_policy::hasFixedSizes;

  template <typename MathObjectType>
  concept MappableMathObjectUsingScatteredViewConcept =
      MappableMutableMathObjectUsingScatteredViewConcept<MathObjectType> ||
      MappableImmutableMathObjectUsingScatteredViewConcept<MathObjectType>;

  /*!
   * \brief access policy storing one pointer per component of the mapped
   * object
   */
  template <MappableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType>
  struct ScatteredAccessPolicy {
    //
    static inline constexpr auto N = [] {
      typename MappedType::indexing_policy p;
      return p.size();
    }();
    //! \brief default constructor
    explicit TFEL_HOST_DEVICE constexpr ScatteredAccessPolicy(
        const std::array<ViewDataPointerType<MappedType>, N>& p) noexcept
        : ptrs([&p] {
            return [&p]<std::size_t... Is>(
                std::integer_sequence<std::size_t, Is...>) {
              return std::array<ViewDataPointerType<MappedType>, N>{p[Is]...};
            }
            (std::make_index_sequence<N>());
          }()) {}
    //! \brief default constructor
    explicit TFEL_HOST_DEVICE constexpr ScatteredAccessPolicy(
        const std::span<const ViewDataPointerType<MappedType>, N>& p) noexcept
        : ptrs([&p] {
            return [&p]<std::size_t... Is>(
                std::integer_sequence<std::size_t, Is...>) {
              return std::array<ViewDataPointerType<MappedType>, N>{p[Is]...};
            }
            (std::make_index_sequence<N>());
          }()) {}
    //! \brief copy constructor
    constexpr ScatteredAccessPolicy(const ScatteredAccessPolicy&) noexcept =
        default;
    //! \brief move constructor
    constexpr ScatteredAccessPolicy(ScatteredAccessPolicy&&) noexcept = default;

   protected:
    [[nodiscard]] TFEL_HOST_DEVICE constexpr auto& getValue(
        const typename IndexingPolicyType::size_type i) noexcept {
      return *(this->ptrs[i]);
    }
    [[nodiscard]] TFEL_HOST_DEVICE constexpr const auto& getValue(
        const typename IndexingPolicyType::size_type i) const noexcept {
      return *(this->ptrs[i]);
    }
    //! \brief pointer to the memory buffer
    std::array<ViewDataPointerType<MappedType>, N> ptrs;
  };

  /*!
   * \brief view of an object whose components are not stored contiguously
   * \tparam MappedType: type of the object mapped
   * \tparam IndexingPolicyType: indexing policy used by the view
   * \tparam AccessPolicy: policy giving access to the component
   * associated with an index
   */
  template <MappableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType,
            typename AccessPolicy>
  struct AccessPolicyBasedView
      : ConceptRebind<
            typename ComputeObjectTag<std::remove_cv_t<MappedType>>::type,
            AccessPolicyBasedView<std::remove_cv_t<MappedType>,
                                  IndexingPolicyType,
                                  AccessPolicy>>::type,
        AccessPolicy,
        IndexingPolicyType {
    //
    static_assert(
        checkIndexingPoliciesCompatiblity<typename MappedType::indexing_policy,
                                          IndexingPolicyType>(),
        "the indexing policy of the view is not compatible with the "
        "indexing policy of the mapped type");
    //
    static inline constexpr auto N = [] {
      IndexingPolicyType p;
      return p.size();
    }();
    //! \brief exposing the array policy
    using array_policy =
        ViewArrayPolicy<numeric_type<MappedType>, IndexingPolicyType>;
    //! \brief exposing the indexing policy
    using indexing_policy = IndexingPolicyType;
    //
    using size_type = typename IndexingPolicyType::size_type;
    //
    static constexpr bool is_const = std::is_const_v<MappedType>;
    //
    static constexpr auto hasFixedSizes = IndexingPolicyType::hasFixedSizes;
    //! \brief a simple alias
    using data_pointer_type = ViewDataPointerType<MappedType>;
    //! \brief a simple alias
    using const_data_pointer_type = ViewConstDataPointerType<MappedType>;
    //
    using AccessPolicy::AccessPolicy;
    //! \brief copy constructor
    constexpr AccessPolicyBasedView(const AccessPolicyBasedView&) noexcept =
        default;
    //! \brief move constructor
    constexpr AccessPolicyBasedView(AccessPolicyBasedView&&) noexcept = default;
    //! \return the current indexing policy
    constexpr const auto& getIndexingPolicy() const {
      return static_cast<const IndexingPolicyType&>(*this);
    }

    TFEL_HOST_DEVICE constexpr typename array_policy::const_reference
    operator[](const typename IndexingPolicyType::size_type i) const noexcept {
      static_assert(IndexingPolicyType::arity == 1u, "invalid call");
      if constexpr (array_policy::isMakeConstReferenceTrivial) {
        return this->getValue(this->getIndex(i));
      } else {
        return array_policy::make_const_reference(
            this->getValue(this->getIndex(i)));
      }
    }  // end of operator[]

    TFEL_HOST_DEVICE constexpr typename array_policy::reference operator[](
        const typename IndexingPolicyType::size_type i) noexcept
        requires(!is_const) {
      static_assert(IndexingPolicyType::arity == 1u, "invalid call");
      if constexpr (array_policy::isMakeReferenceTrivial) {
        return this->getValue(this->getIndex(i));
      } else {
        return array_policy::make_reference(this->getValue(this->getIndex(i)));
      }
    }  // end of operator[]

    template <typename... Indices>
    TFEL_HOST_DEVICE constexpr typename array_policy::const_reference
    operator()(const Indices... i) const noexcept {
      checkIndicesValiditity<IndexingPolicyType, Indices...>();
      if constexpr (array_policy::isMakeConstReferenceTrivial) {
        return this->getValue(this->getIndex(static_cast<size_type>(i)...));
      } else {
        return array_policy::make_const_reference(
            this->getValue(this->getIndex(static_cast<size_type>(i)...)));
      }
    }  // end of operator()

    TFEL_HOST_DEVICE constexpr typename array_policy::const_reference
    operator()(const std::array<typename IndexingPolicyType::size_type,
                                IndexingPolicyType::arity>& i) const noexcept {
      if constexpr (array_policy::isMakeConstReferenceTrivial) {
        return this->getValue(this->getIndex(i));
      } else {
        return array_policy::make_const_reference(
            this->getValue(this->getIndex(i)));
      }
    }  // end of operator()

    template <typename... Indices>
    TFEL_HOST_DEVICE constexpr typename array_policy::reference operator()(
        const Indices... i) noexcept requires(!is_const) {
      checkIndicesValiditity<IndexingPolicyType, Indices...>();
      if constexpr (array_policy::isMakeReferenceTrivial) {
        return this->getValue(this->getIndex(static_cast<size_type>(i)...));
      } else {
        return array_policy::make_reference(
            this->getValue(this->getIndex(static_cast<size_type>(i)...)));
      }
    }  // end of operator()

    TFEL_HOST_DEVICE constexpr typename array_policy::reference operator()(
        const std::array<typename IndexingPolicyType::size_type,
                         IndexingPolicyType::arity>& i) noexcept
        requires(!is_const) {
      if constexpr (array_policy::isMakeReferenceTrivial) {
        return this->getValue(this->getIndex(i));
      } else {
        return array_policy::make_reference(this->getValue(this->getIndex(i)));
      }
    }  // end of operator()

    //! \brief assignement operator
    TFEL_HOST_DEVICE constexpr AccessPolicyBasedView& operator=(
        const AccessPolicyBasedView& src) noexcept {
      static_assert(!is_const, "invalid constructor call");
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->assign(src);
      return *this;
    }  // end of operator =

    /*!
     * \brief assign values values from an other array
     * \param[in] src: array to be assigned
     */
    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr AccessPolicyBasedView&
    operator=(const OtherArray& src) requires(
        isAssignableTo<OtherArray, MappedType>()) {
      static_assert(!is_const, "invalid call");
      //             checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //              src.getIndexingPolicy());
      this->assign(src);
      return *this;
    }
    //
    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr AccessPolicyBasedView&
    operator+=(const OtherArray& src) requires(
        isAssignableTo<OtherArray, MappedType>()) {
      static_assert(!is_const, "invalid call");
      //            checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                     src.getIndexingPolicy());
      this->addAndAssign(src);
      return *this;
    }
    //
    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr AccessPolicyBasedView&
    operator-=(const OtherArray& src) requires(
        isAssignableTo<OtherArray, MappedType>()) {
      static_assert(!is_const, "invalid call");
      //            checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                     src.getIndexingPolicy());
      this->substractAndAssign(src);
      return *this;
    }
    //
    template <typename ValueType2>
    TFEL_HOST_DEVICE constexpr AccessPolicyBasedView&
    operator*=(const ValueType2& s) noexcept requires(
        isAssignableTo<
            BinaryOperationResult<ValueType2, numeric_type<MappedType>, OpMult>,
            numeric_type<MappedType>>()) {
      static_assert(!is_const, "invalid call");
      const auto f = makeMultiIndicesUnaryOperatorFunctor(
          [s](typename array_policy::reference a) { a *= s; }, *this);
      this->iterate(f);
      return *this;
    }  // end of operator*=
       //
    template <typename ValueType2>
    TFEL_HOST_DEVICE constexpr AccessPolicyBasedView&
    operator/=(const ValueType2& s) noexcept requires(
        isAssignableTo<
            BinaryOperationResult<numeric_type<MappedType>, ValueType2, OpDiv>,
            numeric_type<MappedType>>()) {
      static_assert(!is_const, "invalid call");
      const auto f = makeMultiIndicesUnaryOperatorFunctor(
          [s](typename array_policy::reference a) { a /= s; }, *this);
      this->iterate(f);
      return *this;
    }  // end of operator/=

    //! \brief destructor
    ~AccessPolicyBasedView() noexcept = default;

   protected:
    template <typename FunctorType>
    TFEL_HOST_DEVICE constexpr void iterate(const FunctorType& f) const {
      const auto g = [](size_type i) constexpr {
        IndexingPolicyType p;
        return p.size(i);
      };
      if constexpr (IndexingPolicyType::unRollLoop) {
        IterateOverMultipleIndices<size_type, 0, IndexingPolicyType::arity,
                                   true>::exe(f, g);
      } else {
        IterateOverMultipleIndices<size_type, 0, IndexingPolicyType::arity,
                                   false>::exe(f, g);
      }
    }  // end of iterate

    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr void assign(const OtherArray& src) noexcept {
      const auto f = makeMultiIndicesBinaryOperatorFunctor(
          [](typename array_policy::reference a, const auto& b) { a = b; },
          *this, src);
      this->iterate(f);
    }  // end of assign

    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr void addAndAssign(
        const OtherArray& src) noexcept {
      const auto f = makeMultiIndicesBinaryOperatorFunctor(
          [](typename array_policy::reference a, const auto& b) { a += b; },
          *this, src);
      this->iterate(f);
    }  // end of addAndAssign

    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr void substractAndAssign(
        const OtherArray& src) noexcept {
      const auto f = makeMultiIndicesBinaryOperatorFunctor(
          [](typename array_policy::reference a, const auto& b) { a -= b; },
          *this, src);
      this->iterate(f);
    }  // end of substractAndAssign
  };   // end of struct AccessPolicyBasedView

  //! \brief view storing one pointer per component of the mapped object
  template <MappableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType = typename MappedType::indexing_policy>
  using ScatteredView = AccessPolicyBasedView<
      MappedType,
      IndexingPolicyType,
      ScatteredAccessPolicy<MappedType, IndexingPolicyType>>;

  template <MappableMutableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy>
  using ConstScatteredView =
      ScatteredView<const std::remove_cv_t<MappedType>, IndexingPolicyType>;

  /*!
   * \brief partial specialisation of the `MathObjectTraits` for const views
   * \tparam MappedType: mapped type
   */
  template <MappableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType,
            typename AccessPolicy>
  struct MathObjectTraits<
      AccessPolicyBasedView<MappedType, IndexingPolicyType, AccessPolicy>>
      : public MathObjectTraits<std::remove_cv_t<MappedType>> {
  };  // end of struct MathObjectTraits

  /*!
   * \brief partial specialisation of the `ResultOfEvaluation` class for
   * views.
   * \tparam MappedType: mapped type
   */
  template <MappableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType,
            typename AccessPolicy>
  struct ResultOfEvaluation<
      AccessPolicyBasedView<MappedType, IndexingPolicyType, AccessPolicy>> {
    //! \brief result of the metafunction
    using type = std::remove_cv_t<MappedType>;
  };  // end of struct ResultOfEvaluation

  /*!
   * \brief return a scattered view from a list of pointers
   * \tparam MappedType: object mapped
   * \param[in] p: pointers
   */
  template <MappableMutableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType = typename MappedType::indexing_policy>
  TFEL_HOST_DEVICE constexpr ScatteredView<MappedType, IndexingPolicyType> map(
      std::span<const ViewDataPointerType<MappedType>,
                indexing_policy_size<IndexingPolicyType>> ptrs)  //
      requires((!std::is_const_v<MappedType>)&&(
          std::remove_cv_t<MappedType>::hasFixedSizes)) {
    return ScatteredView<MappedType, IndexingPolicyType>{ptrs};
  }  // end of map

  /*!
   * \brief return an immutable scattered view from a list of pointers
   * \tparam MappedType: object mapped
   * \param[in] p: pointers
   */
  template <MappableImmutableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy>
  TFEL_HOST_DEVICE constexpr ScatteredView<MappedType, IndexingPolicyType> map(
      std::span<const ViewConstDataPointerType<std::remove_cv_t<MappedType>>,
                indexing_policy_size<IndexingPolicyType>> ptrs)  //
      requires((std::remove_cv_t<MappedType>::indexing_policy::hasFixedSizes)) {
    return ScatteredView<MappedType, IndexingPolicyType>{ptrs};
  }  // end of map

  /*!
   * \brief return a scattered view from a list of pointers
   * \tparam MappedType: object mapped
   * \param[in] p: pointers
   */
  template <MappableMutableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType = typename MappedType::indexing_policy>
  TFEL_HOST_DEVICE constexpr ScatteredView<MappedType, IndexingPolicyType> map(
      std::array<ViewDataPointerType<MappedType>,
                 indexing_policy_size<IndexingPolicyType>>& ptrs)  //
      requires((!std::is_const_v<MappedType>)&&(
          std::remove_cv_t<MappedType>::hasFixedSizes)) {
    return ScatteredView<MappedType, IndexingPolicyType>{ptrs};
  }  // end of map

  /*!
   * \brief return an immutable scattered view from a list of pointers
   * \tparam MappedType: object mapped
   * \param[in] p: pointers
   */
  template <MappableImmutableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy>
  TFEL_HOST_DEVICE constexpr ScatteredView<MappedType, IndexingPolicyType> map(
      const std::array<ViewConstDataPointerType<std::remove_cv_t<MappedType>>,
                       indexing_policy_size<IndexingPolicyType>>& ptrs)  //
      requires((std::remove_cv_t<MappedType>::indexing_policy::hasFixedSizes)) {
    return ScatteredView<MappedType, IndexingPolicyType>{ptrs};
  }  // end of map

}  // end of namespace tfel::math

namespace tfel::typetraits {

  /*!
   * \brief generic partial specialisation for a view.
   * \tparam MathObject: object used to create the view to be assigned.
   * \tparam MathObject2: math object that will be assigned.
   */
  template <tfel::math::MappableMathObjectUsingScatteredViewConcept MathObject,
            typename IndexingPolicyType,
            typename AccessPolicy,
            typename MathObject2>
  struct IsAssignableTo<tfel::math::AccessPolicyBasedView<MathObject,
                                                          IndexingPolicyType,
                                                          AccessPolicy>,
                        MathObject2> {
    //! \brief result
    static constexpr bool cond =
        isAssignableTo<std::remove_cv_t<MathObject>, MathObject2>();
  };  // end of struct IsAssignableTo

  /*!
   * \brief generic partial specialisation for a view.
   * \tparam MathObject: object to be assigned.
   * \tparam MathObject2: math object used to create the view that will be
   * assigned.
   */
  template <typename MathObject,
            tfel::math::MappableMathObjectUsingScatteredViewConcept MathObject2,
            typename IndexingPolicyType2,
            typename AccessPolicy2>
  struct IsAssignableTo<MathObject,
                        tfel::math::AccessPolicyBasedView<MathObject2,
                                                          IndexingPolicyType2,
                                                          AccessPolicy2>> {
    //!
    static constexpr bool is_const = std::is_const_v<MathObject2>;
    //! \brief result
    static constexpr bool cond =
        (!is_const) &&
        isAssignableTo<MathObject, std::remove_cv_t<MathObject2>>();
  };  // end of struct IsAssignableTo

  /*!
   * \brief generic partial specialisation for a view.
   * \tparam MathObject: object to be assigned.
   * \tparam MathObject2: math object used to create the view that will be
   * assigned.
   */
  template <tfel::math::MappableMathObjectUsingScatteredViewConcept MathObject,
            typename IndexingPolicyType,
            typename AccessPolicy,
            tfel::math::MappableMathObjectUsingScatteredViewConcept MathObject2,
            typename IndexingPolicyType2,
            typename AccessPolicy2>
  struct IsAssignableTo<tfel::math::AccessPolicyBasedView<MathObject,
                                                          IndexingPolicyType,
                                                          AccessPolicy>,
                        tfel::math::AccessPolicyBasedView<MathObject2,
                                                          IndexingPolicyType2,
                                                          AccessPolicy2>> {
    //!
    static constexpr bool is_const = std::is_const_v<MathObject2>;
    //! \brief result
    static constexpr bool cond =
        ((!is_const) &&
         (tfel::math::checkIndexingPoliciesCompatiblity<
             IndexingPolicyType,
             IndexingPolicyType2>()) &&
         (isAssignableTo<MathObject, std::remove_cv_t<MathObject2>>()));
  };  // end of struct IsAssignableTo

  //! \brief specialisation of IsAssignableTo
  template <typename EvaluationResult,
            typename Operation,
            tfel::math::MappableMathObjectUsingScatteredViewConcept MathObject,
            typename IndexingPolicyType,
            typename AccessPolicy>
  struct IsAssignableTo<tfel::math::Expr<EvaluationResult, Operation>,
                        tfel::math::AccessPolicyBasedView<MathObject,
                                                          IndexingPolicyType,
                                                          AccessPolicy>> {
    //!
    static constexpr bool is_const = std::is_const_v<MathObject>;
    //! \brief result
    static constexpr bool cond =
        (!is_const) &&
        isAssignableTo<EvaluationResult, std::remove_cv_t<MathObject>>();
  };  // end of struct IsAssignableTo

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_ARRAY_SCATTEREDVIEW_HXX */
