/*!
 * \file   SymmetricStensorProduct.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20 avril 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCT_HXX
#define LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCT_HXX

#include "TFEL/Math/stensor.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief compute the product \(a\,\dot\,b\,\dot\,a\) of two
     * symmetric tensor \(a\= and \(b\) which results in a symmetric
     * tensor.  This is equivalent but more efficient than
     * `syme(a*b*a)`.
     * \param[in] a:the first symmetric tensor
     * \param[in] b: the second symmetric * tensor
     */
    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_aba(const StensorType1& a, const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 1u) &&
             (StensorTraits<StensorType2>::dime == 1u)),
            stensor<1u, decltype(a[0] * b[0] * a[0])>>::type;
    /*!
     * \brief compute the product \(a\,\dot\,b\,\dot\,a\) of two
     * symmetric tensor \(a\= and \(b\) which results in a symmetric
     * tensor.  This is equivalent but more efficient than
     * `syme(a*b*a)`.
     * \param[in] a:the first symmetric tensor
     * \param[in] b: the second symmetric * tensor
     */
    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_aba(const StensorType1& a, const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 2u) &&
             (StensorTraits<StensorType2>::dime == 2u)),
            stensor<2u, decltype(a[0] * b[0] * a[0])>>::type;
    /*!
     * \brief compute the product \(a\,\dot\,b\,\dot\,a\) of two
     * symmetric tensor \(a\= and \(b\) which results in a symmetric
     * tensor.  This is equivalent but more efficient than
     * `syme(a*b*a)`.
     * \param[in] a:the first symmetric tensor
     * \param[in] b: the second symmetric * tensor
     */
    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_aba(const StensorType1& a, const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 3u) &&
             (StensorTraits<StensorType2>::dime == 3u)),
            stensor<3u, decltype(a[0] * b[0] * a[0])>>::type;

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Stensor/SymmetricStensorProduct.ixx"

#endif /* LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCT_HXX */
