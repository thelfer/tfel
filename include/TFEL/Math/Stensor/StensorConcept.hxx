
/*!
 * \file   include/TFEL/Math/Stensor/StensorConcept.hxx
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORCONCEPT_HXX
#define LIB_TFEL_MATH_STENSORCONCEPT_HXX 1

#include <concepts>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/power.hxx"

namespace tfel::math {

  /*!
   * \class StensorTag
   * \brief Helper class to characterise stensors.
   */
  struct StensorTag {};  // end of StensorTag
  /*!
   * \brief an helper class that simply exposes publically a member named
   * ConceptTag as an alias to StensorTag.
   *
   * The main reason for this alias is to properly implement the `ConceptRebind`
   * metafunction.
   */
  template <typename T>
  struct StensorConceptBase {
    //! \brief an alias to StensorTag
    using ConceptTag = StensorTag;
  };

  /*!
   * \brief definition of the StensorConcept
   * a class matching the stensor concept must expose the `StensorTag` and have
   * access operators.
   */
  template <typename T>
  concept StensorConcept =
      (std::is_same_v<typename std::decay_t<T>::ConceptTag, StensorTag>)&&  //
      (requires(const T t, const unsigned short i) { t[i]; }) &&            //
      (requires(const T t, const unsigned short i) { t(i); });

  //! \brief partial specialisation for symmetric tensors
  template <typename Type>
  struct ConceptRebind<StensorTag, Type> {
    //! \brief a simple alias
    using type = StensorConceptBase<Type>;
  };

  /*!
   * \return the sum of the absolute values of all components of a symmetric
   * tensor
   * \param[in] s: symmetric tensor
   */
  TFEL_HOST_DEVICE constexpr auto abs(const StensorConcept auto&) noexcept;
  /*!
   * \return the trace of a symmetric tensor
   * \param[in] s: symmetric tensor
   */
  TFEL_HOST_DEVICE constexpr auto trace(const StensorConcept auto&) noexcept;
  /*!
   * \return the von Mises stress of a symmetric tensor
   * \param[in] s: symmetric tensor
   */
  TFEL_HOST_DEVICE constexpr auto sigmaeq(const StensorConcept auto&) noexcept;
  /*!
   * \return the deviator of a symmetric tensor
   */
  TFEL_HOST_DEVICE constexpr auto deviator(const StensorConcept auto&) noexcept;
  /*!
   * \brief compute the derivative of the determinant with respect
   * to its argument.
   * \param[out] dJ: determinant derivative
   * \param[in]  s: argument
   */
  TFEL_HOST_DEVICE constexpr auto computeDeterminantDerivative(
      StensorConcept auto&, const StensorConcept auto&) noexcept;
  /*!
   * \brief compute the derivative of the determinant with respect
   * to its argument.
   * \param[out] dJ: determinant derivative
   * \param[in]  s:  argument
   */
  TFEL_HOST_DEVICE constexpr auto computeDeviatorDeterminantDerivative(
      StensorConcept auto&, const StensorConcept auto&) noexcept;
  /*!
   * \brief return the size of a symmetric tensor for the given
   * dimension
   * \param[in] d: space dimension
   */
  TFEL_HOST TFELMATH_VISIBILITY_EXPORT unsigned short getStensorSize(
      const unsigned short);

  /*!
   * \brief This function returns the Voigt index of two indices (beginning from
   * 0). Hence, the correspondence is, in 3d: \f[(0,0)->0, (1,1)->1, (2,2)->2,
   * (0,1) or (1,0) -> 3, (0,2) or (2,0) -> 4, (1,2) or (2,1)-> 5\f].
   * In 2d, it is \f[(0,0)->0, (1,1)->1, (2,2)->2,
   * (0,1) or (1,0) -> 3\f].
   * In 1d, \f[(0,0)->0, (1,1)->1, (2,2)->2\f].
   * Be careful to the Voigt indices between 3 and 5, which are different from
   * the usual convention: \f[\varepsilon_{12}\f] is usually the last component
   * of the vector \f[\varepsilon\f] in the usual Voigt notation, but here it is
   * situated as the fourth component, in order that the vector in dimension 2
   * is just a truncation of the vector in dimension 3. \return an unsigned
   * short \param[in] i,j: unsigned short
   */
  template <unsigned short N>
  TFEL_HOST_DEVICE constexpr unsigned short VoigtIndex(const unsigned short,
                                                       const unsigned short);

  /*!
   * \brief a simple alias for backward compatibility with versions prior
   * to 4.0
   */
  template <typename StensorType>
  using StensorTraits =
      std::conditional_t<StensorConcept<StensorType>,
                         MathObjectTraits<StensorType>,
                         MathObjectTraits<tfel::meta::InvalidType>>;

  /*!
   * \brief a simple alias for backward compatibility with versions prior
   * to 4.0
   */
  template <typename StensorType>
  using StensorNumType =
      std::conditional_t<StensorConcept<StensorType>,
                         numeric_type<StensorType>,
                         MathObjectTraits<tfel::meta::InvalidType>>;

  /*!
   * \brief an helper function which returns if the given type implements the
   * `StensorConcept`.
   * \tparam StensorType: type tested
   * \note function given for backward compatibility with versions prior
   * to 5.0
   */
  template <typename StensorType>
  [[deprecated]] TFEL_HOST_DEVICE constexpr bool implementsStensorConcept() {
    return StensorConcept<StensorType>;
  }  // end of implementsStensorConcept

}  // end of namespace tfel::math

#include "TFEL/Math/Stensor/StensorConcept.ixx"

#endif /* LIB_TFEL_MATH_STENSORCONCEPT_HXX */
