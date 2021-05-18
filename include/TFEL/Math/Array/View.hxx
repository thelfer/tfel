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
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Array/MutableFixedSizeArrayBase.hxx"
#include "TFEL/Math/Array/MutableRuntimeArrayBase.hxx"
#include "TFEL/Math/Array/ConstFixedSizeArrayBase.hxx"
#include "TFEL/Math/Array/ConstRuntimeArrayBase.hxx"

namespace tfel::math {

  // forward declaration
  template <typename MappedType>
  struct View;

  //! \brief a simple alias
  template <typename MappedType>
  using selectViewArrayBase = std::conditional_t<
      std::is_const_v<MappedType>,
      std::conditional_t<MappedType::indexing_policy::hasFixedSizes,
                         ConstFixedSizeArrayBase<View<MappedType>,
                                                 typename std::remove_cv_t<
                                                     MappedType>::array_policy>,
                         ConstRuntimeArrayBase<View<MappedType>,
                                               typename std::remove_cv_t<
                                                   MappedType>::array_policy>>,
      std::conditional_t<
          MappedType::indexing_policy::hasFixedSizes,
          MutableFixedSizeArrayBase<
              View<MappedType>,
              typename std::remove_cv_t<MappedType>::array_policy>,
          MutableRuntimeArrayBase<
              View<MappedType>,
              typename std::remove_cv_t<MappedType>::array_policy>>>;

