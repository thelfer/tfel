/*!
 * \file  include/TFEL/Math/fsarray.hxx
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

#ifndef LIB_TFEL_MATH_FSARRAY_HXX
#define LIB_TFEL_MATH_FSARRAY_HXX

#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/ViewsArray.hxx"
#include "TFEL/Math/Array/ArrayConcept.hxx"
#include "TFEL/Math/Forward/fsarray.hxx"

namespace tfel::math {

  template <unsigned short N, typename ValueType>
  struct array_holder : MutableArrayWithContiguousDataSTLCompatibilityLayer<
                            array_holder<N, ValueType>,
                            FixedSizeVectorPolicy<N, ValueType>> {
    //! \brief a simple alias
    using size_type = unsigned short;
    //
    TFEL_HOST_DEVICE constexpr size_type size() const noexcept { return N; }
    //
    constexpr auto data() noexcept { return this->v; }
    //
    constexpr auto data() const noexcept { return this->v; }
    //! \brief access operator
    TFEL_HOST_DEVICE constexpr ValueType& operator[](
        const size_type i) noexcept {
      return v[i];
    }
    TFEL_HOST_DEVICE constexpr const ValueType& operator[](
        const size_type i) const noexcept {
      return v[i];
    }

   protected:
    //! \brief values
    ValueType v[N];
  };

  /*!
   * \brief a class representing a fixed sized array
   * \tparam N: number of values hold
   * \tparam ValueType: type of the values
   */
  template <unsigned short N, typename ValueType = double>
  struct fsarray : GenericFixedSizeArray<fsarray<N, ValueType>,
                                         FixedSizeVectorPolicy<N, ValueType>>,
                   ArrayConceptBase<fsarray<N, ValueType>> {
    //! \brief default constructor
    constexpr fsarray() noexcept = default;
    //! \brief move constructor
    constexpr fsarray(fsarray&&) noexcept = default;
    //! \brief copy constructor
    constexpr fsarray(const fsarray&) noexcept = default;
    //! \brief move assignement
    constexpr fsarray& operator=(fsarray&&) noexcept = default;
    //! \brief standard assignement
    constexpr fsarray& operator=(const fsarray&) noexcept = default;
    // inheriting GenericFixedSizeArray' constructors
    using GenericFixedSizeArray<
        fsarray<N, ValueType>,
        FixedSizeVectorPolicy<N, ValueType>>::GenericFixedSizeArray;
    // inheriting GenericFixedSizeArray' assignement operators
    using GenericFixedSizeArray<fsarray<N, ValueType>,
                                FixedSizeVectorPolicy<N, ValueType>>::operator=;
  };  // end of fsarray

  // class template argument deduction
  template <typename... T>
  fsarray(T&&... t) -> fsarray<sizeof...(T), std::common_type_t<T...>>;

  /*!
   * \tparam N: arrray size
   * \tparam T: numeric type used by the array
   */
  template <unsigned short N, typename T>
  struct UnaryResultType<fsarray<N, T>, OpNeg> {
    typedef typename UnaryResultType<T, OpNeg>::type ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    fsarray<N, ResultNumericType>>;
  };

  /*!
   * \brief partial specialisation for fsarray and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<ArrayTag,
                                     ScalarTag,
                                     fsarray<N, T>,
                                     Scal,
                                     OpMult> {
    typedef result_type<T, Scal, OpMult> ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    fsarray<N, ResultNumericType>>;
  };

  /*!
   * \brief partial specialisation for fsarray and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<ArrayTag,
                                     ScalarTag,
                                     fsarray<N, T>,
                                     Scal,
                                     OpDiv> {
    typedef result_type<T, Scal, OpDiv> ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    fsarray<N, ResultNumericType>>;
  };

  /*!
   * \brief partial specialisation for fsarray and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename Scal, unsigned short N, typename T>
  class ComputeBinaryOperationResult<ScalarTag,
                                     ArrayTag,
                                     Scal,
                                     fsarray<N, T>,
                                     OpMult> {
    typedef result_type<Scal, T, OpMult> ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    fsarray<N, ResultNumericType>>;
  };

  /*!
   * \brief partial specialisation for fsarray
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<fsarray<N, T>, fsarray<N, T2>, OpPlus> {
    typedef result_type<T, T2, OpPlus> ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    fsarray<N, ResultNumericType>>;
  };

  /*!
   * \brief partial specialisation for fsarray
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<fsarray<N, T>, fsarray<N, T2>, OpMinus> {
    typedef result_type<T, T2, OpMinus> ResultNumericType;

   public:
    using type = std::conditional_t<isInvalid<ResultNumericType>(),
                                    tfel::meta::InvalidType,
                                    fsarray<N, ResultNumericType>>;
  };

}  // end of namespace tfel::math

namespace tfel::typetraits {

  //! \brief specialisation of IsAssignableTo for `fsarray`'s
  template <unsigned short N, typename ValueType, typename ValueType2>
  struct IsAssignableTo<tfel::math::fsarray<N, ValueType>,
                        tfel::math::fsarray<N, ValueType2>> {
    //! \brief result
    static constexpr auto cond = isAssignableTo<ValueType, ValueType2>();
  };

  /*!
   * \brief partial specialisation allowing arrays of views to be assigned to a
   * fixed size array
   */
  template <typename MappedType,
            typename MemoryIndexingPolicyType,
            typename ViewIndexingPolicyType,
            typename ValueType,
            unsigned short N>
  struct IsAssignableTo<tfel::math::ViewsArray<MappedType,
                                               MemoryIndexingPolicyType,
                                               ViewIndexingPolicyType>,
                        tfel::math::fsarray<N, ValueType>> {
    //! \brief a simple alias
    using FixedSizeArrayIndexingPolicy =
        typename tfel::math::fsarray<N, ValueType>::indexing_policy;
    //! \brief a simple alias
    using View = std::decay_t<
        typename tfel::math::ViewsArray<MappedType,
                                        MemoryIndexingPolicyType,
                                        ViewIndexingPolicyType>::view_type>;

    //! \brief result
    static constexpr auto cond =
        ((tfel::math::checkIndexingPoliciesCompatiblity<
             FixedSizeArrayIndexingPolicy,
             MemoryIndexingPolicyType>()) &&
         (isAssignableTo<View, ValueType>()));
  };

}  // end of namespace tfel::typetraits

