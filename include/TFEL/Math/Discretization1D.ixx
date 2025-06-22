/*!
 * \file  include/TFEL/Math/Discretization1D.ixx
 * \brief
 * \author Thomas Helfer
 * \brief 05 déc. 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_DISCRETIZATION1DIXX
#define LIB_TFEL_MATH_DISCRETIZATION1DIXX

#include <cmath>
#include <limits>

#include "TFEL/Math/General/Abs.hxx"

namespace tfel {

  namespace math {

    template <typename T>
    void geometricDiscretization(T& v,
                                 const typename T::value_type xb,
                                 const typename T::value_type xe,
                                 const typename T::value_type db,
                                 const typename T::value_type de,
                                 const typename T::size_type n) {
      using namespace std;
      typedef typename T::value_type real;
      typename T::iterator p;
      const real l = xe - xb;
      if (std::abs(l) < 100 * numeric_limits<real>::min()) {
        throw(GeometricDiscretizationInvalidLength());
      }
      if ((std::abs(db) < 100 * numeric_limits<real>::min()) ||
          (std::abs(de) < 100 * numeric_limits<real>::min())) {
        throw(GeometricDiscretizationInvalidDensity());
      }
      if (std::abs(l) < 100 * numeric_limits<real>::min()) {
        throw(GeometricDiscretizationInvalidLength());
      }
      if (n <= 0) {
        throw(GeometricDiscretizationInvalidNumberOfElements());
      }
      const real rdb = db / l;
      const real rde = de / l;
      const real xaux = 0.5 * (rdb - rde) * (rdb - rde);
      real r;
      real f;
      real rf;
      real re = real(1);
      real s = real(0);
      if (rde < rdb) {
        r = 1. + xaux - std::sqrt(xaux * (2 + xaux));
      } else {
        r = 1 + xaux + std::sqrt(xaux * (2 + xaux));
      }
      if (std::abs(r - 1) > 1.e-5) {
        rf = (1. - r) / (1. - pow(r, static_cast<real>(n)));
      } else {
        rf = 1. / (static_cast<real>(n));
      }
      f = rf * l;
      v.resize(n + 1);
      p = v.begin();
      (*p++) = xb;
      for (; p != v.end(); ++p, re *= r) {
        s += f * re;
        *p = xb + s;
      }
      v.back() = xe;  // not mandatory (xe is the value that shall be
                      // found) but this is more precise
    }                 // end of geometricDiscretization

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_DISCRETIZATION1DIXX */
