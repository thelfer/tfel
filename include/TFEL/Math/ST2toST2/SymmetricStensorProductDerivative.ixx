/*!
 * \file   SymmetricStensorProductDerivative.ixx
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

#ifndef LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCTDERIVATIVE_IXX
#define LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCTDERIVATIVE_IXX

namespace tfel {

  namespace math {

    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_derivative_daba_da(const StensorType1& a,
                                              const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 1u) &&
             (StensorTraits<StensorType2>::dime == 1u)),
            st2tost2<1u, decltype(a[0] * b[0])>>::type {
      using NumType = decltype(a[0] * b[0]);
      using real = tfel::typetraits::base_type<NumType>;
      constexpr const auto zero = real(0);
      return {2 * a[0] * b[0], zero, zero, zero,
              2 * a[1] * b[1], zero, zero, zero,
              2 * a[2] * b[2]};
    }  // end of symmetric_product_aba

    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_derivative_daba_da(const StensorType1& a,
                                              const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 2u) &&
             (StensorTraits<StensorType2>::dime == 2u)),
            st2tost2<2u, decltype(a[0] * b[0])>>::type {
      using NumType = decltype(a[0] * b[0]);
      using real = tfel::typetraits::base_type<NumType>;
      constexpr const auto zero = real(0);
      return {a[3] * b[3] + 2 * a[0] * b[0],
              zero,
              zero,
              a[0] * b[3] + a[3] * b[1],
              zero,
              a[3] * b[3] + 2 * a[1] * b[1],
              zero,
              a[1] * b[3] + a[3] * b[0],
              zero,
              zero,
              2 * a[2] * b[2],
              zero,
              a[1] * b[3] + a[3] * b[0],
              a[0] * b[3] + a[3] * b[1],
              zero,
              a[3] * b[3] + a[1] * b[1] + a[0] * b[0]};
    }  // end of symmetric_product_aba

    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_derivative_daba_da(const StensorType1& a,
                                              const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 3u) &&
             (StensorTraits<StensorType2>::dime == 3u)),
            st2tost2<3u, decltype(a[0] * b[0])>>::type {
      using NumType = decltype(a[0] * b[0]);
      using real = tfel::typetraits::base_type<NumType>;
      constexpr const auto zero = real(0);
      constexpr const auto cste = Cste<real>::sqrt2;
      return {
          a[4] * b[4] + a[3] * b[3] + 2 * a[0] * b[0], zero, zero,
          (cste * a[4] * b[5] + 2 * a[0] * b[3] + 2 * a[3] * b[1]) / 2,
          (cste * a[3] * b[5] + 2 * a[0] * b[4] + 2 * a[4] * b[2]) / 2, zero,
          // second line
          zero, a[5] * b[5] + a[3] * b[3] + 2 * a[1] * b[1], zero,
          (cste * a[5] * b[4] + 2 * a[1] * b[3] + 2 * a[3] * b[0]) / 2, zero,
          (2 * a[1] * b[5] + cste * a[3] * b[4] + 2 * a[5] * b[2]) / 2,
          // third line
          zero, zero, a[5] * b[5] + a[4] * b[4] + 2 * a[2] * b[2], zero,
          (2 * a[2] * b[4] + cste * a[5] * b[3] + 2 * a[4] * b[0]) / 2,
          (2 * a[2] * b[5] + cste * a[4] * b[3] + 2 * a[5] * b[1]) / 2,
          // fourth line
          (cste * a[5] * b[4] + 2 * a[1] * b[3] + 2 * a[3] * b[0]) / 2,
          (cste * a[4] * b[5] + 2 * a[0] * b[3] + 2 * a[3] * b[1]) / 2, zero,
          (a[5] * b[5] + a[4] * b[4] + 2 * a[3] * b[3] + 2 * a[1] * b[1] +
           2 * a[0] * b[0]) /
              2,
          (cste * a[1] * b[5] + a[3] * b[4] + cste * a[5] * b[2]) / 2,
          (a[3] * b[5] + cste * a[0] * b[4] + cste * a[4] * b[2]) / 2,
          // fifth line
          (2 * a[2] * b[4] + cste * a[5] * b[3] + 2 * a[4] * b[0]) / 2, zero,
          (cste * a[3] * b[5] + 2 * a[0] * b[4] + 2 * a[4] * b[2]) / 2,
          (cste * a[2] * b[5] + a[4] * b[3] + cste * a[5] * b[1]) / 2,
          (a[5] * b[5] + 2 * a[4] * b[4] + a[3] * b[3] + 2 * a[2] * b[2] +
           2 * a[0] * b[0]) /
              2,
          (a[4] * b[5] + cste * a[0] * b[3] + cste * a[3] * b[1]) / 2,
          // sixtth line
          zero, (2 * a[2] * b[5] + cste * a[4] * b[3] + 2 * a[5] * b[1]) / 2,
          (2 * a[1] * b[5] + cste * a[3] * b[4] + 2 * a[5] * b[2]) / 2,
          (cste * a[2] * b[4] + a[5] * b[3] + cste * a[4] * b[0]) / 2,
          (a[5] * b[4] + cste * a[1] * b[3] + cste * a[3] * b[0]) / 2,
          (2 * a[5] * b[5] + a[4] * b[4] + a[3] * b[3] + 2 * a[2] * b[2] +
           2 * a[1] * b[1]) /
              2};
    }  // end of symmetric_product_aba

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCTDERIVATIVE_IXX */
