/*!
 * \file  include/TFEL/Math/Array/FixedSizeMathObjectArrayView.hxx
 * \brief
 * \author Thomas Helfer
 * \date   12/05/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_FIXEDSIZEMATHOBJECTARRAYVIEW_HXX
#define LIB_TFEL_MATH_ARRAY_FIXEDSIZEMATHOBJECTARRAYVIEW_HXX

#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/ArrayPolicies.hxx"
#include "TFEL/Math/Array/IndexingPolicies.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Forward/fsarray.hxx"

namespace tfel::math {

  template <typename MappedType>
  constexpr auto isMappableInAFixedSizeMathObjectArray() {
    if constexpr (isScalar<MappedType>()) {
      return true;
    } else {
      return std::remove_cv_t<MappedType>::indexing_policy::hasFixedSizes;
    }
  }  // end of getFixedSizeMathObjectArrayViewMinimalStride


  template <typename MappedType>
  constexpr auto getFixedSizeMathObjectArrayViewMinimalStride() {
    if constexpr (isScalar<MappedType>()) {
      return 1;
    } else {
      return getUnderlyingArrayMinimalSize<MappedType>();
    }
  }  // end of getFixedSizeMathObjectArrayViewMinimalStride

  template <typename MappedType>
  using FixedSizeMathObjectArrayViewDataPointerType =
      std::conditional_t<isScalar<std::remove_cv_t<MappedType>>(),
                         std::conditional_t<std::is_const_v<MappedType>,
                                            const std::remove_cv_t<MappedType>*,
                                            std::remove_cv_t<MappedType>*>,
                         std::conditional_t<std::is_const_v<MappedType>,
                                            const numeric_type<MappedType>*,
                                            numeric_type<MappedType>*>>;

  /*!
   * \brief view representing a contiguous array of N objects of the
   * `MappedType`
   * \tparam N: number of objects mapped
   * \tparam MappedType: type of the object mapped to the memory area
   * \tparam stride: distance between the start of two successive mapped
   * object in the memory area.
   */
  template <unsigned short N,
            typename MappedType,
            unsigned short stride =
                getFixedSizeMathObjectArrayViewMinimalStride<MappedType>()>
  struct FixedSizeMathObjectArrayView {

    //
    static_assert(
        stride >= getFixedSizeMathObjectArrayViewMinimalStride<MappedType>(),
        "invalid stride");
    //! \brief boolean stating if we provide a view to immutable view
    static constexpr bool is_const = std::is_const_v<MappedType>;
    /*!
     * \brief boolean stating if we provide a view to a scalar or the a math
     * object
     */
    static constexpr bool is_scalar = isScalar<std::remove_cv_t<MappedType>>();
    //! \brief a simple alias
    using RawMappedType = std::remove_cv_t<MappedType>;
    //! \brief a simple alias
    using MappedObjectNumericType =
        std::conditional_t<is_scalar, MappedType, numeric_type<RawMappedType>>;
    //! \brief indexing type
    using size_type = unsigned short;
    //! \brief type of the pointer to the underlying data
    using data_pointer_type =
        FixedSizeMathObjectArrayViewDataPointerType<MappedType>;
    //! \brief type returned by the access operator
    using view_type = std::conditional_t<
        is_scalar,
        std::conditional_t<is_const, const RawMappedType&, RawMappedType&>,
        View<MappedType>>;
    //! \brief type returned by the const access operator
    using const_view_type = std::
        conditional_t<is_scalar, const RawMappedType&, View<const MappedType>>;
    /*!
     * \brief default constructor
     * \param[in] p: pointer to the memory area
     */
    explicit constexpr FixedSizeMathObjectArrayView(
        const data_pointer_type p) noexcept
        : data_pointer(p) {}  // end of FixedSizeMathObjectArrayView

    //! \brief copy constructor
    constexpr FixedSizeMathObjectArrayView(
        const FixedSizeMathObjectArrayView&) noexcept = default;
    //! \brief move constructor
    constexpr FixedSizeMathObjectArrayView(
        FixedSizeMathObjectArrayView&&) noexcept = default;

    //! \brief access operator
    constexpr view_type operator[](const unsigned short i) {
      if constexpr (is_scalar) {
        return *(this->data_pointer + i * stride);
      } else {
        return map<MappedType>(this->data_pointer + i * stride);
      }
    }  // end of operator[]

    //! \brief access operator
    constexpr const_view_type operator[](const unsigned short i) const {
      if constexpr (is_scalar) {
        return *(this->data_pointer + i * stride);
      } else {
        return map<const MappedType>(this->data_pointer + i * stride);
      }
    }  // end of operator[]

    //! \brief access operator
    constexpr view_type operator()(const unsigned short i) {
      return this->operator[](i);
    }  // end of operator()

    //! \brief access operator
    constexpr const_view_type operator()(const unsigned short i) const {
      return this->operator[](i);
    }  // end of operator()

    //
    template <typename ValueType2>
    constexpr std::enable_if_t<
        ((isScalar<ValueType2>()) &&
         (isAssignableTo<
             BinaryOperationResult<MappedObjectNumericType, ValueType2, OpMult>,
             MappedObjectNumericType>())),
        FixedSizeMathObjectArrayView&>
    operator*=(const ValueType2& s) noexcept {
      static_assert(!is_const, "invalid call");
      for (size_type i = 0; i != N; ++i) {
        this->operator[](i) *= s;
      }
      return *this;
    }  // end of operator*=
       //
    template <typename ValueType2>
    constexpr std::enable_if_t<
        ((isScalar<ValueType2>()) &&
         (isAssignableTo<
             BinaryOperationResult<MappedObjectNumericType, ValueType2, OpDiv>,
             MappedObjectNumericType>())),
        FixedSizeMathObjectArrayView&> 
    operator/=(const ValueType2& s) noexcept {
      static_assert(!is_const, "invalid call");
      for (size_type i = 0; i != N; ++i) {
        this->operator[](i) /= s;
      }
      return *this;
    }  // end of operator/=

    //! \brief destructor
    ~FixedSizeMathObjectArrayView() noexcept = default;

   protected:
    const data_pointer_type data_pointer;
  };  // end of struct FixedSizeMathObjectArrayView

  /*!
   * \brief partial specialisation of the `ResultOfEvaluation` class for
   * views.
   * \tparam MappedType: mapped type
   */
  template <unsigned short N, typename MappedType>
  struct ResultOfEvaluation<FixedSizeMathObjectArrayView<N, MappedType>> {
    //! \brief result of the metafunction
    using type = fsarray<N, std::remove_cv_t<MappedType>>;
  };  // end of struct ResultOfEvaluation

  /*!
   * \brief return a view of an array of objects of type `MappedType` from
   * a memory area
   * \tparam N: number of mapped object
   * \tparam MappedType: object mapped
   * \param[in] p: pointer to the mapped memory area
   */
  template <unsigned short N,
            typename MappedType,
            unsigned short stride =
                getFixedSizeMathObjectArrayViewMinimalStride<MappedType>()>
  constexpr std::enable_if_t<
      ((!std::is_const_v<MappedType>)&&(
          isMappableInAFixedSizeMathObjectArray<MappedType>())),
      FixedSizeMathObjectArrayView<N, MappedType, stride>>
  map(const FixedSizeMathObjectArrayViewDataPointerType<MappedType> p) {
    static_assert(
        stride >= getFixedSizeMathObjectArrayViewMinimalStride<MappedType>(),
        "invalid stride");
    return FixedSizeMathObjectArrayView<N, MappedType, stride>{p};
  }  // end of map

  /*!
   * \brief return a const view of an array of objects of type `MappedType` from
   * a memory area
   * \tparam N: size of the array
   * \tparam MappedType: object mapped
   * \param[in] p: pointer to the mapped memory area
   */
  template <unsigned short N,
            typename MappedType,
            unsigned short stride =
                getFixedSizeMathObjectArrayViewMinimalStride<MappedType>()>
  constexpr std::enable_if_t<
      isMappableInAFixedSizeMathObjectArray<MappedType>(),
      FixedSizeMathObjectArrayView<N, const MappedType, stride>>
  map(const FixedSizeMathObjectArrayViewDataPointerType<const MappedType> p) {
    static_assert(
        stride >= getFixedSizeMathObjectArrayViewMinimalStride<MappedType>(),
        "invalid stride");
    return FixedSizeMathObjectArrayView<N, const MappedType, stride>{p};
  }  // end of map

}  // end of namespace tfel::math

namespace tfel::typetraits {

  //! \brief specialisation of IsAssignableTo for `fsarray`'s
  template <unsigned short N, typename T2, unsigned short stride, typename T>
  struct IsAssignableTo<tfel::math::FixedSizeMathObjectArrayView<N, T2,
  stride>,
                        tfel::math::fsarray<N, T>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<T2, T>();
  };

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_ARRAY_FIXEDSIZEMATHOBJECTARRAYVIEW_HXX */
