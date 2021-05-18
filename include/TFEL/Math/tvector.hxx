/*!
 * \file   include/TFEL/Math/tvector.hxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TVECTOR_HXX
#define LIB_TFEL_MATH_TVECTOR_HXX 1

#include <cstddef>
#include <iterator>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/RealPartType.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/FSAlgorithm/copy.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Array/FixedSizeArrayView.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/Vector/VectorConceptOperations.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"
#include "TFEL/Math/Forward/tvector.hxx"

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
   * \brief a base for tvector or classes acting like tvector.
   */
  template <typename Child, unsigned short N, typename T>
  struct tvector_base {
    //! Assignement operator
    /*!
     * \param src: a vector expression.
     * \return a reference to this.
     * \pre T2 must be assignable to a T.
     */
    template <typename T2, typename Operation>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator=(const Expr<tvector<N, T2>, Operation>&);

    // Assignement operator
    template <typename T2>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator=(const tvector<N, T2>&);

    // Assignement operator
    template <typename T2, typename Operation>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator+=(const Expr<tvector<N, T2>, Operation>&);

    // Assignement operator
    template <typename T2>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator+=(const tvector<N, T2>&);

    // Assignement operator
    template <typename T2, typename Operation>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator-=(const Expr<tvector<N, T2>, Operation>&);

    // Assignement operator
    template <typename T2>
    TFEL_MATH_INLINE std::enable_if_t<isAssignableTo<T2, T>(), Child&>
    operator-=(const tvector<N, T2>&);

    /*!
     * operator*=
     */
    template <typename T2>
    TFEL_MATH_INLINE
        std::enable_if_t<IsTVectorScalarOperationValid<T, T2, OpMult>::cond,
                         Child&>
        operator*=(const T2);

    /*!
     * operator/=
     */
    template <typename T2>
    TFEL_MATH_INLINE
        std::enable_if_t<IsTVectorScalarOperationValid<T, T2, OpDiv>::cond,
                         Child&>
        operator/=(const T2);
  };

  template <unsigned short N, typename ValueType = double>
  struct tvector : VectorConcept<tvector<N, ValueType>>,
                   GenericFixedSizeArray<tvector<N, ValueType>,
                                         FixedSizeVectorPolicy<N, ValueType>> {
    //! \brief a simple alias
    using GenericFixedSizeArrayBase =
        GenericFixedSizeArray<tvector<N, ValueType>,
                              FixedSizeVectorPolicy<N, ValueType>>;
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(tvector,
                                               GenericFixedSizeArrayBase);
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
    /*!
     * copy the Nth elements following this argument.
     * \param const InputIterator, an iterator to the first element
     * to be copied.
     */
    template <typename InputIterator>
    TFEL_MATH_INLINE void copy(const InputIterator src);
    /*!
     * \brief create a slice
     * \param[in] I : the starting index
     */
    template <unsigned short I>
    auto slice();
    /*!
     * \brief create a slice
     * \param[in] I : the starting index
     * \param[in] J : the size of the slice
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template <unsigned short I, unsigned short J>
    auto slice();
    /*!
     * \brief create a slice (const version)
     * \param[in] I : the starting index
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template <unsigned short I>
    auto slice() const;
    /*!
     * \brief create a slice (const version)
     * \param[in] I : the starting index
     * \param[in] J : the size of the slice
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template <unsigned short I, unsigned short J>
    auto slice() const;
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
  tvector<N, typename std::result_of<F(T)>::type> map(F, const tvector<N, T>&);
  /*!
   * export the given vector to an array of the
   */
  template <unsigned short N, typename T, typename OutputIterator>
  TFEL_MATH_INLINE2 std::enable_if_t<isScalar<T>(), void> exportToBaseTypeArray(
      const tvector<N, T>&, OutputIterator);

  template <unsigned short N, typename T>
  TFEL_MATH_INLINE2 typename tfel::typetraits::AbsType<T>::type abs(
      const tvector<N, T>& v);

  template <typename T>
  tvector<1u, T> makeTVector1D(const T);

  template <typename T>
  tvector<2u, T> makeTVector2D(const T, const T);

  template <typename T>
  tvector<3u, T> makeTVector3D(const T, const T, const T);

  template <typename T>
  tvector<3u, T> cross_product(const tvector<2u, T>&, const tvector<2u, T>&);

  template <typename T>
  tvector<3u, T> cross_product(const tvector<3u, T>&, const tvector<3u, T>&);

  /*!
   * find a vector perpendicular to the second one
   */
  template <typename T>
  void find_perpendicular_vector(tvector<3u, T>&, const tvector<3u, T>&);

  /*!
   * \brief create a slice from a tiny vector
   * \param[in] v : vector
   * \note : the slice object contains a reference to the source
   * vector, so this vector shall not be destroyed before the slice
   */
  template <unsigned short I, unsigned short N, typename T>
  auto slice(tvector<N, T>&);
  /*!
   * \brief create a slice from a tiny vector
   * \param[in] v : vector
   * \note : the slice object contains a reference to the source
   * vector, so this vector shall not be destroyed before the slice
   */
  template <unsigned short I, unsigned short J, unsigned short N, typename T>
  auto slice(tvector<N, T>&);
  /*!
   * \brief create a slice from a tiny vector
   * \param[in] v : vector
   * \note : the slice object contains a reference to the source
   * vector, so this vector shall not be destroyed before the slice
   */
  template <unsigned short I, unsigned short N, typename T>
  auto slice(const tvector<N, T>&);
  /*!
   * \brief create a slice from a tiny vector (const version)
   * \param[in] v : vector
   * \note : the slice object contains a reference to the source
   * vector, so this vector shall not be destroyed before the slice
   */
  template <unsigned short I, unsigned short J, unsigned short N, typename T>
  auto slice(const tvector<N, T>&);

  /*!
   * \brief create a view of a math object from a tiny vector
   * \tparam MappedType : type of mapped object
   * \tparam offset: offset in the tiny vector
   * \tparam N: size of the tiny vector
   * \tparam T: type hold by the tiny vector
   */
  template <typename MappedType,
            unsigned short offset = 0u,
            unsigned short N,
            typename real>
  constexpr std::enable_if_t<((!std::is_const_v<MappedType>)&&(
                                 MappedType::indexing_policy::hasFixedSizes)),
                             View<MappedType>>
  map(tvector<N, real>&);

  /*!
   * \brief create a constant view of a math object from a tiny vector
   * \tparam MappedType : type of mapped object
   * \tparam offset: offset in the tiny vector
   * \tparam N: size of the tiny vector
   * \tparam T: type hold by the tiny vector
   */
  template <typename MappedType,
            unsigned short offset = 0u,
            unsigned short N,
            typename real>
  constexpr std::enable_if_t<MappedType::indexing_policy::hasFixedSizes,
                             View<const MappedType>>
  map(const tvector<N, real>&);

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
            unsigned short stride =
                getFixedSizeArrayViewMinimalStride<tvector<M, MappedType>>(),
            unsigned short N>
  constexpr std::enable_if_t<
      ((!std::is_const_v<MappedType>)&&(
          isMappableInAFixedSizeArray<tvector<M, MappedType>>())),
      FixedSizeArrayView<tvector<M, MappedType>, stride>>
  map(tvector<N, FixedSizeArrayViewNumericType<tvector<M, MappedType>>>&);

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
            unsigned short stride =
                getFixedSizeArrayViewMinimalStride<tvector<M, MappedType>>(),
            unsigned short N>
  constexpr std::enable_if_t<
      isMappableInAFixedSizeArray<tvector<M, std::remove_cv_t<MappedType>>>(),
      FixedSizeArrayView<const tvector<M, std::remove_cv_t<MappedType>>,
                         stride>>
  map(const tvector<N,
                    FixedSizeArrayViewNumericType<
                        tvector<M, std::remove_cv_t<MappedType>>>>&);

}  // namespace tfel::math

namespace tfel::typetraits {

  //! \brief partial specialisation for tvectors
  template <unsigned short N, typename T2, typename T>
  struct IsAssignableTo<tfel::math::tvector<N, T2>, tfel::math::tvector<N, T>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<T2, T>();
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/Vector/tvector.ixx"
#include "TFEL/Math/Vector/tvectorResultType.hxx"

#endif /* LIB_TFEL_MATH_TVECTOR_HXX */
