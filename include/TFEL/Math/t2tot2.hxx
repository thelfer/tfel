/*!
 * \file   include/TFEL/Math/t2tot2.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOT2_HXX
#define LIB_TFEL_MATH_T2TOT2_HXX

#include <cstddef>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/Math/fsarray.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Forward/t2tot2.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include "TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include "TFEL/Math/T2toT2/T2toT2ConceptOperations.hxx"

namespace tfel::math {

  // forward declaration
  template <unsigned short N>
  struct TensorProductLeftDerivativeExpr;

  // forward declaration
  template <unsigned short N>
  struct TensorProductRightDerivativeExpr;

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename TensorType1, typename TensorType2>
  struct DerivativeTypeDispatcher<TensorTag,
                                  TensorTag,
                                  TensorType1,
                                  TensorType2> {
    static_assert(implementsTensorConcept<TensorType1>(),
                  "template argument TensorType1 is not a tensor");
    static_assert(implementsTensorConcept<TensorType2>(),
                  "template argument TensorType2 is not a tensor");
    static_assert(getSpaceDimension<TensorType1>() ==
                      getSpaceDimension<TensorType2>(),
                  "symmetric tensor types don't have the same dimension");
    static_assert(tfel::typetraits::IsScalar<TensorNumType<TensorType1>>::cond,
                  "the first tensor type does not hold a scalar");
    static_assert(tfel::typetraits::IsScalar<TensorNumType<TensorType2>>::cond,
                  "the second tensor type does not hold a scalar");
    //! \brief result
    using type = t2tot2<getSpaceDimension<TensorType1>(),
                        derivative_type<TensorNumType<TensorType1>,
                                        TensorNumType<TensorType2>>>;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief a base for stensor or classes acting like stensor.
   * \param Child : child class
   */
  template <typename Child>
  struct t2tot2_base {
    /*!
     * Assignement operator
     */
    template <typename T2toT2Type>
    TFEL_MATH_INLINE std::enable_if_t<
        implementsT2toT2Concept<T2toT2Type>() &&
            getSpaceDimension<Child>() == getSpaceDimension<T2toT2Type>() &&
            tfel::typetraits::IsAssignableTo<
                MathObjectNumType<T2toT2Type>,
                MathObjectNumType<Child>>::cond,
        Child&>
    operator=(const T2toT2Type&);
    //! Assignement operator
    template <typename T2toT2Type>
    TFEL_MATH_INLINE std::enable_if_t<
        implementsT2toT2Concept<T2toT2Type>() &&
            getSpaceDimension<Child>() == getSpaceDimension<T2toT2Type>() &&
            tfel::typetraits::IsAssignableTo<
                MathObjectNumType<T2toT2Type>,
                MathObjectNumType<Child>>::cond,
        Child&>
    operator+=(const T2toT2Type&);
    //! Assignement operator
    template <typename T2toT2Type>
    TFEL_MATH_INLINE std::enable_if_t<
        implementsT2toT2Concept<T2toT2Type>() &&
            getSpaceDimension<Child>() == getSpaceDimension<T2toT2Type>() &&
            tfel::typetraits::IsAssignableTo<
                MathObjectNumType<T2toT2Type>,
                MathObjectNumType<Child>>::cond,
        Child&>
    operator-=(const T2toT2Type&);
    /*!
     * operator*=
     */
    template <typename T2>
    TFEL_MATH_INLINE std::enable_if_t<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same_v<
                typename ResultType<MathObjectNumType<Child>,
                                    T2,
                                    OpMult>::type,
                MathObjectNumType<Child>>,
        Child&>
    operator*=(const T2);
    /*!
     * operator/=
     */
    template <typename T2>
    TFEL_MATH_INLINE std::enable_if_t<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same_v<
                typename ResultType<MathObjectNumType<Child>,
                                    T2,
                                    OpDiv>::type,
                MathObjectNumType<Child>>,
        Child&>
    operator/=(const T2);
  };  // end of struct t2tot2_base

  template <unsigned short N, typename T>
  struct t2tot2
      : public T2toT2Concept<t2tot2<N, T>>,
        public fsarray<TensorDimeToSize<N>::value * TensorDimeToSize<N>::value,
                       T>,
        public t2tot2_base<t2tot2<N, T>> {
    /*!
     * \return a t2tot2 which acts on a tensor as a rotation of the coordinate
     * system
     * \param[in] r : rotation matrix
     */
    static t2tot2<N, tfel::typetraits::base_type<T>> fromRotationMatrix(
        const rotation_matrix<T>&);
    /*!
     * \param[in] B : second tensor of the product
     * \return the left part of the derivative of a tensor product
     */
    template <typename TensorType>
    static TFEL_MATH_INLINE
        std::enable_if_t<implementsTensorConcept<TensorType>() &&
                             getSpaceDimension<TensorType>() == N &&
                             tfel::typetraits::IsAssignableTo<
                                 MathObjectNumType<TensorType>,
                                 T>::cond,
                         Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>>
        tpld(const TensorType&);
    /*!
     * \param[in] B : second tensor of the product
     * \param[in] C : derivative of the first tensor
     * \return the left part of the derivative of a tensor product
     */
    template <typename TensorType, typename T2toT2Type>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsTensorConcept<TensorType>() &&
            implementsT2toT2Concept<T2toT2Type>() &&
            getSpaceDimension<TensorType>() == N &&
            getSpaceDimension<T2toT2Type>() == N &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    MathObjectNumType<TensorType>,
                    MathObjectNumType<T2toT2Type>,
                    OpMult>::Result,
                T>::cond,
        Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>>
    tpld(const TensorType&, const T2toT2Type&);
    /*!
     * \param[in] A : first tensor of the product
     * \return the right part of the derivative of a tensor product
     */
    template <typename TensorType>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsTensorConcept<TensorType>() &&
            getSpaceDimension<TensorType>() == N &&
            tfel::typetraits::IsAssignableTo<
                MathObjectNumType<TensorType>,
                T>::cond,
        Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>>
    tprd(const TensorType&);
    /*!
     * \param[in] A : first tensor of the product
     * \param[in] C : derivative of the first tensor
     * \return the right part of the derivative of a tensor product
     */
    template <typename TensorType, typename T2toT2Type>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsTensorConcept<TensorType>() &&
            implementsT2toT2Concept<T2toT2Type>() &&
            getSpaceDimension<TensorType>() == N &&
            getSpaceDimension<T2toT2Type>() == N &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    MathObjectNumType<TensorType>,
                    MathObjectNumType<T2toT2Type>,
                    OpMult>::Result,
                T>::cond,
        Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>>
    tprd(const TensorType&, const T2toT2Type&);
    /*!
     * \return the derivative of the transpose of a tensor with respect of
     * this tensor
     */
    static TFEL_MATH_INLINE TFEL_CONSTEXPR
        t2tot2<N, tfel::typetraits::base_type<T>>
        transpose_derivative();
    //! \return the identity
    static TFEL_MATH_INLINE TFEL_CONSTEXPR
        t2tot2<N, tfel::typetraits::base_type<T>>
        Id();
    //! \return Id2^Id2, where Id2 is the identity tensor
    static TFEL_MATH_INLINE TFEL_CONSTEXPR
        t2tot2<N, tfel::typetraits::base_type<T>>
        IxI();
    /*!
     * \return Id4-Id2^Id2/3, where Id4 is the identity of t2tot2 and
     * Id2 is the identity tensor
     */
    static TFEL_MATH_INLINE TFEL_CONSTEXPR
        t2tot2<N, tfel::typetraits::base_type<T>>
        K();
    //! This is a T2toT2 concept requirement.
    typedef EmptyRunTimeProperties RunTimeProperties;
    /*!
     * \brief Default Constructor
     */
    TFEL_MATH_INLINE explicit constexpr t2tot2();
    /*!
     * \brief Default Constructor
     * \param T, value used to initialise the components of the t2tot2
     */
    template <typename T2,
              std::enable_if_t<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                               bool> = true>
    TFEL_MATH_INLINE explicit constexpr t2tot2(const T2&);
    /*!
     * \brief Copy Constructor
     */
    TFEL_MATH_INLINE constexpr t2tot2(const t2tot2<N, T>&);
    // Copy Constructor
    template <typename T2, typename Op>
    TFEL_MATH_INLINE t2tot2(const Expr<t2tot2<N, T2>, Op>&);
    /*!
     * \brief default constructor
     * \param[in] v : values
     */
    template <typename T2,
              std::enable_if_t<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                               bool> = true>
    TFEL_MATH_INLINE constexpr t2tot2(const std::initializer_list<T2>&);
    /*!
     * \brief constructor from a t2tost2
     * \param[in] v : values
     */
    template <typename T2toST2Type,
              std::enable_if_t<
                  ((implementsT2toST2Concept<T2toST2Type>()) &&
                   (tfel::typetraits::
                        IsAssignableTo<T2toST2NumType<T2toST2Type>, T>::cond) &&
                   (getSpaceDimension<T2toST2Type>() == N)),
                  bool> = true>
    TFEL_MATH_INLINE t2tot2(const T2toST2Type&);
    //! assignement operator
    TFEL_MATH_INLINE t2tot2& operator=(const t2tot2&);
    /*!
     * Import values
     */
    template <typename T2>
    TFEL_MATH_INLINE2
        std::enable_if_t<tfel::typetraits::IsSafelyReinterpretCastableTo<
                             T2,
                             tfel::typetraits::base_type<T>>::cond,
                         void>
        import(const T2* const);

    /*!
     * Assignement operator
     */
    using t2tot2_base<t2tot2>::operator=;

    //! access operator
    TFEL_MATH_INLINE constexpr const T& operator()(const unsigned short,
                                                   const unsigned short) const;
    //! access operator
    TFEL_MATH_INLINE T& operator()(const unsigned short, const unsigned short);
    /*!
     * Return the RunTimeProperties of the tvector
     * \return tvector::RunTimeProperties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const;

    template <typename InputIterator>
    TFEL_MATH_INLINE2 void copy(const InputIterator src);

  };  // end of struct t2tot2

  /*!
   * \return change the basis of a t2tot2
   * \param[in] s : t2tot2
   * \param[in] r : rotation matrix
   */
  template <typename T2toT2Type>
  TFEL_MATH_INLINE2 std::enable_if_t<
      implementsT2toT2Concept<T2toT2Type>(),
      t2tot2<getSpaceDimension<T2toT2Type>(), T2toT2NumType<T2toT2Type>>>
  change_basis(const T2toT2Type&,
               const rotation_matrix<T2toT2NumType<T2toT2Type>>&);

  /*!
   * \return compute the derivative of the velocity gradient
   * \param[in] F : deformation gradient
   */
  template <typename TensorType>
  TFEL_MATH_INLINE2 std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      t2tot2<getSpaceDimension<TensorType>(), TensorNumType<TensorType>>>
  computeVelocityGradientDerivative(const TensorType&);

  /*!
   * \return compute the derivative of the spin rate
   * \param[in] F : deformation gradient
   */
  template <typename TensorType>
  TFEL_MATH_INLINE2 std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      t2tot2<getSpaceDimension<TensorType>(), TensorNumType<TensorType>>>
  computeSpinRateDerivative(const TensorType&);
  /*!
   * \brief compute the second derivative of the determinant of a
   * symmetric tensor
   * \param[in] s: tensor
   */
  template <typename TensorType>
  std::enable_if_t<
      implementsTensorConcept<TensorType>() &&
          tfel::typetraits::IsScalar<TensorNumType<TensorType>>::cond,
      t2tot2<getSpaceDimension<TensorType>(), TensorNumType<TensorType>>>
  computeDeterminantSecondDerivative(const TensorType&);

  template <typename T, typename T2toST2Type>
  std::enable_if_t<((implementsT2toST2Concept<T2toST2Type>()) &&
                    (tfel::typetraits::
                         IsAssignableTo<T2toST2NumType<T2toST2Type>, T>::cond)),
                   void>
  convert(t2tot2<getSpaceDimension<T2toST2Type>(), T>&, const T2toST2Type&);

}  // end of namespace tfel::math

namespace tfel::typetraits {

  template <unsigned short N, typename T2, typename T>
  struct IsAssignableTo<tfel::math::t2tot2<N, T2>, tfel::math::t2tot2<N, T>> {
    /*!
     *  Result
     */
    static constexpr bool cond = IsAssignableTo<T2, T>::cond;
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/T2toT2/t2tot2.ixx"
#include "TFEL/Math/T2toT2/t2tot2ResultType.hxx"

#endif /* LIB_TFEL_MATH_T2TOT2_HXX */
