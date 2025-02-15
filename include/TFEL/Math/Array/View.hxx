/*!
 * \file  include/TFEL/Math/Array/View.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_VIEW_HXX
#define LIB_TFEL_MATH_ARRAY_VIEW_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/Forward/qt.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Array/IndexingPolicies.hxx"
#include "TFEL/Math/Array/MutableFixedSizeArrayBase.hxx"
#include "TFEL/Math/Array/MutableRuntimeArrayBase.hxx"
#include "TFEL/Math/Array/ConstFixedSizeArrayBase.hxx"
#include "TFEL/Math/Array/ConstRuntimeArrayBase.hxx"

namespace tfel::math::internals {

  template <typename T>
  struct MakeViewReference {
    //! \brief result of the metafunction
    using type = T&;
  };

  template <UnitConcept UnitType, typename ValueType, typename OwnershipPolicy>
  struct MakeViewReference<Quantity<UnitType, ValueType, OwnershipPolicy>> {
    //! \brief result of the metafunction
    using type = qt_ref<UnitType, ValueType>;
  };

  template <typename T>
  struct MakeConstViewReference {
    //! \brief result of the metafunction
    using type = const T&;
  };

  template <UnitConcept UnitType, typename ValueType, typename OwnershipPolicy>
  struct MakeConstViewReference<
      Quantity<UnitType, ValueType, OwnershipPolicy>> {
    //! \brief result of the metafunction
    using type = const_qt_ref<UnitType, ValueType>;
  };

}  // end of namespace tfel::math::internals

namespace tfel::math {

  // forward declaration
  template <typename MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy>
  struct View;

  //! \brief a standard array policy.
  template <typename ValueType, typename IndexingPolicyType>
  struct ViewArrayPolicy {
    //! \brief type of the values hold by the array.
    using value_type = ValueType;
    //! \brief const type of the values hold by the array.
    using const_value_type = const value_type;
    //! \brief type of a const reference to the value contained.
    using reference =
        typename tfel::math::internals::MakeViewReference<ValueType>::type;
    //! \brief type of a const reference to the value contained.
    using const_reference =
        typename tfel::math::internals::MakeConstViewReference<ValueType>::type;
    //! \brief type of the values hold by the array.
    using storage_type = std::
        conditional_t<isQuantity<ValueType>(), base_type<ValueType>, ValueType>;
    //!
    static constexpr auto isMakeConstReferenceTrivial =
        !isQuantity<ValueType>();
    //! \brief make a const_reference from a const reference to a base type
    static constexpr typename ViewArrayPolicy::const_reference
    make_const_reference(
        const typename ViewArrayPolicy::storage_type& v) noexcept {
      if constexpr (isQuantity<ValueType>()) {
        using const_qt_reference = typename ViewArrayPolicy::const_reference;
        return const_qt_reference{v};
      } else {
        return v;
      }
    }
    //!
    static constexpr auto isMakeReferenceTrivial = !isQuantity<ValueType>();
    //! \brief make a reference from a reference to a base type
    static constexpr typename ViewArrayPolicy::reference make_reference(
        typename ViewArrayPolicy::storage_type& v) noexcept {
      if constexpr (isQuantity<ValueType>()) {
        using qt_reference = typename ViewArrayPolicy::reference;
        return qt_reference{v};
      } else {
        return v;
      }
    }
    //! \brief the underlying indexing policy
    using IndexingPolicy = IndexingPolicyType;
  };

  //! \brief a simple alias
  template <typename MappedType, typename IndexingPolicyType>
  using selectViewArrayBase = std::conditional_t<
      std::is_const_v<MappedType>,
      std::conditional_t<
          IndexingPolicyType::hasFixedSizes,
          ConstFixedSizeArrayBase<
              View<MappedType, IndexingPolicyType>,
              ViewArrayPolicy<numeric_type<MappedType>, IndexingPolicyType>>,
          ConstRuntimeArrayBase<
              View<MappedType, IndexingPolicyType>,
              ViewArrayPolicy<numeric_type<MappedType>, IndexingPolicyType>>>,
      std::conditional_t<
          IndexingPolicyType::hasFixedSizes,
          MutableFixedSizeArrayBase<
              View<MappedType, IndexingPolicyType>,
              ViewArrayPolicy<numeric_type<MappedType>, IndexingPolicyType>>,
          MutableRuntimeArrayBase<
              View<MappedType, IndexingPolicyType>,
              ViewArrayPolicy<numeric_type<MappedType>, IndexingPolicyType>>>>;

  //! \brief
  template <typename MappedType>
  using ViewStorageType = std::conditional_t<
      isQuantity<numeric_type<std::remove_cv_t<MappedType>>>(),
      base_type<numeric_type<std::remove_cv_t<MappedType>>>,
      numeric_type<std::remove_cv_t<MappedType>>>;
  //! \brief
  template <typename MappedType>
  using ViewDataPointerType =
      std::conditional_t<std::is_const_v<MappedType>,
                         const ViewStorageType<MappedType>*,
                         ViewStorageType<MappedType>*>;
  //! \brief
  template <typename MappedType>
  using ViewConstDataPointerType = const ViewStorageType<MappedType>*;

  /*!
   * \brief view of an object from a continuous memory area
   * \tparam MappedType: type of the object mapped to the memory area
   */
  template <typename MappedType, typename IndexingPolicyType>
  struct View : ConceptRebind<typename ComputeObjectTag<MappedType>::type,
                              View<MappedType, IndexingPolicyType>>::type,
                selectViewArrayBase<MappedType, IndexingPolicyType> {
    //
    static_assert(
        checkIndexingPoliciesCompatiblity<typename MappedType::indexing_policy,
                                          IndexingPolicyType>(),
        "the indexing policy of the view is not compatible with the "
        "indexing policy of the mapped type");
    //
    static constexpr bool is_const = std::is_const_v<MappedType>;
    //
    static constexpr auto hasFixedSizes = IndexingPolicyType::hasFixedSizes;
    //! \brief a simple alias
    using data_pointer_type = ViewDataPointerType<MappedType>;
    //! \brief a simple alias
    using const_data_pointer_type = ViewConstDataPointerType<MappedType>;
    //! \brief default constructor
    explicit TFEL_HOST_DEVICE constexpr View(const data_pointer_type p) noexcept
        : data_pointer(p) {
      static_assert(hasFixedSizes, "invalid constructor call");
    }  // end of View
    //! \brief default constructor
    explicit TFEL_HOST_DEVICE constexpr View(
        const data_pointer_type p, const IndexingPolicyType& i) noexcept
        : selectViewArrayBase<MappedType, IndexingPolicyType>(i),
          data_pointer(p) {
      static_assert(!hasFixedSizes, "invalid constructor call");
    }  // end of View

    //! \brief copy constructor
    constexpr View(const View&) noexcept = default;
    //! \brief move constructor
    constexpr View(View&&) noexcept = default;
    //! \brief assignement operator
    TFEL_HOST_DEVICE constexpr View& operator=(const View& src) noexcept {
      static_assert(!is_const, "invalid constructor call");
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->assign(src);
      return *this;
    }
    //! \brief move assigment
    TFEL_HOST_DEVICE constexpr View& operator=(View&& src) noexcept {
      static_assert(!is_const, "invalid constructor call");
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->assign(src);
      return *this;
    }
    // exposing MutableFixedSizeArrayBase' assignement operators
    using selectViewArrayBase<MappedType, IndexingPolicyType>::operator=;
    using selectViewArrayBase<MappedType, IndexingPolicyType>::operator[];
    using selectViewArrayBase<MappedType, IndexingPolicyType>::operator();
    //! \return a pointer to the underlying array serving as element storage.
    TFEL_HOST_DEVICE constexpr data_pointer_type data() noexcept {
      static_assert(!is_const, "invalid call");
      return this->data_pointer;
    }  // end of data

    //! \return a pointer to the underlying array serving as element storage.
    TFEL_HOST_DEVICE constexpr const_data_pointer_type data() const noexcept {
      return this->data_pointer;
    }  // end of data

    /*!
     * \brief assign values values from an other array
     * \param[in] src: array to be assigned
     */
    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr View& operator=(const OtherArray& src) requires(
        isAssignableTo<OtherArray, MappedType>()) {
      static_assert(!is_const, "invalid call");
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->assign(src);
      return *this;
    }
    //
    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr View& operator+=(const OtherArray& src) requires(
        isAssignableTo<OtherArray, MappedType>()) {
      static_assert(!is_const, "invalid call");
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->addAndAssign(src);
      return *this;
    }
    //
    template <typename OtherArray>
    TFEL_HOST_DEVICE constexpr View& operator-=(const OtherArray& src) requires(
        isAssignableTo<OtherArray, MappedType>()) {
      static_assert(!is_const, "invalid call");
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->substractAndAssign(src);
      return *this;
    }
    //
    template <typename ValueType2>
    TFEL_HOST_DEVICE constexpr View&
    operator*=(const ValueType2& s) noexcept requires(
        isAssignableTo<
            BinaryOperationResult<ValueType2, numeric_type<MappedType>, OpMult>,
            numeric_type<MappedType>>()) {
      static_assert(!is_const, "invalid call");
      selectViewArrayBase<MappedType, IndexingPolicyType>::multiplyByScalar(s);
      return *this;
    }  // end of operator*=
       //
    template <typename ValueType2>
    TFEL_HOST_DEVICE constexpr View&
    operator/=(const ValueType2& s) noexcept requires(
        isAssignableTo<
            BinaryOperationResult<numeric_type<MappedType>, ValueType2, OpDiv>,
            numeric_type<MappedType>>()) {
      static_assert(!is_const, "invalid call");
      selectViewArrayBase<MappedType, IndexingPolicyType>::multiplyByScalar(1 /
                                                                            s);
      return *this;
    }  // end of operator/=

    //! \brief destructor
    ~View() noexcept = default;

   protected:
    //! \brief pointer to the memory buffer
    const data_pointer_type data_pointer;
  };  // end of struct View

  /*!
   * \brief partial specialisation of the `MathObjectTraits` for const views
   * \tparam MappedType: mapped type
   */
  template <typename MappedType, typename ArrayPolicy>
  struct MathObjectTraits<View<MappedType, ArrayPolicy>>
      : public MathObjectTraits<std::remove_cv_t<MappedType>> {
  };  // end of struct MathObjectTraits

  /*!
   * \brief partial specialisation of the `ResultOfEvaluation` class for
   * views.
   * \tparam MappedType: mapped type
   */
  template <typename MappedType, typename ArrayPolicy>
  struct ResultOfEvaluation<View<MappedType, ArrayPolicy>> {
    //! \brief result of the metafunction
    using type = std::remove_cv_t<MappedType>;
  };  // end of struct ResultOfEvaluation

  namespace internals {

    template <ScalarConcept ScalarType, bool isQt>
    struct ScalarViewImplementation {
      using type = std::conditional_t<std::is_const_v<ScalarType>,
                                      const ScalarType&,
                                      ScalarType&>;
    };

    template <ScalarConcept ScalarType>
    struct ScalarViewImplementation<ScalarType, true> {
      using type = std::conditional_t<
          std::is_const_v<ScalarType>,
          const qt_ref<
              typename QuantityTraits<std::decay_t<ScalarType>>::UnitType,
              typename QuantityTraits<std::decay_t<ScalarType>>::ValueType>,
          qt_ref<typename QuantityTraits<std::decay_t<ScalarType>>::UnitType,
                 typename QuantityTraits<std::decay_t<ScalarType>>::ValueType>>;
    };

  }  // namespace internals

  //! \brief view to a scalar
  template <ScalarConcept ScalarType>
  using scalar_view = typename internals::
      ScalarViewImplementation<ScalarType, isQuantity<ScalarType>()>::type;

  template <ScalarConcept ScalarType>
  TFEL_HOST_DEVICE constexpr scalar_view<ScalarType> map(
      base_type<ScalarType>* const p) {
    return scalar_view<ScalarType>(*p);
  }  // end of map

  template <ScalarConcept ScalarType>
  TFEL_HOST_DEVICE constexpr scalar_view<ScalarType> map(
      base_type<ScalarType>& v) {
    return scalar_view<ScalarType>(v);
  }  // end of map

  template <ScalarConcept ScalarType>
  TFEL_HOST_DEVICE constexpr scalar_view<const ScalarType> map(
      const base_type<ScalarType>* const p) {
    return scalar_view<const ScalarType>(*p);
  }  // end of map

  template <ScalarConcept ScalarType>
  TFEL_HOST_DEVICE constexpr scalar_view<const ScalarType> map(
      const base_type<ScalarType>& v) {
    return scalar_view<const ScalarType>(v);
  }  // end of map

  /*!
   * \brief return a view from a memory area
   * \tparam MappedType: object mapped
   * \param[in] p: pointer to the mapped memory area
   */
  template <typename MappedType,
            typename IndexingPolicyType = typename MappedType::indexing_policy>
  TFEL_HOST_DEVICE constexpr View<MappedType, IndexingPolicyType>
  map(const ViewDataPointerType<MappedType> p) requires(
      (!std::is_const_v<MappedType>)&&(!isScalar<MappedType>()) &&
      (std::remove_cv_t<MappedType>::hasFixedSizes)) {
    return View<MappedType, IndexingPolicyType>{p};
  }  // end of map

  template <typename MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy>
  TFEL_HOST_DEVICE constexpr View<const MappedType, IndexingPolicyType>
  map(const ViewConstDataPointerType<MappedType> p) requires(
      (!isScalar<MappedType>()) &&
      (std::remove_cv_t<MappedType>::indexing_policy::hasFixedSizes)) {
    return View<const MappedType, IndexingPolicyType>{p};
  }  // end of map

  /*!
   * \brief an helper metafunction which check of the last type of the type list
   * is convertible to a pointer to a memory area that can be used to create a
   * view.
   */
  template <typename MappedType, typename... Args>
  constexpr auto canMakeViewFromLastArgument() {
    using value_type = numeric_type<MappedType>;
    using last_type = tfel::math::internals::select_last<Args...>;
    return (std::is_convertible_v<last_type, value_type* const>);
  }  // end of canMakeViewFromLastArgument

  /*!
   * \brief an helper metafunction which check of the last type of the type list
   * is convertible to a const pointer to a memory area that can be used to
   * create a view.
   */
  template <typename MappedType, typename... Args>
  constexpr auto canMakeConstViewFromLastArgument() {
    using value_type = numeric_type<MappedType>;
    using last_type = tfel::math::internals::select_last<Args...>;
    return std::is_convertible_v<const last_type, const value_type* const>;
  }

  //! \brief return a view from a memory area
  template <typename MappedType,
            typename... Args,
            typename IndexingPolicyType = typename MappedType::indexing_policy>
  TFEL_HOST_DEVICE constexpr View<MappedType, IndexingPolicyType>
  map(Args&&... args) requires(
      (!std::is_const_v<MappedType>)&&(!IndexingPolicyType::hasFixedSizes) &&
      (checkIndexingPoliciesCompatiblity<
          IndexingPolicyType,
          typename MappedType::indexing_policy>()) &&
      (canMakeViewFromLastArgument<MappedType, Args...>())) {
    static_assert(sizeof...(Args) >= 2, "invalid call");
    const auto r =
        buildIndexingPolicyAndExtractPointerToData<IndexingPolicyType>(args...);
    return View<MappedType, IndexingPolicyType>{std::get<1>(r), std::get<0>(r)};
  }  // end of map

  //! \brief return a view from a memory area
  template <typename MappedType,
            typename... Args,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy>
  TFEL_HOST_DEVICE constexpr View<const MappedType, IndexingPolicyType>
  map(const Args&... args) requires(
      (!std::remove_cv_t<MappedType>::indexing_policy::hasFixedSizes) &&
      (checkIndexingPoliciesCompatiblity<
          IndexingPolicyType,
          typename std::remove_cv_t<MappedType>::indexing_policy>()) &&
      (canMakeConstViewFromLastArgument<std::remove_cv_t<MappedType>,
                                        Args...>())) {
    static_assert(sizeof...(Args) >= 2, "invalid call");
    const auto r =
        buildIndexingPolicyAndExtractPointerToData<IndexingPolicyType>(args...);
    return View<const MappedType, IndexingPolicyType>{std::get<1>(r),
                                                      std::get<0>(r)};
  }  // end of map

  template <typename MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy>
  using ConstView =
      View<const std::remove_cv_t<MappedType>, IndexingPolicyType>;

}  // end of namespace tfel::math

