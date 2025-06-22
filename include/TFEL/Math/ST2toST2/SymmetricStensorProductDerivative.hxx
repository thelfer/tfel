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

namespace tfel {

  namespace math {

    /*!
     * \brief compute the derivative of the product
     * \(a\,\dot\,b\,\dot\,a\) with respect to \(a\), where \(a\) and
     * \(b) are two symmetric tensor.
     * \param[in] a: the first symmetric tensor
     * \param[in] b: the second symmetric tensor
     */
    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_derivative_daba_da(const StensorType1& a,
                                              const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 1u) &&
             (StensorTraits<StensorType2>::dime == 1u)),
            st2tost2<1u, decltype(a[0] * b[0])>>::type;
    /*!
     * \brief compute the derivative of the product
     * \(a\,\dot\,b\,\dot\,a\) with respect to \(a\), where \(a\) and
     * \(b) are two symmetric tensor.
     * \param[in] a: the first symmetric tensor
     * \param[in] b: the second symmetric tensor
     */
    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_derivative_daba_da(const StensorType1& a,
                                              const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 2u) &&
             (StensorTraits<StensorType2>::dime == 2u)),
            st2tost2<2u, decltype(a[0] * b[0])>>::type;
    /*!
     * \brief compute the derivative of the product \(a\,\dot\,b\,\dot\,a\) with
     * respect to \(a\), where \(a\) and \(b) are two symmetric tensor.
     * \param[in] a: the first symmetric tensor
     * \param[in] b: the second symmetric tensor
     */
    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_derivative_daba_da(const StensorType1& a,
                                              const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 3u) &&
             (StensorTraits<StensorType2>::dime == 3u)),
            st2tost2<3u, decltype(a[0] * b[0])>>::type;

    /*!
     * \brief compute the derivative of the product
     * \(a\,\dot\,b\,\dot\,a\) with respect to \(b\), where \(a\) and
     * \(b) are two symmetric tensor.
     * \param[in] a: the first symmetric tensor
     */
    template <typename StensorType>
    auto symmetric_product_derivative_daba_db(const StensorType& a) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
             (StensorTraits<StensorType>::dime == 1u)),
            st2tost2<1u, decltype(a[0] * a[0])>>::type;
    /*!
     * \brief compute the derivative of the product
     * \(a\,\dot\,b\,\dot\,a\) with respect to \(b\), where \(a\) and
     * \(b) are two symmetric tensor.
     * \param[in] a: the first symmetric tensor
     */
    template <typename StensorType>
    auto symmetric_product_derivative_daba_db(const StensorType& a) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
             (StensorTraits<StensorType>::dime == 2u)),
            st2tost2<2u, decltype(a[0] * a[0])>>::type;
    /*!
     * \brief compute the derivative of the product
     * \(a\,\dot\,b\,\dot\,a\) with respect to \(b\), where \(a\) and
     * \(b) are two symmetric tensor.
     * \param[in] a: the first symmetric tensor
     */
    template <typename StensorType>
    auto symmetric_product_derivative_daba_db(const StensorType& a) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
             (StensorTraits<StensorType>::dime == 3u)),
            st2tost2<3u, decltype(a[0] * a[0])>>::type;

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/ST2toST2/SymmetricStensorProductDerivative.ixx"

#endif /* LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCTDERIVATIVE_HXX */
