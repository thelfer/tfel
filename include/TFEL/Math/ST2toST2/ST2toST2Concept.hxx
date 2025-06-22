/*!
 * \file  include/TFEL/Math/ST2toST2/ST2toST2Concept.hxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOST2CONCEPT_HXX
#define LIB_TFEL_MATH_ST2TOST2CONCEPT_HXX 1

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel::math {

  /*!
   * \class ST2toST2Tag
   * \brief Helper class to characterise st2tost2.
   */
  struct ST2toST2Tag {};
  /*!
   * \brief an helper class that simply exposes publically a member named
   * ConceptTag as an alias to ST2toST2Tag.
   *
   * The main reason for this alias is to properly implement the `ConceptRebind`
   * metafunction.
   */
  template <typename T>
  struct ST2toST2ConceptBase {
    using ConceptTag = ST2toST2Tag;
  };
  /*!
   * \brief definition of the ST2toST2Concept
   * a class matching the stensor concept must expose the `ST2toST2Tag` and have
   * access operators.
   */
  template <typename T>
  concept ST2toST2Concept =
      (std::is_same_v<typename std::decay_t<T>::ConceptTag, ST2toST2Tag>)&&  //
      (requires(const T t, const unsigned short i, const unsigned short j) {
        t(i, j);
      });
  /*!
   * \brief partial specialisation for fourth order tensor transforming a
   * symmetric tensor in another symmetric tensor
   */
  template <typename T>
  struct ConceptRebind<ST2toST2Tag, T> {
    //! \brief a simple alias
    using type = ST2toST2ConceptBase<T>;
  };
  // \forward declaration
  template <ST2toST2Concept A>
  struct ST2toST2TransposeExpr;
  /*!
   * \return the sum of the absolute values of all components of an
   * linear application transforming a symmetric tensor in a symmetric tensor
   * \param[in] s: linear application transforming a symmetric tensor in a
   * symmetric tensor
   */
  TFEL_HOST_DEVICE constexpr auto abs(const ST2toST2Concept auto&) noexcept;

  /*!
   * \brief This function computes the transpose of a `st2tost2`
   * as \f[ {}^tA_{ij}=A_{ji}\f] in Voigt notation, which also
   * corresponds to \f[ {}^tA_{ijkl}=A_{lkji}=A_{klij}\f].
   * \return a `st2tost2`
   * \param[in] t: `st2tost2`
   */
  TFEL_HOST_DEVICE constexpr auto transpose(ST2toST2Concept auto&&) noexcept;

  /*!
   * \brief This function computes the trace of a `st2tost2`
   * as \f[ Tr\left(A\right)=\sum_i A_{ii}\f] in Voigt notation, which also
   * corresponds to \f[
   * Tr\left(A\right)=I::A=\frac{1}{2}\sum_{i,j}(A_{ijij}+A_{ijji})\f] \return
   * the trace \param[in] A: `st2tost2`
   */
  TFEL_HOST_DEVICE constexpr auto trace(const ST2toST2Concept auto&) noexcept;

  /*!
   * \brief This function computes the quadruple contraction of two `st2tost2`
   * as \f[ A::B=\sum_{i,j} A_{ij}B_{ji}\f] in Voigt notation, which also
   * corresponds to \f[
   * A::B=\sum_{i,j,k,l}A_{ijkl}B_{lkji}=\sum_{i,j,k,l}A_{ijkl}B_{klij}\f]
   * \return the quadruple contraction scalar
   * \param[in] A,B: `st2tost2`
   */
  TFEL_HOST_DEVICE constexpr auto quaddot(const ST2toST2Concept auto&,
                                          const ST2toST2Concept auto&) noexcept;

  /*!
   * \brief This function computes the norm of a `st2tost2`
   * as \f[ ||A||=\sqrt{\frac{1}{N}{}^tA::A}\f] where \f[N\f] is
   * the size of the tensor (for example, \f[6\f] for a dimension 3,
   * and \f[4\f] for a dimension 2).
   * \f[ ||A||=\sqrt{\frac{1}{N}\sum_{i,j,k,l}A_{ijkl}A_{ijkl}}\f]
   * or also \f[ ||A||=\sqrt{\frac{1}{N}\sum_{i,j} A_{ij}A_{ij}}\f] in Voigt
   * notation. The factor \f[\frac 1N\f] ensures that the norm of \f[I\f] is 1.
   * \return the norm
   * \param[in] A: `st2tost2`
   */
  TFEL_HOST_DEVICE constexpr auto norm(const ST2toST2Concept auto&) noexcept;

  /*!
   * \return the determinant of a `st2tost2`
   * \param[in] s: fourth order tensor
   */
  TFEL_HOST_DEVICE constexpr auto det(const ST2toST2Concept auto&) noexcept;

  /*!
   * \brief compute de derivative of the push-forward of a symmetric
   * second order tensor with respect to the deformation gradient, i.e.
   * it computes the derivative of F . S . transpose(F) with respect to S
   * assuming that F is constant
   *
   * \param[out] r: derivative of the push-forward symmetric tensor
   * \param[in]  F: deformation gradient
   */
  template <ST2toST2Concept ST2toST2ResultType, TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void computePushForwardDerivative(
      ST2toST2ResultType&,
      const TensorType&) noexcept  //
      requires(tfel::typetraits::IsFundamentalNumericType<
               numeric_type<TensorType>>::cond);
  /*!
   * \brief performs the push_forward of a st2tost2:
   * \[
   * Ct_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C_{mnpq}
   * \]
   * \param[out] Ct: result
   * \param[in] C: input
   * \param[in] F: deformation gradient
   */
  template <ST2toST2Concept ST2toST2Type,
            ST2toST2Concept ST2toST2Type2,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void push_forward(ST2toST2Type&,
                                               const ST2toST2Type2&,
                                               const TensorType&) noexcept  //
      requires(getSpaceDimension<ST2toST2Type>() ==
                   getSpaceDimension<ST2toST2Type2>() &&
               getSpaceDimension<ST2toST2Type>() ==
                   getSpaceDimension<TensorType>());

  /*!
   * \brief an helper function which returns if the given type implements the
   * `ST2toST2Concept`.
   * \tparam ST2toST2Type: type tested
   * \note function given for backward compatibility with versions prior
   * to 5.0
   */
  template <typename ST2toST2Type>
  [[deprecated]] TFEL_HOST_DEVICE constexpr bool implementsST2toST2Concept() {
    return ST2toST2Concept<ST2toST2Type>;
  }  // end of implementsST2toST2Concept

}  // end of namespace tfel::math

#include "TFEL/Math/ST2toST2/ST2toST2Concept.ixx"

#endif /* LIB_TFEL_MATH_ST2TOST2CONCEPT_HXX */
