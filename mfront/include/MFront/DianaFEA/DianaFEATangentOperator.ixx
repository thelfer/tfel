/*!
 * \file   DianaFEATangentOperator.ixx
 * \brief
 * \author Thomas Helfer
 * \date   04 avril 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_DIANAFEATANGENTOPERATOR_IXX
#define LIB_DIANAFEATANGENTOPERATOR_IXX

#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace dianafea {

  template <typename real>
  void DianaFEATangentOperator<real>::normalize(
      tfel::math::ST2toST2View<1u, real>&) {
  }  // end of DianaFEATangentOperator<real>::normalize

  template <typename real>
  void DianaFEATangentOperator<real>::normalize(
      tfel::math::ST2toST2View<2u, real>& Dt) {
    constexpr auto icste = tfel::math::Cste<real>::isqrt2;
    Dt(0, 3) *= icste;
    Dt(1, 3) *= icste;
    Dt(2, 3) *= icste;
    Dt(3, 0) *= icste;
    Dt(3, 1) *= icste;
    Dt(3, 2) *= icste;
    Dt(3, 3) /= real(2);
  }  // end of DianaFEATangentOperator<real>::normalize

  template <typename real>
  void DianaFEATangentOperator<real>::normalize(
      tfel::math::ST2toST2View<3u, real>& Dt) {
    constexpr const auto icste = tfel::math::Cste<real>::isqrt2;
    constexpr const auto one_half = real(1) / real(2);
    Dt(0, 3) *= icste;
    Dt(1, 3) *= icste;
    Dt(2, 3) *= icste;
    Dt(0, 4) *= icste;
    Dt(1, 4) *= icste;
    Dt(2, 4) *= icste;
    Dt(0, 5) *= icste;
    Dt(1, 5) *= icste;
    Dt(2, 5) *= icste;
    Dt(3, 0) *= icste;
    Dt(3, 1) *= icste;
    Dt(3, 2) *= icste;
    Dt(4, 0) *= icste;
    Dt(4, 1) *= icste;
    Dt(4, 2) *= icste;
    Dt(5, 0) *= icste;
    Dt(5, 1) *= icste;
    Dt(5, 2) *= icste;
    Dt(3, 3) *= one_half;
    Dt(3, 4) *= one_half;
    Dt(3, 5) *= one_half;
    Dt(4, 3) *= one_half;
    Dt(4, 4) *= one_half;
    Dt(4, 5) *= one_half;
    Dt(5, 3) *= one_half;
    Dt(5, 4) *= one_half;
    Dt(5, 5) *= one_half;
    // now changing to Voigt conventions: 23 <-> 13
    // changing last columns
    std::swap(Dt(0, 4), Dt(0, 5));
    std::swap(Dt(1, 4), Dt(1, 5));
    std::swap(Dt(2, 4), Dt(2, 5));
    std::swap(Dt(3, 4), Dt(3, 5));
    std::swap(Dt(4, 4), Dt(4, 5));
    std::swap(Dt(5, 4), Dt(5, 5));
    // changing last rows
    std::swap(Dt(4, 0), Dt(5, 0));
    std::swap(Dt(4, 1), Dt(5, 1));
    std::swap(Dt(4, 2), Dt(5, 2));
    std::swap(Dt(4, 3), Dt(5, 3));
    std::swap(Dt(4, 4), Dt(5, 4));
    std::swap(Dt(4, 5), Dt(5, 5));
  }  // end of struct DianaFEATangentOperator

  template <typename real>
  void DianaFEATangentOperator<real>::transpose(
      tfel::math::ST2toST2View<1u, real>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
    std::swap(Dt(0, 2), Dt(2, 0));
    std::swap(Dt(1, 2), Dt(2, 1));
  }

  template <typename real>
  void DianaFEATangentOperator<real>::transpose(
      tfel::math::ST2toST2View<2u, real>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
    std::swap(Dt(0, 2), Dt(2, 0));
    std::swap(Dt(0, 3), Dt(3, 0));
    std::swap(Dt(1, 2), Dt(2, 1));
    std::swap(Dt(1, 3), Dt(3, 1));
    std::swap(Dt(2, 3), Dt(3, 2));
  }

  template <typename real>
  void DianaFEATangentOperator<real>::transpose(
      tfel::math::ST2toST2View<3u, real>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
    std::swap(Dt(0, 2), Dt(2, 0));
    std::swap(Dt(0, 3), Dt(3, 0));
    std::swap(Dt(0, 4), Dt(4, 0));
    std::swap(Dt(0, 5), Dt(5, 0));
    std::swap(Dt(1, 2), Dt(2, 1));
    std::swap(Dt(1, 3), Dt(3, 1));
    std::swap(Dt(1, 4), Dt(4, 1));
    std::swap(Dt(1, 5), Dt(5, 1));
    std::swap(Dt(2, 3), Dt(3, 2));
    std::swap(Dt(2, 4), Dt(4, 2));
    std::swap(Dt(2, 5), Dt(5, 2));
    std::swap(Dt(3, 4), Dt(4, 3));
    std::swap(Dt(3, 5), Dt(5, 3));
    std::swap(Dt(4, 5), Dt(5, 4));
  }

  template <typename real>
  tfel::math::st2tost2<2u, real> DianaFEATangentOperator<real>::convert2D(
      const real* const D) {
    tfel::math::st2tost2<2u, real> r;
    constexpr const real cste = tfel::math::Cste<real>::sqrt2;
    using size_type = typename tfel::math::st2tost2<2u, real>::size_type;
    for (size_type i = 0; i != 4; ++i) {
      for (size_type j = 0; j != 4; ++j) {
        r(i, j) = D[j * 4 + i];
      }
    }
    r(0, 3) *= cste;
    r(1, 3) *= cste;
    r(2, 3) *= cste;
    r(3, 0) *= cste;
    r(3, 1) *= cste;
    r(3, 2) *= cste;
    r(3, 3) *= 2;
    return r;
  }

  template <typename real>
  tfel::math::st2tost2<3u, real> DianaFEATangentOperator<real>::convert3D(
      const real* const D) {
    tfel::math::st2tost2<3u, real> r;
    constexpr const real cste = tfel::math::Cste<real>::sqrt2;
    using size_type = typename tfel::math::st2tost2<3u, real>::size_type;
    for (size_type i = 0; i != 6; ++i) {
      for (size_type j = 0; j != 6; ++j) {
        r(i, j) = D[j * 6 + i];
      }
    }
    r(0, 3) *= cste;
    r(1, 3) *= cste;
    r(2, 3) *= cste;
    r(0, 4) *= cste;
    r(1, 4) *= cste;
    r(2, 4) *= cste;
    r(0, 5) *= cste;
    r(1, 5) *= cste;
    r(2, 5) *= cste;
    r(3, 0) *= cste;
    r(3, 1) *= cste;
    r(3, 2) *= cste;
    r(4, 0) *= cste;
    r(4, 1) *= cste;
    r(4, 2) *= cste;
    r(5, 0) *= cste;
    r(5, 1) *= cste;
    r(5, 2) *= cste;
    r(3, 3) *= 2;
    r(3, 4) *= 2;
    r(3, 5) *= 2;
    r(4, 3) *= 2;
    r(4, 4) *= 2;
    r(4, 5) *= 2;
    r(5, 3) *= 2;
    r(5, 4) *= 2;
    r(5, 5) *= 2;
    // now changing to Voigt conventions: 23 <-> 13
    // changing last columns
    std::swap(r(0, 4), r(0, 5));
    std::swap(r(1, 4), r(1, 5));
    std::swap(r(2, 4), r(2, 5));
    std::swap(r(3, 4), r(3, 5));
    std::swap(r(4, 4), r(4, 5));
    std::swap(r(5, 4), r(5, 5));
    // changing last rows
    std::swap(r(4, 0), r(5, 0));
    std::swap(r(4, 1), r(5, 1));
    std::swap(r(4, 2), r(5, 2));
    std::swap(r(4, 3), r(5, 3));
    std::swap(r(4, 4), r(5, 4));
    std::swap(r(4, 5), r(5, 5));
    return r;
  }

}  // end of namespace dianafea

#endif /* LIB_DIANAFEATANGENTOPERATOR_IXX */
