/*!
 * \file   AnsysTangentOperator.ixx
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

#ifndef LIB_ANSYSTANGENTOPERATOR_IXX
#define LIB_ANSYSTANGENTOPERATOR_IXX

#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace ansys {

  template <typename real>
  void AnsysTangentOperator<real>::normalize(
      tfel::math::ST2toST2View<1u, real>&) {
  }  // end of AnsysTangentOperator<real>::normalize

  template <typename real>
  void AnsysTangentOperator<real>::normalize(
      tfel::math::ST2toST2View<2u, real>& Dt) {
    constexpr const auto cste = tfel::math::Cste<real>::sqrt2;
    Dt(0, 3) *= cste;
    Dt(1, 3) *= cste;
    Dt(2, 3) *= cste;
    Dt(3, 0) *= cste;
    Dt(3, 1) *= cste;
    Dt(3, 2) *= cste;
  }  // end of AnsysTangentOperator<real>::normalize

  template <typename real>
  void AnsysTangentOperator<real>::normalize(
      tfel::math::ST2toST2View<3u, real>& Dt) {
    constexpr const auto cste = tfel::math::Cste<real>::sqrt2;
    Dt(0, 3) *= cste;
    Dt(1, 3) *= cste;
    Dt(2, 3) *= cste;
    Dt(0, 4) *= cste;
    Dt(1, 4) *= cste;
    Dt(2, 4) *= cste;
    Dt(0, 5) *= cste;
    Dt(1, 5) *= cste;
    Dt(2, 5) *= cste;
    Dt(3, 0) *= cste;
    Dt(3, 1) *= cste;
    Dt(3, 2) *= cste;
    Dt(4, 0) *= cste;
    Dt(4, 1) *= cste;
    Dt(4, 2) *= cste;
    Dt(5, 0) *= cste;
    Dt(5, 1) *= cste;
    Dt(5, 2) *= cste;
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
  }  // end of struct AnsysTangentOperator

  template <typename real>
  void AnsysTangentOperator<real>::transpose(
      tfel::math::ST2toST2View<1u, real>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
    std::swap(Dt(0, 2), Dt(2, 0));
    std::swap(Dt(1, 2), Dt(2, 1));
  }

  template <typename real>
  void AnsysTangentOperator<real>::transpose(
      tfel::math::ST2toST2View<2u, real>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
    std::swap(Dt(0, 2), Dt(2, 0));
    std::swap(Dt(0, 3), Dt(3, 0));
    std::swap(Dt(1, 2), Dt(2, 1));
    std::swap(Dt(1, 3), Dt(3, 1));
    std::swap(Dt(2, 3), Dt(3, 2));
  }

  template <typename real>
  void AnsysTangentOperator<real>::transpose(
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
  tfel::math::st2tost2<2u, real> AnsysTangentOperator<real>::convert2D(
      const real* const D) {
    tfel::math::st2tost2<2u, real> r;
    TFEL_CONSTEXPR const real isqrt2 = tfel::math::Cste<real>::isqrt2;
    using size_type = typename tfel::math::st2tost2<2u, real>::size_type;
    for (size_type i = 0; i != 4; ++i) {
      for (size_type j = 0; j != 4; ++j) {
        r(i, j) = D[j * 4 + i];
      }
    }
    r(0, 3) *= isqrt2;
    r(1, 3) *= isqrt2;
    r(2, 3) *= isqrt2;
    r(3, 0) *= isqrt2;
    r(3, 1) *= isqrt2;
    r(3, 2) *= isqrt2;
    return r;
  }

  template <typename real>
  tfel::math::st2tost2<3u, real> AnsysTangentOperator<real>::convert3D(
      const real* const D) {
    tfel::math::st2tost2<3u, real> r;
    TFEL_CONSTEXPR const real isqrt2 = tfel::math::Cste<real>::isqrt2;
    using size_type = typename tfel::math::st2tost2<3u, real>::size_type;
    for (size_type i = 0; i != 6; ++i) {
      for (size_type j = 0; j != 6; ++j) {
        r(i, j) = D[j * 6 + i];
      }
    }
    r(0, 3) *= isqrt2;
    r(1, 3) *= isqrt2;
    r(2, 3) *= isqrt2;
    r(0, 4) *= isqrt2;
    r(1, 4) *= isqrt2;
    r(2, 4) *= isqrt2;
    r(0, 5) *= isqrt2;
    r(1, 5) *= isqrt2;
    r(2, 5) *= isqrt2;
    r(3, 0) *= isqrt2;
    r(3, 1) *= isqrt2;
    r(3, 2) *= isqrt2;
    r(4, 0) *= isqrt2;
    r(4, 1) *= isqrt2;
    r(4, 2) *= isqrt2;
    r(5, 0) *= isqrt2;
    r(5, 1) *= isqrt2;
    r(5, 2) *= isqrt2;
    return r;
  }

}  // end of namespace ansys

#endif /* LIB_ANSYSTANGENTOPERATOR_IXX */