namespace std {

  /*!
   * \brief partial specialisation of the `std::tuple_size` for
   * `tfel::math::fsarray`
   * \tparam N: size of the tiny vector
   * \tparam ValueType: type hold by the tiny vector
   */
  template <unsigned short N, typename ValueType>
  struct tuple_size<tfel::math::fsarray<N, ValueType>>
      : integral_constant<size_t, N> {};
  /*!
   * \brief partial specialisation of the `std::tuple_element` for
   * `tfel::math::fsarray`
   * \tparam N: size of the tiny vector
   * \tparam ValueType: type hold by the tiny vector
   */
  template <std::size_t I, unsigned short N, typename ValueType>
  struct tuple_element<I, tfel::math::fsarray<N, ValueType>> {
    //! \brief returned type
    using type = ValueType;
  };  // end of struct tuple_element

}  // namespace std

namespace tfel::math {

  /*!
   * \brief partial specialisation of `std::get` for
   * `tfel::math::fsarray`
   * \tparam i: index of the element to be retrieved
   * \tparam N: size of the tiny vector
   * \tparam ValueType: type hold by the tiny vector
   */
  template <std::size_t i, unsigned short N, typename ValueType>
  TFEL_HOST_DEVICE constexpr std::tuple_element_t<i, fsarray<N, ValueType>>&
  get(fsarray<N, ValueType>& v) noexcept {
    return v[i];
  }  // end of get
  /*!
   * \brief partial specialisation of `std::get` for
   * `tfel::math::fsarray`
   * \tparam i: index of the element to be retrieved
   * \tparam N: size of the tiny vector
   * \tparam ValueType: type hold by the tiny vector
   */
  template <std::size_t i, unsigned short N, typename ValueType>
  TFEL_HOST_DEVICE constexpr const std::tuple_element_t<i,
                                                        fsarray<N, ValueType>>&
  get(const fsarray<N, ValueType>& v) noexcept {
    return v[i];
  }  // end of get

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FSARRAY_HXX */
