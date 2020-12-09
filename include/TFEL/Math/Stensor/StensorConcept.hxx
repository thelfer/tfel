/*!
 * \file   include/TFEL/Math/Stensor/StensorConcept.hxx
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORCONCEPT_HXX
#define LIB_TFEL_MATH_STENSORCONCEPT_HXX 1

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/power.hxx"

namespace tfel::math {

  template <typename StensorType>
  struct StensorTraits {
    typedef tfel::meta::InvalidType NumType;
    typedef unsigned short IndexType;
    static constexpr unsigned short dime = 0u;
  };

  template <typename StensorType>
  using StensorNumType = typename StensorTraits<StensorType>::NumType;

  template <typename StensorType>
  using StensorBaseType =
      tfel::typetraits::base_type<StensorNumType<StensorType>>;

  /*!
   * \class StensorTag
   * \brief Helper class to characterise stensors.
   */
  struct StensorTag {};  // end of StensorTag

  template <typename T>
  struct StensorConcept {
    using ConceptTag = StensorTag;

    StensorNumType<T> operator()(const unsigned short) const;

    StensorNumType<T> operator[](const unsigned short) const;

   protected:
    StensorConcept() = default;
    StensorConcept(StensorConcept&&) = default;
    StensorConcept(const StensorConcept&) = default;
    StensorConcept& operator=(const StensorConcept&) = default;
    ~StensorConcept() = default;
  };

  template <typename StensorType>
  constexpr bool implementsStensorConcept() {
    return tfel::meta::implements<StensorType, StensorConcept>();
  }  // end of implementsStensorConcept

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
  std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      typename tfel::typetraits::AbsType<StensorNumType<StensorType>>::type>
  abs(const StensorType&);
  /*!
   * \return the trace of a symmetric tensor
   * \param[in] s: symmetric tensor
   */
  template <typename StensorType>
  TFEL_MATH_INLINE std::enable_if_t<implementsStensorConcept<StensorType>(),
                                    StensorNumType<StensorType>>
  trace(const StensorType&);
  /*!
   * \return the von Mises stress of a symmetric tensor
   * \param[in] s: symmetric tensor
   */
  template <typename StensorType>
  TFEL_MATH_INLINE2 std::enable_if_t<implementsStensorConcept<StensorType>(),
                                     StensorNumType<StensorType>>
  sigmaeq(const StensorType&);
  /*!
   * \return the deviator of a symmetric tensor
   */
  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>(),
                   EvaluationResult<StensorType>>
  deviator(const StensorType&);

  /*!
   * \brief return the size of a symmetric tensor for the given
   * dimension
   * \param[in] d: space dimension
   */
  TFELMATH_VISIBILITY_EXPORT unsigned short getStensorSize(
      const unsigned short);
  /*!
   * \brief compute the derivative of the determinant with respect
   * to its argument.
   * \param[out] dJ: determinant derivative
   * \param[in]  s: argument
   */
  template <typename StensorResultType, typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorResultType>() &&
          implementsStensorConcept<StensorType>() &&
          tfel::typetraits::IsAssignableTo<
              typename ComputeUnaryResult<StensorNumType<StensorType>,
                                          Power<2>>::Result,
              StensorNumType<StensorResultType>>::cond,
      void>
  computeDeterminantDerivative(StensorResultType&, const StensorType&);
  /*!
   * \brief compute the derivative of the determinant with respect
   * to its argument.
   * \param[out] dJ: determinant derivative
   * \param[in]  s:  argument
   */
  template <typename StensorResultType, typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorResultType>() &&
          implementsStensorConcept<StensorType>() &&
          tfel::typetraits::IsAssignableTo<
              typename ComputeUnaryResult<StensorNumType<StensorType>,
                                          Power<2>>::Result,
              StensorNumType<StensorResultType>>::cond,
      void>
  computeDeviatorDeterminantDerivative(StensorResultType&, const StensorType&);

}  // end of namespace tfel::math

#include "TFEL/Math/Stensor/StensorConcept.ixx"

#endif /* LIB_TFEL_MATH_STENSORCONCEPT_HXX */
