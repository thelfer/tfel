/*!
 * \file   include/TFEL/Math/Quantity/qt.ixx
 * \brief  This file implements the methods defined in the qt class.
 * \author Thomas Helfer
 * \date   09 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QT_IXX
#define LIB_TFEL_MATH_QT_IXX

#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/UnaryResultType.hxx"

namespace tfel::math::stdfunctions {

  template <int N, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::
      qt<typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type, float>
      power(const tfel::math::qt<Unit, float> x) {
    typedef typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type, float>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

  template <int N, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::
      qt<typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type, double>
      power(const tfel::math::qt<Unit, double> x) {
    typedef typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type, double>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

  template <int N, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::qt<
      typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type,
      long double>
  power(const tfel::math::qt<Unit, long double> x) {
    typedef typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type,
        long double>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

  template <int N, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::qt<
      typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type,
      Complex<float>>
  power(const tfel::math::qt<Unit, Complex<float>> x) {
    typedef typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type,
        Complex<float>>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

  template <int N, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::qt<
      typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type,
      Complex<double>>
  power(const tfel::math::qt<Unit, Complex<double>> x) {
    typedef typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type,
        Complex<double>>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

  template <int N, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::qt<
      typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type,
      Complex<long double>>
  power(const tfel::math::qt<Unit, Complex<long double>> x) {
    typedef typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, 1u, Unit>::type,
        Complex<long double>>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

  template <int N, unsigned int D, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::
      qt<typename tfel::math::internals::PowerUnit<N, D, Unit>::type, float>
      power(const tfel::math::qt<Unit, float> x) {
    typedef typename tfel::math::PowerImplSelector<N, D>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, D, Unit>::type, float>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

  template <int N, unsigned int D, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::
      qt<typename tfel::math::internals::PowerUnit<N, D, Unit>::type, double>
      power(const tfel::math::qt<Unit, double> x) {
    typedef typename tfel::math::PowerImplSelector<N, D>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, D, Unit>::type, double>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

  template <int N, unsigned int D, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::qt<
      typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
      long double>
  power(const tfel::math::qt<Unit, long double> x) {
    typedef typename tfel::math::PowerImplSelector<N, D>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
        long double>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

  template <int N, unsigned int D, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::qt<
      typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
      tfel::math::Complex<float>>
  power(const tfel::math::qt<Unit, tfel::math::Complex<float>> x) {
    typedef typename tfel::math::PowerImplSelector<N, D>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
        tfel::math::Complex<float>>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

  template <int N, unsigned int D, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::qt<
      typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
      tfel::math::Complex<double>>
  power(const tfel::math::qt<Unit, tfel::math::Complex<double>> x) {
    typedef typename tfel::math::PowerImplSelector<N, D>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
        tfel::math::Complex<double>>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

  template <int N, unsigned int D, UnitConcept Unit>
  TFEL_MATH_INLINE tfel::math::qt<
      typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
      tfel::math::Complex<long double>>
  power(const tfel::math::qt<Unit, tfel::math::Complex<long double>> x) {
    typedef typename tfel::math::PowerImplSelector<N, D>::type Implementation;
    typedef tfel::math::qt<
        typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
        tfel::math::Complex<long double>>
        Res;
    return Res(Implementation::exe(x.getValue()));
  }

}  // end of namespace tfel::math::stdfunctions

namespace tfel::math {

  template <int N, unsigned int D>
  struct Power;

  template <UnitConcept Unit, int N, unsigned int D>
  class TFEL_VISIBILITY_LOCAL UnaryResultType<qt<Unit, float>, Power<N, D>> {
   public:
    typedef qt<typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
               float>
        type;
  };

  template <UnitConcept Unit, int N, unsigned int D>
  class TFEL_VISIBILITY_LOCAL UnaryResultType<qt<Unit, double>, Power<N, D>> {
   public:
    typedef qt<typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
               double>
        type;
  };

  template <UnitConcept Unit, int N, unsigned int D>
  class TFEL_VISIBILITY_LOCAL
      UnaryResultType<qt<Unit, long double>, Power<N, D>> {
   public:
    typedef qt<typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
               long double>
        type;
  };

  template <UnitConcept Unit, int N, unsigned int D>
  class TFEL_VISIBILITY_LOCAL
      UnaryResultType<qt<Unit, Complex<float>>, Power<N, D>> {
   public:
    typedef qt<typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
               Complex<float>>
        type;
  };

  template <UnitConcept Unit, int N, unsigned int D>
  class TFEL_VISIBILITY_LOCAL
      UnaryResultType<qt<Unit, Complex<double>>, Power<N, D>> {
   public:
    typedef qt<typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
               Complex<double>>
        type;
  };

  template <UnitConcept Unit, int N, unsigned int D>
  class TFEL_VISIBILITY_LOCAL
      UnaryResultType<qt<Unit, Complex<long double>>, Power<N, D>> {
   public:
    typedef qt<typename tfel::math::internals::PowerUnit<N, D, Unit>::type,
               Complex<long double>>
        type;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_QT_IXX */