  /*!
   * \brief view of an object from a continuous memory area
   * \tparam MappedType: type of the object mapped to the memory area
   */
  template <typename MappedType>
  struct View : ConceptRebind<typename ComputeObjectTag<MappedType>::type,
                              View<MappedType>>::type,
                selectViewArrayBase<MappedType> {
    //
    static constexpr bool is_const = std::is_const_v<MappedType>;
    //! \brief a simple alias
    using RawMappedType = std::remove_cv_t<MappedType>;
    //! \brief a simple alias
    using IndexingPolicy = typename RawMappedType::indexing_policy;
    //
    using data_pointer_type =
        std::conditional_t<is_const,
                           const numeric_type<MappedType>*,
                           numeric_type<MappedType>*>;
    //
    using const_data_pointer_type = const numeric_type<MappedType>*;
    //
    static constexpr auto hasFixedSizes = IndexingPolicy::hasFixedSizes;
    //

    //! \brief default constructor
    explicit constexpr View(const data_pointer_type p) noexcept
        : data_pointer(p) {
      static_assert(hasFixedSizes, "invalid constructor call");
    }  // end of View
    //! \brief default constructor
    constexpr View(const IndexingPolicy& i, const data_pointer_type p) noexcept
        : selectViewArrayBase<MappedType>(i), data_pointer(p) {
      static_assert(!hasFixedSizes, "invalid constructor call");
    }  // end of View
    //! \brief copy constructor
    constexpr View(const View&) noexcept = default;
    //! \brief move constructor
    constexpr View(View&&) noexcept = default;
    //! \brief assignement operator
    constexpr View& operator=(const View& src) noexcept {
      static_assert(!is_const, "invalid constructor call");
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->assign(src);
      return *this;
    }
    //! \brief move assigment
    constexpr View& operator=(View&& src) noexcept {
      static_assert(!is_const, "invalid constructor call");
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->assign(src);
      return *this;
    }
    // exposing MutableFixedSizeArrayBase' assignement operators
    using selectViewArrayBase<MappedType>::operator=;
    using selectViewArrayBase<MappedType>::operator[];
    using selectViewArrayBase<MappedType>::operator();
    //! \return a pointer to the underlying array serving as element storage.
    constexpr data_pointer_type data() noexcept {
      static_assert(!is_const, "invalid call");
      return this->data_pointer;
    }  // end of data

    //! \return a pointer to the underlying array serving as element storage.
    constexpr const_data_pointer_type data() const noexcept {
      return this->data_pointer;
    }  // end of data

    /*!
     * \brief assign values values from an other array
     * \param[in] src: array to be assigned
     */
    template <typename OtherArray>
    constexpr std::enable_if_t<isAssignableTo<OtherArray, MappedType>(), View&>
    operator=(const OtherArray& src) {
      static_assert(!is_const, "invalid call");
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->assign(src);
      return *this;
    }
    //
    template <typename OtherArray>
    constexpr std::enable_if_t<isAssignableTo<OtherArray, MappedType>(), View&>
    operator+=(const OtherArray& src) {
      static_assert(!is_const, "invalid call");
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->addAndAssign(src);
      return *this;
    }
    //
    template <typename OtherArray>
    constexpr std::enable_if_t<isAssignableTo<OtherArray, MappedType>(), View&>
    operator-=(const OtherArray& src) {
      static_assert(!is_const, "invalid call");
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->substractAndAssign(src);
      return *this;
    }
    //
    template <typename ValueType2>
    constexpr std::enable_if_t<
        isAssignableTo<
            BinaryOperationResult<ValueType2, numeric_type<MappedType>, OpMult>,
            numeric_type<MappedType>>(),
        View&>
    operator*=(const ValueType2& s) noexcept {
      static_assert(!is_const, "invalid call");
      selectViewArrayBase<MappedType>::multiplyByScalar(s);
      return *this;
    }  // end of operator*=
       //
    template <typename ValueType2>
    constexpr std::enable_if_t<
        isAssignableTo<
            BinaryOperationResult<numeric_type<MappedType>,ValueType2, OpDiv>,
            numeric_type<MappedType>>(),
        View&>
    operator/=(const ValueType2& s) noexcept {
      static_assert(!is_const, "invalid call");
      selectViewArrayBase<MappedType>::multiplyByScalar(1 / s);
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
  template <typename MappedType>
  struct MathObjectTraits<View<MappedType>>
      : public MathObjectTraits<std::remove_cv_t<MappedType>> {
  };  // end of struct MathObjectTraits

  /*!
   * \brief partial specialisation of the `ResultOfEvaluation` class for
   * views.
   * \tparam MappedType: mapped type
   */
  template <typename MappedType>
  struct ResultOfEvaluation<View<MappedType>> {
    //! \brief result of the metafunction
    using type = std::remove_cv_t<MappedType>;
  };  // end of struct ResultOfEvaluation

  /*!
   * \brief return a view from a memory area
   * \tparam MappedType: object mapped
   * \param[in] p: pointer to the mapped memory area
   */
  template <typename MappedType>
  constexpr std::enable_if_t<((!std::is_const_v<MappedType>)&&(
                                 MappedType::indexing_policy::hasFixedSizes)),
                             View<MappedType>>
  map(numeric_type<MappedType>* const p) {
    return View<MappedType>{p};
  }  // end of map

  template <typename MappedType>
  constexpr std::enable_if_t<
      std::remove_cv_t<MappedType>::indexing_policy::hasFixedSizes,
      View<const MappedType>>
  map(const numeric_type<MappedType>* const p) {
    return View<const MappedType>{p};
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

  /*!
   * \brief return a view from a memory area
   */
  template <typename MappedType, typename... Args>
  constexpr std::enable_if_t<
      ((!std::is_const_v<MappedType>)&&(
           !MappedType::indexing_policy::hasFixedSizes) &&
       (canMakeViewFromLastArgument<MappedType, Args...>())),
      View<MappedType>>
  map(Args&&... args) {
    static_assert(sizeof...(Args) >= 2, "invalid call");
    using IndexingPolicy = typename MappedType::indexing_policy;
    const auto r =
        buildIndexingPolicyAndExtractPointerToData<IndexingPolicy>(args...);
    return View<MappedType>{std::get<0>(r), std::get<1>(r)};
  }  // end of map

  template <typename MappedType, typename... Args>
  constexpr std::enable_if_t<
      ((!std::remove_cv_t<MappedType>::indexing_policy::hasFixedSizes) &&
       (canMakeConstViewFromLastArgument<std::remove_cv_t<MappedType>,
                                         Args...>())),
      View<const MappedType>>
  map(const Args&... args) {
    static_assert(sizeof...(Args) >= 2, "invalid call");
    using IndexingPolicy = typename MappedType::indexing_policy;
    const auto r =
        buildIndexingPolicyAndExtractPointerToData<IndexingPolicy>(args...);
    return View<const MappedType>{std::get<0>(r), std::get<1>(r)};
  }  // end of map

  template <typename MappedType>
  using ConstView = View<const std::remove_cv_t<MappedType>>;

}  // end of namespace tfel::math

namespace tfel::typetraits {

  /*!
   * \brief generic partial specialisation for a view.
   * \tparam MathObject: object used to create the view to be assigned.
   * \tparam MathObject2: math object that will be assigned.
   */
  template <typename MathObject, typename MathObject2>
  struct IsAssignableTo<tfel::math::View<MathObject>, MathObject2> {
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
  template <typename MathObject, typename MathObject2>
  struct IsAssignableTo<MathObject, tfel::math::View<MathObject2>> {
    //!
    static constexpr bool is_const = std::is_const_v<MathObject2>;
    //! \brief result
    static constexpr bool cond =
        (!is_const) &&
        isAssignableTo<MathObject, std::remove_cv_t<MathObject2>>();
  };  // end of struct IsAssignableTo

  //! \brief specialisation of IsAssignableTo
  template <typename EvaluationResult, typename Operation, typename MathObject>
  struct IsAssignableTo<tfel::math::Expr<EvaluationResult, Operation>,
                        tfel::math::View<MathObject>> {
    //!
    static constexpr bool is_const = std::is_const_v<MathObject>;
    //! \brief result
    static constexpr bool cond =
        (!is_const) &&
        isAssignableTo<EvaluationResult, std::remove_cv_t<MathObject>>();
  };  // end of struct IsAssignableTo

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_ARRAY_VIEW_HXX */
