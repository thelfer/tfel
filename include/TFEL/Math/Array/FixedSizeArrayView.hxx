/*!
 * \file  include/TFEL/Math/Array/FixedSizeArrayView.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYVIEW_HXX
#define LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYVIEW_HXX

#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Array/ArrayPolicies.hxx"
#include "TFEL/Math/Array/IndexingPolicies.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Forward/fsarray.hxx"

namespace tfel::math {

  template <typename MappedArrayType>
  constexpr auto isMappableInAFixedSizeArray() {
    using value_type = numeric_type<MappedArrayType>;
    if constexpr (isScalar<value_type>()) {
      return true;
    } else {
      if constexpr (hasIndexingPolicy<value_type>()) {
        using IndexingPolicy = typename value_type::indexing_policy;
        return (IndexingPolicy::hasFixedSizes) && (IndexingPolicy::arity == 1);
      } else {
        return false;
      }
    }
  }  // end of getFixedSizeArrayViewMinimalStride


  template <typename MappedArrayType>
  constexpr auto getFixedSizeArrayViewMinimalStride() {
    if constexpr (isScalar<numeric_type<MappedArrayType>>()) {
      return 1;
    } else {
      return getUnderlyingArrayMinimalSize<numeric_type<MappedArrayType>>();
    }
  }  // end of getFixedSizeArrayViewMinimalStride

  template <bool isScalar, typename MappedArrayType>
  struct FixedSizeArrayViewNumericTypeII {
    using MappedType = numeric_type<MappedArrayType>;
    using type = std::conditional_t<std::is_const_v<MappedArrayType>,
                                    const MappedType,
                                    MappedType>;
  };

  template <typename MappedArrayType>
  struct FixedSizeArrayViewNumericTypeII<false, MappedArrayType> {
    using MappedType = numeric_type<MappedArrayType>;
    using type = std::conditional_t<std::is_const_v<MappedArrayType>,
                                    const numeric_type<MappedType>,
                                    numeric_type<MappedType>>;
  };

  template <typename MappedArrayType>
  using FixedSizeArrayViewNumericType =
      typename FixedSizeArrayViewNumericTypeII<
          isScalar<typename MappedArrayType::value_type>(),
          MappedArrayType>::type;

  template <typename MappedArrayType>
  using FixedSizeArrayViewDataPointerType =
      FixedSizeArrayViewNumericType<MappedArrayType>*;

  /*!
   * \brief view representing a contiguous array of N objects of the
   * `MappedType`
   * \tparam N: number of objects mapped
   * \tparam MappedType: type of the object mapped to the memory area
   * \tparam stride: distance between the start of two successive mapped
   * object in the memory area.
   */
  template <typename MappedArrayType,
            index_type<MappedArrayType> stride =
                getFixedSizeArrayViewMinimalStride<MappedArrayType>()>
  struct FixedSizeArrayView
      : ConceptRebind<typename ComputeObjectTag<MappedArrayType>::type,
                      FixedSizeArrayView<MappedArrayType, stride>>::type {
    //

    //
    static_assert(stride >=
                      getFixedSizeArrayViewMinimalStride<MappedArrayType>(),
                  "invalid stride");
    //! \brief boolean stating if we provide a view to immutable view
    static constexpr bool is_const = std::is_const_v<MappedArrayType>;
    //! \brief a simple alias
    using MappedType = numeric_type<MappedArrayType>;
    /*!
     * \brief boolean stating if we provide a view to a scalar or the a math
     * object
     */
    static constexpr bool is_scalar = isScalar<MappedType>();
    //! \brief a simple alias
    using MappedObjectNumericType =
        std::conditional_t<is_scalar, MappedType, numeric_type<MappedType>>;
    //! \brief indexing type
    using size_type = index_type<MappedArrayType>;
    //! \brief type of the pointer to the underlying data
    using data_pointer_type =
        FixedSizeArrayViewDataPointerType<MappedArrayType>;
    //! \brief type returned by the access operator
    using view_type = std::conditional_t<
        is_scalar,
        std::conditional_t<is_const, const MappedType&, MappedType&>,
        View<MappedType>>;
    //! \brief type returned by the const access operator
    using const_view_type = std::
        conditional_t<is_scalar, const MappedType&, View<const MappedType>>;
    /*!
     * \brief default constructor
     * \param[in] p: pointer to the memory area
     */
    explicit constexpr FixedSizeArrayView(
        const data_pointer_type p) noexcept
        : data_pointer(p) {}  // end of FixedSizeArrayView

    //! \brief constructor
    FixedSizeArrayView() = default;
    //! \brief copy constructor
    constexpr FixedSizeArrayView(
        const FixedSizeArrayView&) noexcept = default;
    //! \brief move constructor
    constexpr FixedSizeArrayView(
        FixedSizeArrayView&&) noexcept = default;

    constexpr size_type size() const {
      using IndexingPolicy =
          typename std::remove_cv_t<MappedArrayType>::indexing_policy;
      IndexingPolicy p;
      return p.size();
    }  // end of size

    //! \brief access operator
    constexpr view_type operator[](const size_type i) {
      if constexpr (is_scalar) {
        if constexpr (stride == 1) {
          return *(this->data_pointer + i);
        } else {
          return *(this->data_pointer + i * stride);
        }
      } else {
        if constexpr (stride == 1) {
          return map<MappedType>(this->data_pointer + i);
        } else {
          return map<MappedType>(this->data_pointer + i * stride);
        }
      }
    }  // end of operator[]

    //! \brief access operator
    constexpr const_view_type operator[](const size_type i) const {
      if constexpr (is_scalar) {
        if constexpr (stride == 1) {
          return *(this->data_pointer + i);
        } else {
          return *(this->data_pointer + i * stride);
        }
      } else {
        if constexpr (stride == 1) {
          return map<const MappedType>(this->data_pointer + i);
        } else {
          return map<const MappedType>(this->data_pointer + i * stride);
        }
      }
    }  // end of operator[]

    //! \brief access operator
    constexpr view_type operator()(const size_type i) {
      return this->operator[](i);
    }  // end of operator()

    //! \brief access operator
    constexpr const_view_type operator()(const size_type i) const {
      return this->operator[](i);
    }  // end of operator()

    template <typename OtherArray>
    constexpr std::enable_if_t<isAssignableTo<OtherArray, MappedArrayType>(),
                               FixedSizeArrayView&>
    operator=(const OtherArray& a) {
      static_assert(!is_const, "invalid call");
      for (size_type i = 0; i != this->size(); ++i) {
        this->operator[](i) = a[i];
      }
      return *this;
    }  // end of operator=

    template <typename OtherArray>
    constexpr std::enable_if_t<isAssignableTo<OtherArray, FixedSizeArrayView>(),
                               FixedSizeArrayView&>
    operator+=(const OtherArray& a) {
      static_assert(!is_const, "invalid call");
      for (size_type i = 0; i != this->size(); ++i) {
        this->operator[](i) += a[i];
      }
      return *this;
    }  // end of operator+=

    template <typename OtherArray>
    constexpr std::enable_if_t<isAssignableTo<OtherArray, FixedSizeArrayView>(),
                               FixedSizeArrayView&>
    operator-=(const OtherArray& a) {
      static_assert(!is_const, "invalid call");
      for (size_type i = 0; i != this->size(); ++i) {
        this->operator[](i) -= a[i];
      }
      return *this;
    }  // end of operator-=

    template <typename ValueType2>
    constexpr std::enable_if_t<
        ((isScalar<ValueType2>()) &&
         (isAssignableTo<
             BinaryOperationResult<MappedObjectNumericType, ValueType2, OpMult>,
             MappedObjectNumericType>())),
        FixedSizeArrayView&>
    operator*=(const ValueType2& s) noexcept {
      static_assert(!is_const, "invalid call");
      for (size_type i = 0; i != this->size(); ++i) {
        this->operator[](i) *= s;
      }
      return *this;
    }  // end of operator*=

    template <typename ValueType2>
    constexpr std::enable_if_t<
        ((isScalar<ValueType2>()) &&
         (isAssignableTo<
             BinaryOperationResult<MappedObjectNumericType, ValueType2, OpDiv>,
             MappedObjectNumericType>())),
        FixedSizeArrayView&>
    operator/=(const ValueType2& s) noexcept {
      static_assert(!is_const, "invalid call");
      for (size_type i = 0; i != this->size(); ++i) {
        this->operator[](i) /= s;
      }
      return *this;
    }  // end of operator/=

    //! \brief destructor
    ~FixedSizeArrayView() noexcept = default;

   protected:
    const data_pointer_type data_pointer;
  };  // end of struct FixedSizeArrayView

  /*!
   * \brief partial specialisation of the `MathObjectTraits` for const views
   * \tparam MappedType: mapped type
   */
  template <typename MappedArrayType, index_type<MappedArrayType> stride>
  struct MathObjectTraits<FixedSizeArrayView<MappedArrayType, stride>>
      : public MathObjectTraits<std::remove_cv_t<MappedArrayType>> {
  };  // end of struct MathObjectTraits

  /*!
   * \brief partial specialisation of the `ResultOfEvaluation` class for
   * views.
   * \tparam MappedType: mapped type
   */
  template <typename MappedArrayType, index_type<MappedArrayType> stride>
  struct ResultOfEvaluation<FixedSizeArrayView<MappedArrayType, stride>> {
    //! \brief result of the metafunction
    using type = std::remove_cv_t<MappedArrayType>;
  };  // end of struct ResultOfEvaluation

  /*!
   * \brief return a view of an array of objects from a memory area
   * \tparam MappedArrayType: array mapped
   * \tparam stride: stride between two mapped value
   * \param[in] p: pointer to the mapped memory area
   */
  template <typename MappedArrayType,
            index_type<MappedArrayType> offset = 0,
            index_type<MappedArrayType> stride =
                getFixedSizeArrayViewMinimalStride<MappedArrayType>()>
  constexpr std::enable_if_t<
      ((!std::is_const_v<MappedArrayType>)&&(
          isMappableInAFixedSizeArray<MappedArrayType>())),
      FixedSizeArrayView<MappedArrayType, stride>>
  map_array(const FixedSizeArrayViewDataPointerType<MappedArrayType> p) {
    static_assert(
        stride >= getFixedSizeArrayViewMinimalStride<MappedArrayType>(),
        "invalid stride");
    return FixedSizeArrayView<MappedArrayType, stride>{p + offset};
  }  // end of map

  /*!
   * \brief return a const view of an array of objects of type `MappedArrayType` from
   * a memory area
   * \tparam MappedArrayType: object mapped
   * \param[in] p: pointer to the mapped memory area
   */
  template <typename MappedArrayType,
            index_type<MappedArrayType> offset = 0,
            index_type<MappedArrayType> stride =
                getFixedSizeArrayViewMinimalStride<MappedArrayType>()>
  constexpr std::enable_if_t<isMappableInAFixedSizeArray<MappedArrayType>(),
                             FixedSizeArrayView<const MappedArrayType, stride>>
  map_array(const FixedSizeArrayViewDataPointerType<const MappedArrayType> p) {
    static_assert(
        stride >= getFixedSizeArrayViewMinimalStride<MappedArrayType>(),
        "invalid stride");
    return FixedSizeArrayView<const MappedArrayType, stride>{p + offset};
  }  // end of map

}  // end of namespace tfel::math

