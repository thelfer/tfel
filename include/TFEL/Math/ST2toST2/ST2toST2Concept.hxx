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

  //! a simple alias
  template <typename A>
  struct ST2toST2TransposeExpr;

  /*!
   * \class ST2toST2Tag
   * \brief Helper class to characterise st2tost2.
   */
  struct ST2toST2Tag {};

  template <class T>
  struct ST2toST2Concept {
    typedef ST2toST2Tag ConceptTag;

   protected:
    ST2toST2Concept() = default;
    ST2toST2Concept(ST2toST2Concept&&) = default;
    ST2toST2Concept(const ST2toST2Concept&) = default;
    ST2toST2Concept& operator=(const ST2toST2Concept&) = default;
    ~ST2toST2Concept() = default;
  };

  /*!
   * \brief an helper function which returns if the given type implements the
   * `ST2toST2Concept`.
   * \tparam ST2toST2Type: type tested
   */
  template <typename ST2toST2Type>
  constexpr bool implementsST2toST2Concept() {
    return tfel::meta::implements<ST2toST2Type, ST2toST2Concept>();
  }  // end of implementsST2toST2Concept

  template <typename T>
  struct ConceptRebind<ST2toST2Tag, T> {
    typedef ST2toST2Concept<T> type;
  };

  template <typename ST2toST2Type>
  std::enable_if_t<
      implementsST2toST2Concept<ST2toST2Type>(),
      typename tfel::typetraits::AbsType<numeric_type<ST2toST2Type>>::type>
  abs(const ST2toST2Type&);

  /*!
   * \brief performs the push_forward of a st2tost2:
   * \[
   * Ct_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C_{mnpq}
   * \]
   * \param[out] Ct: result
   * \param[in] C: input
   * \param[in] F: deformation gradient
   */
  template <typename ST2toST2Type, typename ST2toST2Type2, typename TensorType>
  std::enable_if_t<implementsST2toST2Concept<ST2toST2Type>() &&
                       implementsST2toST2Concept<ST2toST2Type2>() &&
                       implementsTensorConcept<TensorType>() &&
                       getSpaceDimension<ST2toST2Type>() == 1u &&
                       getSpaceDimension<ST2toST2Type2>() == 1u &&
                       getSpaceDimension<TensorType>() == 1u,
                   void>
  push_forward(ST2toST2Type&, const ST2toST2Type2&, const TensorType&);
  /*!
   * \brief performs the push_forward of a st2tost2:
   * \[
   * Ct_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C_{mnpq}
   * \]
   * \param[out] Ct: result
   * \param[in] C: input
   * \param[in] F: deformation gradient
   */
  template <typename ST2toST2Type, typename ST2toST2Type2, typename TensorType>
  std::enable_if_t<implementsST2toST2Concept<ST2toST2Type>() &&
                       implementsST2toST2Concept<ST2toST2Type2>() &&
                       implementsTensorConcept<TensorType>() &&
                       getSpaceDimension<ST2toST2Type>() == 2u &&
                       getSpaceDimension<ST2toST2Type2>() == 2u &&
                       getSpaceDimension<TensorType>() == 2u,
                   void>
  push_forward(ST2toST2Type&, const ST2toST2Type2&, const TensorType&);
  /*!
   * \brief performs the push_forward of a st2tost2:
   * \[
   * Ct_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C_{mnpq}
   * \]
   * \param[out] Ct: result
   * \param[in] C: input
   * \param[in] F: deformation gradient
   */
  template <typename ST2toST2Type, typename ST2toST2Type2, typename TensorType>
  std::enable_if_t<implementsST2toST2Concept<ST2toST2Type>() &&
                       implementsST2toST2Concept<ST2toST2Type2>() &&
                       implementsTensorConcept<TensorType>() &&
                       getSpaceDimension<ST2toST2Type>() == 3u &&
                       getSpaceDimension<ST2toST2Type2>() == 3u &&
                       getSpaceDimension<TensorType>() == 3u,
                   void>
  push_forward(ST2toST2Type&, const ST2toST2Type2&, const TensorType&);
  /*!
   * \return a transposed view
   */
  template <typename ST2toST2Type>
  TFEL_MATH_INLINE auto transpose(ST2toST2Type&& t) -> std::enable_if_t<
      implementsST2toST2Concept<ST2toST2Type>(),
      Expr<EvaluationResult<ST2toST2Type>, ST2toST2TransposeExpr<decltype(t)>>>;
  /*!
   * \return the determinant of a `st2tost2`
   * \param[in] s: fourth order tensor
   */
  template <typename ST2toST2Type>
  std::enable_if_t<
      implementsST2toST2Concept<ST2toST2Type>() &&
          (getSpaceDimension<ST2toST2Type>() == 1u) &&
          isScalar<numeric_type<ST2toST2Type>>(),
      typename ComputeUnaryResult<numeric_type<ST2toST2Type>, Power<3>>::Result>
  det(const ST2toST2Type&);
  /*!
   * \return the determinant of a `st2tost2`
   * \param[in] s: fourth order tensor
   */
  template <typename ST2toST2Type>
  std::enable_if_t<implementsST2toST2Concept<ST2toST2Type>() &&
                       ((getSpaceDimension<ST2toST2Type>() == 2u) ||
                        (getSpaceDimension<ST2toST2Type>() == 3u)) &&
                       isScalar<numeric_type<ST2toST2Type>>(),
                   typename ComputeUnaryResult<
                       numeric_type<ST2toST2Type>,
                       Power<getSpaceDimension<ST2toST2Type>()>>::Result>
  det(const ST2toST2Type&);

}  // end of namespace tfel::math

#include "TFEL/Math/ST2toST2/ST2toST2Concept.ixx"

#endif /* LIB_TFEL_MATH_ST2TOST2CONCEPT_HXX */
