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

namespace tfel::math {

  template <typename MappedType>
  struct View;

  template <typename MappedType>
  using selectViewArrayBase = std::conditional_t<
      MappedType::indexing_policy::hasFixedSizes,
      MutableFixedSizeArrayBase<View<MappedType>,
                                typename MappedType::array_policy>,
      MutableRuntimeArrayBase<View<MappedType>,
                              typename MappedType::array_policy>>;

  template <typename MappedType>
  struct View : ConceptRebind<typename ComputeObjectTag<MappedType>::type,
                              View<MappedType>>::type,
                selectViewArrayBase<MappedType> {
    //
    using ArrayPolicy = typename MappedType::array_policy;
    //
    using IndexingPolicy = typename MappedType::indexing_policy;
    //
    static constexpr auto hasFixedSizes = IndexingPolicy::hasFixedSizes;
    //! \brief default constructor
    constexpr View(typename View::value_type* const p) noexcept
        : data_values(p) {
      static_assert(hasFixedSizes, "invalid constructor call");
    }  // end of View
    //! \brief default constructor
    constexpr View(const IndexingPolicy& i,
                   typename View::value_type* const p) noexcept
        : selectViewArrayBase<MappedType>(i), data_values(p) {
      static_assert(!hasFixedSizes, "invalid constructor call");
    }  // end of View
    //! \brief copy constructor
    constexpr View(const View&) noexcept = default;
    //! \brief move constructor
    constexpr View(View&&) noexcept = default;
    //! \brief assignement operator
    constexpr View& operator=(const View& src) noexcept {
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->assign(src);
      return *this;
    }
    //! \brief move assigment
    constexpr View& operator=(View&& src) noexcept {
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
    constexpr typename View::pointer data() noexcept {
      return this->data_values;
    }  // end of data

    //! \return a pointer to the underlying array serving as element storage.
    constexpr typename View::const_pointer data() const noexcept {
      return this->data_values;
    }  // end of data

    /*!
     * \brief assign values values from an other array
     * \param[in] src: array to be assigned
     */
    template <typename OtherArray>
    std::enable_if_t<isAssignableTo<OtherArray, MappedType>(), View&> operator=(
        const OtherArray& src) {
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->assign(src);
      return *this;
    }
    //
    template <typename OtherArray>
    std::enable_if_t<isAssignableTo<OtherArray, MappedType>(), View&>
    operator+=(const OtherArray& src) {
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->addAndAssign(src);
      return *this;
    }
    //
    template <typename OtherArray>
    std::enable_if_t<isAssignableTo<OtherArray, MappedType>(), View&>
    operator-=(const OtherArray& src) {
      //       checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
      //                                                src.getIndexingPolicy());
      this->substractAndAssign(src);
      return *this;
    }
    //! \brief destructor
    ~View() noexcept = default;

   protected:
    typename MappedType::value_type* const data_values;
  };  // end of struct View

  /*!
   * \brief partial specialisation of the `MathObjectTraits` for const views
   * \tparam MappedType: mapped type
   */
  template <typename MappedType>
  struct MathObjectTraits<View<MappedType>>
      : public MathObjectTraits<MappedType> {
  };  // end of struct MathObjectTraits<Expr<MappedType, Operation>>

  /*!
   * \brief partial specialisation of the `ResultOfEvaluation` class for
   * views.
   * \tparam MappedType: mapped type
   */
  template <typename MappedType>
  struct ResultOfEvaluation<View<MappedType>> {
    //! \brief result of the metafunction
    using type = MappedType;
  };  // end of struct ResultOfEvaluation

  template <typename MappedType>
  constexpr std::enable_if_t<MappedType::indexing_policy::hasFixedSizes,
                             View<MappedType>>
  map(typename MappedType::value_type* const p) {
    return View<MappedType>{p};
  }  // end of map

  template <typename MappedType, typename... Args>
  constexpr auto canMakeViewFromLastArgument(){
    using value_type = typename MappedType::value_type;
    using last_type = select_last<Args...>;
    return (std::is_convertible_v<last_type, value_type* const>);
  }

  template <typename MappedType, typename... Args>
  constexpr std::enable_if_t<
      ((!MappedType::indexing_policy::hasFixedSizes) &&
       (canMakeViewFromLastArgument<MappedType, Args...>())),
      View<MappedType>>
  map(Args&&... args) {
    static_assert(sizeof...(Args) >= 2, "invalid call");
    using IndexingPolicy = typename MappedType::indexing_policy;
    const auto r =
        buildIndexingPolicyAndExtractPointerToData<IndexingPolicy>(args...);
    return View<MappedType>{std::get<0>(r), std::get<1>(r)};
  }  // end of map

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
    static constexpr bool cond = isAssignableTo<MathObject, MathObject2>();
  };  // end of struct IsAssignableTo

  /*!
   * \brief generic partial specialisation for a view.
   * \tparam MathObject: object to be assigned.
   * \tparam MathObject2: math object used to create the view that will be
   * assigned.
   */
  template <typename MathObject, typename MathObject2>
  struct IsAssignableTo<MathObject, tfel::math::View<MathObject2>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<MathObject, MathObject2>();
  };  // end of struct IsAssignableTo

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_ARRAY_VIEW_HXX */
