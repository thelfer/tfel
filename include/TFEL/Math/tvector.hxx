/*!
 * \file   include/TFEL/Math/tvector.hxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TVECTOR_HXX
#define LIB_TFEL_MATH_TVECTOR_HXX 1

#include <utility>
#include <cstddef>
#include <iterator>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/RealPartType.hxx"
#include "TFEL/FSAlgorithm/copy.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/General/DerivativeType.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Array/ViewsArray.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/Vector/VectorConceptOperations.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"
#include "TFEL/Math/Forward/tvector.hxx"
#include "TFEL/Math/fsarray.hxx"

namespace tfel::math {

  /*!
   * An helper class to deal with limitation of Visual Studio 10
   */
  template <typename T, typename T2, typename Op>
  struct IsTVectorScalarOperationValid {
    static constexpr bool cond =
        isScalar<T2>() && std::is_same<result_type<T, T2, OpMult>, T>::value;
  };  // end of struct IsTVectorScalarOperationValid

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <unsigned short N, typename TVectorNumericType, typename ScalarType>
  struct DerivativeTypeDispatcher<VectorTag,
                                  ScalarTag,
                                  tvector<N, TVectorNumericType>,
                                  ScalarType> {
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<TVectorNumericType>(),
                  "template argument TVectorNumericType is not a scalar");
    //! \brief result
    using type = tvector<N, derivative_type<TVectorNumericType, ScalarType>>;
  };  // end of struct DerivativeTypeDispatcher
  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename ScalarType, unsigned short N, typename TVectorNumericType>
  struct DerivativeTypeDispatcher<ScalarTag,
                                  VectorTag,
                                  ScalarType,
                                  tvector<N, TVectorNumericType>> {
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<TVectorNumericType>(),
                  "template argument TVectorNumericType is not a scalar");
    //! \brief result
    using type = tvector<N, derivative_type<ScalarType, TVectorNumericType>>;
  };  // end of struct DerivativeTypeDispatcher

  template <unsigned short N, typename ValueType = double>
  struct tvector : VectorConceptBase<tvector<N, ValueType>>,
                   GenericFixedSizeArray<tvector<N, ValueType>,
                                         FixedSizeVectorPolicy<N, ValueType>> {
    //! \brief a simple alias
    using GenericFixedSizeArrayBase =
        GenericFixedSizeArray<tvector<N, ValueType>,
                              FixedSizeVectorPolicy<N, ValueType>>;
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(tvector,
                                               GenericFixedSizeArrayBase);
    /*!
     * \brief implicit conversion from an `fsarray` provided for backward
     * compatibility.
     * \param[in] src: object to be copied
     */
    TFEL_HOST_DEVICE tvector(const fsarray<N, ValueType>&) noexcept;
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
    /*!
     * copy the Nth elements following this argument.
     * \param const InputIterator, an iterator to the first element
     * to be copied.
     */
    template <typename InputIterator>
    TFEL_HOST_DEVICE TFEL_MATH_INLINE constexpr void copy(
        const InputIterator src) noexcept;
    /*!
     * \brief create a slice
     * \param[in] I : the starting index
     */
    template <unsigned short I>
    TFEL_HOST_DEVICE constexpr auto slice() noexcept;
    /*!
     * \brief create a slice
     * \param[in] I : the starting index
     * \param[in] J : the size of the slice
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template <unsigned short I, unsigned short J>
    TFEL_HOST_DEVICE constexpr auto slice() noexcept;
    /*!
     * \brief create a slice (const version)
     * \param[in] I : the starting index
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template <unsigned short I>
    TFEL_HOST_DEVICE constexpr auto slice() const noexcept;
    /*!
     * \brief create a slice (const version)
     * \param[in] I : the starting index
     * \param[in] J : the size of the slice
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template <unsigned short I, unsigned short J>
    TFEL_HOST_DEVICE constexpr auto slice() const noexcept;
  };  // end of tvector

  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: number of values
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using TVectorView = View<tvector<N, T>>;
  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: number of values
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using ConstTVectorView = ConstView<tvector<N, T>>;

  /*!
   * \brief create a new tvector by applying a functor
   * \param[in] f: functor
   * \param[in] x: inital value
   */
  template <typename F, typename T, unsigned short N>
  TFEL_HOST_DEVICE constexpr tvector<N, std::invoke_result_t<F, T>> map(
      F, const tvector<N, T>&);
  /*!
   * export the given vector to an array of the
   */
  template <unsigned short N, typename T, typename OutputIterator>
  TFEL_HOST_DEVICE constexpr void exportToBaseTypeArray(const tvector<N, T>&,
                                                        OutputIterator) noexcept
      requires(isScalar<T>());

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto norm(const tvector<N, T>&) noexcept  //
      requires(isScalar<T>());

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto abs(const tvector<N, T>& v) noexcept;

  template <typename T>
  TFEL_HOST_DEVICE constexpr tvector<1u, T> makeTVector1D(const T) noexcept;

  template <typename T>
  TFEL_HOST_DEVICE constexpr tvector<2u, T> makeTVector2D(const T,
                                                          const T) noexcept;

  template <typename T>
  TFEL_HOST_DEVICE constexpr tvector<3u, T> makeTVector3D(const T,
                                                          const T,
                                                          const T) noexcept;

  template <typename T>
  TFEL_HOST_DEVICE constexpr tvector<3u, T> cross_product(
      const tvector<2u, T>&, const tvector<2u, T>&) noexcept;

  template <typename T>
  TFEL_HOST_DEVICE constexpr tvector<3u, T> cross_product(
      const tvector<3u, T>&, const tvector<3u, T>&) noexcept;

  /*!
   * find a vector perpendicular to the second one
   */
  template <typename T>
  TFEL_HOST_DEVICE constexpr void find_perpendicular_vector(
      tvector<3u, T>&, const tvector<3u, T>&) noexcept;

  /*!
   * \brief create a slice from a tiny vector
   * \param[in] v : vector
   * \note : the slice object contains a reference to the source
   * vector, so this vector shall not be destroyed before the slice
   */
  template <unsigned short I, unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto slice(tvector<N, T>&) noexcept;
  /*!
   * \brief create a slice from a tiny vector
   * \param[in] v : vector
   * \note : the slice object contains a reference to the source
   * vector, so this vector shall not be destroyed before the slice
   */
  template <unsigned short I, unsigned short J, unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto slice(tvector<N, T>&) noexcept;
  /*!
   * \brief create a slice from a tiny vector
   * \param[in] v : vector
   * \note : the slice object contains a reference to the source
   * vector, so this vector shall not be destroyed before the slice
   */
  template <unsigned short I, unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto slice(const tvector<N, T>&) noexcept;
  /*!
   * \brief create a slice from a tiny vector (const version)
   * \param[in] v : vector
   * \note : the slice object contains a reference to the source
   * vector, so this vector shall not be destroyed before the slice
   */
  template <unsigned short I, unsigned short J, unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto slice(const tvector<N, T>&) noexcept;

  /*!
   * \brief create a view of a math object from a tiny vector
   * \tparam MappedType : type of mapped object
   * \tparam IndexingPolicyType: index policy type used by the generated view
   * \tparam N: size of the tiny vector
   * \tparam T: type hold by the tiny vector
   */
  template <typename MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy,
            unsigned short N>
  TFEL_HOST_DEVICE constexpr auto map(
      tvector<N, base_type<numeric_type<MappedType>>>&) noexcept
      requires((!isScalar<MappedType>()) &&
               (IndexingPolicyType::hasFixedSizes) &&
               (checkIndexingPoliciesCompatiblity<
                   IndexingPolicyType,
                   typename std::remove_cv_t<MappedType>::indexing_policy>()));

  /*!
   * \brief create a constant view of a math object from a tiny vector
   * \tparam MappedType : type of mapped object
   * \tparam IndexingPolicyType: index policy type used by the generated view
   * \tparam N: size of the tiny vector
   * \tparam T: type hold by the tiny vector
   */
  template <typename MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy,
            unsigned short N>
  TFEL_HOST_DEVICE constexpr auto map(
      const tvector<N, base_type<numeric_type<MappedType>>>&) noexcept
      requires((!isScalar<MappedType>()) &&
               (IndexingPolicyType::hasFixedSizes) &&
               (checkIndexingPoliciesCompatiblity<
                   IndexingPolicyType,
                   typename std::remove_cv_t<MappedType>::indexing_policy>()));
  /*!
   * \brief create a view of a math object from a tiny vector
   * \tparam MappedType : type of mapped object
   * \tparam offset: offset in the tiny vector
   * \tparam IndexingPolicyType: index policy type used by the generated view
   * \tparam N: size of the tiny vector
   * \tparam T: type hold by the tiny vector
   */
  template <typename MappedType,
            unsigned short offset,
            typename IndexingPolicyType = typename MappedType::indexing_policy,
            unsigned short N,
            typename real>
  TFEL_HOST_DEVICE constexpr auto map(tvector<N, real>&) noexcept requires(
      (!std::is_const_v<MappedType>)&&(IndexingPolicyType::hasFixedSizes) &&
      (checkIndexingPoliciesCompatiblity<
          IndexingPolicyType,
          typename MappedType::indexing_policy>()));

  /*!
   * \brief create a constant view of a math object from a tiny vector
   * \tparam MappedType : type of mapped object
   * \tparam offset: offset in the tiny vector
   * \tparam IndexingPolicyType: index policy type used by the generated view
   * \tparam N: size of the tiny vector
   * \tparam T: type hold by the tiny vector
   */
  template <typename MappedType,
            unsigned short offset,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy,
            unsigned short N,
            typename real>
  TFEL_HOST_DEVICE constexpr auto map(const tvector<N, real>&) noexcept
      requires((IndexingPolicyType::hasFixedSizes) &&
               (checkIndexingPoliciesCompatiblity<
                   IndexingPolicyType,
                   typename std::remove_cv_t<MappedType>::indexing_policy>()));

  /*!
   * \brief create a view on an array of fixed sized math objects from a tiny
   * vector
   * \tparam M: number of objects mapped
   * \tparam MappedType : type of mapped object
   * \tparam offset: offset from the start of the tiny vector
   * \tparam N: size of the tiny vector
   */
  template <unsigned short M,
            typename MappedType,
            unsigned short offset = 0u,
            unsigned short stride = getViewsArrayMinimalStride<MappedType>(),
            unsigned short N>
  TFEL_HOST_DEVICE constexpr auto map(
      tvector<N, ViewsArrayNumericType<MappedType>>&) noexcept
      requires(!std::is_const_v<MappedType>);
  /*!
   * \brief create a const view on an array of fixed sized math objects from a
   * tiny vector
   * \tparam M: number of objects mapped
   * \tparam MappedType : type of mapped object
   * \tparam offset: offset from the start of the tiny vector
   * \tparam N: size of the tiny vector
   */
  template <unsigned short M,
            typename MappedType,
            unsigned short offset = 0u,
            unsigned short stride = getViewsArrayMinimalStride<MappedType>(),
            unsigned short N>
  TFEL_HOST_DEVICE constexpr auto map(
      const tvector<N, ViewsArrayNumericType<MappedType>>&);

}  // namespace tfel::math

