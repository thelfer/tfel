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

  template <typename T>
  struct StensorConcept {
    using ConceptTag = StensorTag;

   protected:
    constexpr StensorConcept() = default;
    constexpr StensorConcept(StensorConcept&&) = default;
    constexpr StensorConcept(const StensorConcept&) = default;
    constexpr StensorConcept& operator=(const StensorConcept&) = default;
    ~StensorConcept() = default;
  };

  /*!
   * \brief an helper function which returns if the given type implements the
   * `StensorConcept`.
   * \tparam StensorType: type tested
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE constexpr bool implementsStensorConcept() {
    return tfel::meta::implements<StensorType, StensorConcept>();
  }  // end of implementsStensorConcept

  //! \brief a simple alias for backward compatibility with versions prior
  //! to 4.0
  template <typename StensorType>
  using StensorTraits =
      std::conditional_t<implementsStensorConcept<StensorType>(),
                         MathObjectTraits<StensorType>,
                         MathObjectTraits<tfel::meta::InvalidType>>;

  //! \brief a simple alias for backward compatibility with versions prior
  //! to 4.0
  template <typename StensorType>
  using StensorNumType =
      std::conditional_t<implementsStensorConcept<StensorType>(),
                         numeric_type<StensorType>,
                         MathObjectTraits<tfel::meta::InvalidType>>;

  //! partial specialisation for symmetric tensors
  template <typename Type>
  struct ConceptRebind<StensorTag, Type> {
    //! \brief a simple alias
    using type = StensorConcept<Type>;
  };
  /*!
   * \return the sum of the absolute values of all components of a symmetric
   * tensor
   * \param[in] s: symmetric tensor
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE TFEL_HOST_DEVICE constexpr std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      typename tfel::typetraits::AbsType<numeric_type<StensorType>>::type>
  abs(const StensorType&);
  /*!
   * \return the trace of a symmetric tensor
   * \param[in] s: symmetric tensor
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE TFEL_HOST_DEVICE constexpr std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      numeric_type<StensorType>>
  trace(const StensorType&);
  /*!
   * \return the von Mises stress of a symmetric tensor
   * \param[in] s: symmetric tensor
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2
      TFEL_HOST_DEVICE constexpr std::enable_if_t<
          implementsStensorConcept<StensorType>(),
          numeric_type<StensorType>>
      sigmaeq(const StensorType&);
  /*!
   * \return the deviator of a symmetric tensor
   */
  template <typename StensorType>
  TFEL_HOST_DEVICE TFEL_HOST_DEVICE constexpr std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      EvaluationResult<StensorType>>
  deviator(const StensorType&);

  /*!
   * \brief return the size of a symmetric tensor for the given
   * dimension
   * \param[in] d: space dimension
   */
  TFEL_HOST TFELMATH_VISIBILITY_EXPORT unsigned short getStensorSize(
      const unsigned short);
  /*!
   * \brief compute the derivative of the determinant with respect
   * to its argument.
   * \param[out] dJ: determinant derivative
   * \param[in]  s: argument
   */
  template <typename StensorResultType, typename StensorType>
  TFEL_HOST_DEVICE TFEL_HOST_DEVICE constexpr std::enable_if_t<
      (implementsStensorConcept<StensorResultType>() &&
       implementsStensorConcept<StensorType>() &&
       isAssignableTo<typename ComputeUnaryResult<numeric_type<StensorType>,
                                                  Power<2>>::Result,
                      numeric_type<StensorResultType>>()),
      void>
  computeDeterminantDerivative(StensorResultType&, const StensorType&);
  /*!
   * \brief compute the derivative of the determinant with respect
   * to its argument.
   * \param[out] dJ: determinant derivative
   * \param[in]  s:  argument
   */
  template <typename StensorResultType, typename StensorType>
  TFEL_HOST_DEVICE TFEL_HOST_DEVICE constexpr std::enable_if_t<
      (implementsStensorConcept<StensorResultType>() &&
       implementsStensorConcept<StensorType>() &&
       isAssignableTo<typename ComputeUnaryResult<numeric_type<StensorType>,
                                                  Power<2>>::Result,
                      numeric_type<StensorResultType>>()),
      void>
  computeDeviatorDeterminantDerivative(StensorResultType&, const StensorType&);

}  // end of namespace tfel::math

#include "TFEL/Math/Stensor/StensorConcept.ixx"

#endif /* LIB_TFEL_MATH_STENSORCONCEPT_HXX */