namespace tfel::typetraits {

  /*!
   * \brief generic partial specialisation for a view.
   * \tparam MathObject: object used to create the view to be assigned.
   * \tparam MathObject2: math object that will be assigned.
   */
  template <typename MathObject,
            typename IndexingPolicyType,
            typename MathObject2>
  struct IsAssignableTo<tfel::math::View<MathObject, IndexingPolicyType>,
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
            typename MathObject2,
            typename IndexingPolicyType2>
  struct IsAssignableTo<MathObject,
                        tfel::math::View<MathObject2, IndexingPolicyType2>> {
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
  template <typename MathObject,
            typename IndexingPolicyType,
            typename MathObject2,
            typename IndexingPolicyType2>
  struct IsAssignableTo<tfel::math::View<MathObject, IndexingPolicyType>,
                        tfel::math::View<MathObject2, IndexingPolicyType2>> {
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
            typename MathObject,
            typename IndexingPolicyType>
  struct IsAssignableTo<tfel::math::Expr<EvaluationResult, Operation>,
                        tfel::math::View<MathObject, IndexingPolicyType>> {
    //!
    static constexpr bool is_const = std::is_const_v<MathObject>;
    //! \brief result
    static constexpr bool cond =
        (!is_const) &&
        isAssignableTo<EvaluationResult, std::remove_cv_t<MathObject>>();
  };  // end of struct IsAssignableTo

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_ARRAY_VIEW_HXX */
