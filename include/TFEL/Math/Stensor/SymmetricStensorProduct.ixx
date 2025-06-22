/*!
 * \file   SymmetricStensorProduct.ixx
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

#ifndef LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCT_IXX
#define LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCT_IXX

namespace tfel {

  namespace math {

    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_aba(const StensorType1& a, const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 1u) &&
             (StensorTraits<StensorType2>::dime == 1u)),
            stensor<1u, decltype(a[0] * b[0] * a[0])>>::type {
      return {a[0] * a[0] * b[0], a[1] * a[1] * b[1], a[2] * a[2] * b[2]};
    }  // end of symmetric_product_aba

    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_aba(const StensorType1& a, const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 2u) &&
             (StensorTraits<StensorType2>::dime == 2u)),
            stensor<2u, decltype(a[0] * b[0] * a[0])>>::type {
      return {(2 * a[0] * a[3] * b[3] + a[3] * a[3] * b[1] +
               2 * a[0] * a[0] * b[0]) /
                  2,
              (2 * a[1] * a[3] * b[3] + 2 * a[1] * a[1] * b[1] +
               a[3] * a[3] * b[0]) /
                  2,
              a[2] * a[2] * b[2],
              ((a[3] * a[3] + 2 * a[0] * a[1]) * b[3] + 2 * a[1] * a[3] * b[1] +
               2 * a[0] * a[3] * b[0]) /
                  2};
    }  // end of symmetric_product_aba

    template <typename StensorType1, typename StensorType2>
    auto symmetric_product_aba(const StensorType1& a, const StensorType2& b) ->
        typename std::enable_if<
            ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
             (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
             (StensorTraits<StensorType1>::dime == 3u) &&
             (StensorTraits<StensorType2>::dime == 3u)),
            stensor<3u, decltype(a[0] * b[0] * a[0])>>::type {
      using NumType = decltype(a[0] * b[0] * a[0]);
      using real = tfel::typetraits::base_type<NumType>;
      constexpr const auto cste = Cste<real>::sqrt2;
      return {
          (cste * a[3] * a[4] * b[5] + 2 * a[0] * a[4] * b[4] +
           2 * a[0] * a[3] * b[3] + a[4] * a[4] * b[2] + a[3] * a[3] * b[1] +
           2 * a[0] * a[0] * b[0]) /
              2,
          // second line
          (2 * a[1] * a[5] * b[5] + cste * a[3] * a[5] * b[4] +
           2 * a[1] * a[3] * b[3] + a[5] * a[5] * b[2] +
           2 * a[1] * a[1] * b[1] + a[3] * a[3] * b[0]) /
              2,
          // third line
          (2 * a[2] * a[5] * b[5] + 2 * a[2] * a[4] * b[4] +
           cste * a[4] * a[5] * b[3] + 2 * a[2] * a[2] * b[2] +
           a[5] * a[5] * b[1] + a[4] * a[4] * b[0]) /
              2,
          // fourth line
          ((a[3] * a[5] + cste * a[1] * a[4]) * b[5] +
           (cste * a[0] * a[5] + a[3] * a[4]) * b[4] +
           (a[3] * a[3] + 2 * a[0] * a[1]) * b[3] + cste * a[4] * a[5] * b[2] +
           2 * a[1] * a[3] * b[1] + 2 * a[0] * a[3] * b[0]) /
              2,
          // fifth line
          ((a[4] * a[5] + cste * a[2] * a[3]) * b[5] +
           (a[4] * a[4] + 2 * a[0] * a[2]) * b[4] +
           (cste * a[0] * a[5] + a[3] * a[4]) * b[3] + 2 * a[2] * a[4] * b[2] +
           cste * a[3] * a[5] * b[1] + 2 * a[0] * a[4] * b[0]) /
              2,
          // sixtth line
          ((a[5] * a[5] + 2 * a[1] * a[2]) * b[5] +
           (a[4] * a[5] + cste * a[2] * a[3]) * b[4] +
           (a[3] * a[5] + cste * a[1] * a[4]) * b[3] + 2 * a[2] * a[5] * b[2] +
           2 * a[1] * a[5] * b[1] + cste * a[3] * a[4] * b[0]) /
              2};
    }  // end of symmetric_product_aba

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_SYMMETRICSTENSORPRODUCT_IXX */