namespace tfel::typetraits {

  //! \brief specialisation of IsAssignableTo
  template <typename MappedArrayType,
            tfel::math::index_type<MappedArrayType> stride,
            typename OtherArray>
  struct IsAssignableTo<tfel::math::FixedSizeArrayView<MappedArrayType, stride>,
                        OtherArray> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<MappedArrayType, OtherArray>();
  };  // end of struct IsAssignableTo

  //! \brief specialisation of IsAssignableTo
  template <typename OtherArray,
            typename MappedArrayType,
            tfel::math::index_type<MappedArrayType> stride>
  struct IsAssignableTo<
      OtherArray,
      tfel::math::FixedSizeArrayView<MappedArrayType, stride>> {
    //!
    static constexpr bool is_const = std::is_const_v<MappedArrayType>;

    //! \brief result
    static constexpr bool cond =
        (!is_const) &&
        isAssignableTo<OtherArray, std::remove_cv_t<MappedArrayType>>();
  };  // end of struct IsAssignableTo

  //! \brief specialisation of IsAssignableTo
  template <typename MappedArrayType,
            tfel::math::index_type<MappedArrayType> stride,
            typename OtherArray>
  struct IsAssignableTo<tfel::math::FixedSizeArrayView<MappedArrayType, stride>,
                        tfel::math::View<OtherArray>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<MappedArrayType, OtherArray>();
  };  // end of struct IsAssignableTo

  //! \brief specialisation of IsAssignableTo
  template <typename OtherArray,
            typename MappedArrayType,
            tfel::math::index_type<MappedArrayType> stride>
  struct IsAssignableTo<
      tfel::math::View<OtherArray>,
      tfel::math::FixedSizeArrayView<MappedArrayType, stride>> {
    //!
    static constexpr bool is_const = std::is_const_v<MappedArrayType>;
    //! \brief result
    static constexpr bool cond =
        (!is_const) &&
        isAssignableTo<OtherArray, std::remove_cv_t<MappedArrayType>>();
  };  // end of struct IsAssignableTo

  //! \brief specialisation of IsAssignableTo
  template <typename EvaluationResult,
            typename Operation,
            typename MappedArrayType,
            tfel::math::index_type<MappedArrayType> stride>
  struct IsAssignableTo<
      tfel::math::Expr<EvaluationResult, Operation>,
      tfel::math::FixedSizeArrayView<MappedArrayType, stride>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<EvaluationResult, MappedArrayType>();
  };  // end of struct IsAssignableTo

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYVIEW_HXX */
