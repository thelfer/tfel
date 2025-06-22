/*!
 * \file   SymmetricStensorProductDerivative.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21 avril 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCTDERIVATIVE_HXX
#define LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCTDERIVATIVE_HXX

#include "TFEL/Math/st2tost2.hxx"

namespace tfel::math {

  /*!
   * \brief compute the derivative of the product
   * \(a\,\dot\,b\,\dot\,a\) with respect to \(a\), where \(a\) and
   * \(b) are two symmetric tensor.
   * \param[in] a: the first symmetric tensor
   * \param[in] b: the second symmetric tensor
   */
  template <StensorConcept StensorType1, StensorConcept StensorType2>
  TFEL_HOST_DEVICE constexpr auto symmetric_product_derivative_daba_da(
      const StensorType1& a, const StensorType2& b) noexcept
      -> st2tost2<1u, decltype(a[0] * b[0])>  //
  requires((getSpaceDimension<StensorType1>() == 1u) &&
           (getSpaceDimension<StensorType2>() == 1u));

  /*!
   * \brief compute the derivative of the product
   * \(a\,\dot\,b\,\dot\,a\) with respect to \(a\), where \(a\) and
   * \(b) are two symmetric tensor.
   * \param[in] a: the first symmetric tensor
   * \param[in] b: the second symmetric tensor
   */
  template <StensorConcept StensorType1, StensorConcept StensorType2>
  TFEL_HOST_DEVICE constexpr auto symmetric_product_derivative_daba_da(
      const StensorType1& a, const StensorType2& b) noexcept
      -> st2tost2<2u, decltype(a[0] * b[0])>  //
  requires((getSpaceDimension<StensorType1>() == 2u) &&
           (getSpaceDimension<StensorType2>() == 2u));

  /*!
   * \brief compute the derivative of the product \(a\,\dot\,b\,\dot\,a\)
   * with respect to \(a\), where \(a\) and \(b) are two symmetric tensor.
   * \param[in] a: the first symmetric tensor
   * \param[in] b: the second symmetric tensor
   */
  template <StensorConcept StensorType1, StensorConcept StensorType2>
  TFEL_HOST_DEVICE constexpr auto symmetric_product_derivative_daba_da(
      const StensorType1& a, const StensorType2& b) noexcept
      -> st2tost2<3u, decltype(a[0] * b[0])>  //
  requires((getSpaceDimension<StensorType1>() == 3u) &&
           (getSpaceDimension<StensorType2>() == 3u));
  /*!
   * \brief compute the derivative of the product
   * \(a\,\dot\,b\,\dot\,a\) with respect to \(b\), where \(a\) and
   * \(b) are two symmetric tensor.
   * \param[in] a: the first symmetric tensor
   */
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto symmetric_product_derivative_daba_db(
      const StensorType& a) noexcept -> st2tost2<1u, decltype(a[0] * a[0])>  //
  requires(getSpaceDimension<StensorType>() == 1u);
  /*!
   * \brief compute the derivative of the product
   * \(a\,\dot\,b\,\dot\,a\) with respect to \(b\), where \(a\) and
   * \(b) are two symmetric tensor.
   * \param[in] a: the first symmetric tensor
   */
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto symmetric_product_derivative_daba_db(
      const StensorType& a) noexcept -> st2tost2<2u, decltype(a[0] * a[0])>  //
  requires(getSpaceDimension<StensorType>() == 2u);
  /*!
   * \brief compute the derivative of the product
   * \(a\,\dot\,b\,\dot\,a\) with respect to \(b\), where \(a\) and
   * \(b) are two symmetric tensor.
   * \param[in] a: the first symmetric tensor
   */
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto symmetric_product_derivative_daba_db(
      const StensorType& a) noexcept -> st2tost2<3u, decltype(a[0] * a[0])>  //
  requires(getSpaceDimension<StensorType>() == 3u);

}  // end of namespace tfel::math

#include "TFEL/Math/ST2toST2/SymmetricStensorProductDerivative.ixx"

#endif /* LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCTDERIVATIVE_HXX */
