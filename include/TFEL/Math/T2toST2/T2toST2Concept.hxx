/*!
 * \file   include/TFEL/Math/T2toST2/T2toST2Concept.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOST2CONCEPT_HXX
#define LIB_TFEL_MATH_T2TOST2CONCEPT_HXX 1

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel::math {

  /*!
   * \class T2toST2Tag
   * \brief Helper class to characterise t2tost2.
   */
  struct T2toST2Tag {};
  /*!
   * \brief an helper class that simply exposes publically a member named
   * ConceptTag as an alias to T2toST2Tag.
   *
   * The main reason for this alias is to properly implement the `ConceptRebind`
   * metafunction.
   */
  template <class T>
  struct T2toST2ConceptBase {
    using ConceptTag = T2toST2Tag;
  };
  /*!
   * \brief definition of the T2toST2Concept
   * a class matching the stensor concept must expose the `T2toST2Tag` and have
   * access operators.
   */
  template <typename T>
  concept T2toST2Concept =
      (std::is_same_v<typename std::decay_t<T>::ConceptTag, T2toST2Tag>)&&  //
      (requires(const T t, const unsigned short i, const unsigned short j) {
        t(i, j);
      });
  //! \brief partial specialisation for T2toST2
  template <typename Type>
  struct ConceptRebind<T2toST2Tag, Type> {
    using type = T2toST2ConceptBase<Type>;
  };
  /*!
   * \return the sum of the absolute values of all components of an
   * linear application transforming a symmetric tensor in an unsymmetric tensor
   * \param[in] t: linear application transforming a symmetric tensor in an
   * unsymmetric tensor
   */
  TFEL_HOST_DEVICE constexpr void abs(const T2toST2Concept auto&) noexcept;
  /*!
   * \brief compute de derivative of the push-forward of a symmetric
   * second order tensor with respect to the deformation gradient, i.e.
   * it computes the derivative of F . S . transpose(F) with respect to F
   * assuming that S is constant
   *
   * \param[out] : derivative of the push-forward symmetric tensor
   * \param[in]  : orginal tensor
   * \param[in]  : deformation gradient
   */
  template <T2toST2Concept T2toST2ResultType,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void
  computePushForwardDerivativeWithRespectToDeformationGradient(
      T2toST2ResultType&, const StensorType&, const TensorType&) noexcept  //
      requires(tfel::typetraits::IsFundamentalNumericType<
               numeric_type<TensorType>>::cond&&
                   isAssignableTo<numeric_type<StensorType>,
                                  numeric_type<T2toST2ResultType>>());
  /*!
   * \brief compute the Cauchy stress derivative from the Kirchhoff
   * stress derivative with respect to the deformation gradient
   * \param[out] ds   : Cauchy stress derivative
   * \param[in]  dt_K : Kirchhoff stress derivative
   * \param[in]  s    : Cauchy stress
   * \param[in]  F    : deformation gradient
   */
  template <T2toST2Concept T2toST2ResultType,
            T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void
  computeCauchyStressDerivativeFromKirchhoffStressDerivative(
      T2toST2ResultType&,
      const T2toST2Type&,
      const StensorType&,
      const TensorType&) noexcept  //
      requires(
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<T2toST2Type>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<StensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<TensorType>() &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                      numeric_type<StensorType>,
                                                      OpPlus>::Result,
                         numeric_type<T2toST2ResultType>>());
  /*!
   * \brief compute the Kirchhoff stress derivative from the Cauchy
   * stress derivative with respect to the deformation gradient
   * \param[out] dt_K : Kirchhoff stress derivative
   * \param[in]  ds   : Cauchy stress derivative
   * \param[in]  s    : Cauchy stress
   * \param[in]  F    : deformation gradient
   */
  template <T2toST2Concept T2toST2ResultType,
            T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void
  computeKirchhoffStressDerivativeFromCauchyStressDerivative(
      T2toST2ResultType&,
      const T2toST2Type&,
      const StensorType&,
      const TensorType&) noexcept  //
      requires(
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<T2toST2Type>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<StensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<TensorType>() &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                      numeric_type<StensorType>,
                                                      OpPlus>::Result,
                         numeric_type<T2toST2ResultType>>());
  /*!
   * \brief an helper function which returns if the given type implements the
   * `T2toST2Concept`.
   * \tparam T2toST2Type: type tested
   */
  template <T2toST2Concept T2toST2Type>
  [[deprecated]] TFEL_HOST_DEVICE constexpr bool implementsT2toST2Concept() {
    return T2toST2Concept<T2toST2Type>;
  }  // end of implementsT2toST2Concept

}  // end of namespace tfel::math

#include "TFEL/Math/T2toST2/T2toST2Concept.ixx"

#endif /* LIB_TFEL_MATH_T2TOST2CONCEPT_HXX */
