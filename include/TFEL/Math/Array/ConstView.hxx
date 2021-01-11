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

#ifndef LIB_TFEL_MATH_ARRAY_CONSTVIEW_HXX
#define LIB_TFEL_MATH_ARRAY_CONSTVIEW_HXX

#include <type_traits>
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/ConstFixedSizeArrayBase.hxx"
#include "TFEL/Math/Array/ConstRuntimeArrayBase.hxx"

namespace tfel::math {

  template <typename MappedType>
  using selectConstViewArrayBase = std::conditional_t<
      MappedType::indexing_policy::hasFixedSizes,
      ConstFixedSizeArrayBase<View<MappedType>,
                              typename MappedType::array_policy>,
      ConstRuntimeArrayBase<View<MappedType>,
                            typename MappedType::array_policy>>;

  /*!
   * \brief a const view
   */
  template <typename MappedType>
  struct ConstView : selectConstViewArrayBase<MappedType> {
    //
    using IndexingPolicy = typename MappedType::indexing_policy;
    //
    static constexpr auto hasFixedSizes = IndexingPolicy::hasFixedSizes;
    //! \brief default constructor
    constexpr ConstView(const typename MappedType::value_type* const p) noexcept
        : data_values(p) {
      static_assert(hasFixedSizes, "invalid constructor call");
    }  // end of ConstView
    //! \brief default constructor
    constexpr ConstView(const IndexingPolicy& i,
                        const typename MappedType::value_type* const p) noexcept
        : selectConstViewArrayBase<MappedType>(i), data_values(p) {
      static_assert(!hasFixedSizes, "invalid constructor call");
    }  // end of ConstView
    //! \brief copy constructor
    constexpr ConstView(const ConstView&) noexcept = default;
    //! \brief move constructor
    constexpr ConstView(ConstView&&) noexcept = default;

    //! \return a pointer to the underlying array serving as element storage.
    constexpr typename ConstView::const_pointer data() const noexcept {
      return this->data_values;
    }  // end of data

    //! \brief destructor
    ~ConstView() noexcept = default;

   protected:
    const typename MappedType::value_type* const data_values;
  };  // end of struct ConstView

  template <typename MappedType>
  constexpr std::enable_if_t<MappedType::indexing_policy::hasFixedSizes,
                             ConstView<MappedType>>
  map(const typename MappedType::value_type* const p) {
    return ConstView<MappedType>{p};
  }  // end of map

  template <typename MappedType, typename... Args>
  constexpr auto canMakeConstViewFromLastArgument(){
    using value_type = typename MappedType::value_type;
    using last_type = select_last<Args...>;
    return std::is_convertible_v<const last_type, const value_type* const>;
  };

  template <typename MappedType, typename... Args>
  constexpr std::enable_if_t<
      ((!MappedType::indexing_policy::hasFixedSizes) &&
       (canMakeConstViewFromLastArgument<MappedType, Args...>())),
      ConstView<MappedType>>
  map(const Args&... args) {
    static_assert(sizeof...(Args) >= 2, "invalid call");
    using IndexingPolicy = typename MappedType::indexing_policy;
    const auto r =
        buildIndexingPolicyAndExtractPointerToData<IndexingPolicy>(args...);
    return ConstView<MappedType>{std::get<0>(r), std::get<1>(r)};
  }  // end of map

}  // end of namespace tfel::math

namespace tfel::typetraits {

  /*!
   * \brief generic partial specialisation for a view.
   * \tparam MathObject: math object used to create the view.
   * \tparam MathObject2: object to which the view will be assigned.
   */
  template <typename MathObject, typename MathObject2>
  struct IsAssignableTo<tfel::math::ConstView<MathObject>, MathObject2> {
    //! \brief result
    static constexpr auto cond = isAssignableTo<MathObject, MathObject2>();
  };  // end of struct IsAssignableTo

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_ARRAY_CONSTVIEW_HXX */