namespace tfel::typetraits {

  //! \brief partial specialisation for tvectors
  template <unsigned short N, typename T2, typename T>
  struct IsAssignableTo<tfel::math::tvector<N, T2>, tfel::math::tvector<N, T>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<T2, T>();
  };

}  // end of namespace tfel::typetraits

namespace std {

  /*!
   * \brief partial specialisation of the `std::tuple_size` for
   * `tfel::math::tvector`
   * \tparam N: size of the tiny vector
   * \tparam ValueType: type hold by the tiny vector
   */
  template <unsigned short N, typename ValueType>
  struct tuple_size<tfel::math::tvector<N, ValueType>>
      : integral_constant<size_t, N> {};
  /*!
   * \brief partial specialisation of the `std::tuple_element` for
   * `tfel::math::tvector`
   * \tparam N: size of the tiny vector
   * \tparam ValueType: type hold by the tiny vector
   */
  template <std::size_t I, unsigned short N, typename ValueType>
  struct tuple_element<I, tfel::math::tvector<N, ValueType>> {
    //! \brief returned type
    using type = ValueType;
  };  // end of struct tuple_element

}  // namespace std

namespace tfel::math {

  /*!
   * \brief partial specialisation of `std::get` for
   * `tfel::math::tvector`
   * \tparam i: index of the element to be retrieved
   * \tparam N: size of the tiny vector
   * \tparam ValueType: type hold by the tiny vector
   */
  template <std::size_t i, unsigned short N, typename ValueType>
  TFEL_HOST_DEVICE constexpr std::tuple_element_t<i, tvector<N, ValueType>>&
  get(tvector<N, ValueType>& v) noexcept {
    return v[i];
  }  // end of get
  /*!
   * \brief partial specialisation of `std::get` for
   * `tfel::math::tvector`
   * \tparam i: index of the element to be retrieved
   * \tparam N: size of the tiny vector
   * \tparam ValueType: type hold by the tiny vector
   */
  template <std::size_t i, unsigned short N, typename ValueType>
  TFEL_HOST_DEVICE constexpr const std::tuple_element_t<i,
                                                        tvector<N, ValueType>>&
  get(const tvector<N, ValueType>& v) noexcept {
    return v[i];
  }  // end of get

  // class template argument deduction
  template <typename... T>
  tvector(T&&... t) -> tvector<sizeof...(T), std::common_type_t<T...>>;

}  // end of namespace tfel::math

#include "TFEL/Math/Vector/tvector.ixx"
#include "TFEL/Math/Vector/tvectorResultType.hxx"

#endif /* LIB_TFEL_MATH_TVECTOR_HXX */
