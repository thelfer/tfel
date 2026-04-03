/*!
 * \file  include/TFEL/Math/Kriging/KrigingDefaultModel2D.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_KRIGINGDEFAULTMODEL2D_HXX
#define LIB_TFEL_MATH_KRIGINGDEFAULTMODEL2D_HXX

#include <cmath>
#include <limits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Kriging/KrigingVariable.hxx"

namespace tfel::math {

  /*!
   * Partial specialisation in 2D
   * \param T  : numeric type
   * \param NM : nugget model
   */
  template <typename T, typename NM>
  struct KrigingDefaultModel<2u, T, NM> : public NM {
    static TFEL_MATH_INLINE T
    one(const typename KrigingVariable<2u, T>::type&) {
      return T(1);
    }

    static TFEL_MATH_INLINE T
    x(const typename KrigingVariable<2u, T>::type& v) {
      return v(0);
    }

    static TFEL_MATH_INLINE T
    y(const typename KrigingVariable<2u, T>::type& v) {
      return v(1);
    }

    static TFEL_MATH_INLINE T
    covariance(const typename KrigingVariable<2u, T>::type& v) {
      using namespace std;
      T h2 = v(0) * v(0) + v(1) * v(1);
      if (h2 < 10 * numeric_limits<T>::epsilon()) {
        return T(0);
      }
      return 0.5 * h2 * log(h2);
    }  // end of covariance

    typedef T (*Drifts)(const typename KrigingVariable<2u, T>::type&);

    static const unsigned short nb = 3u; /* number of drifts */
    static const Drifts drifts[3u];
  };

  template <typename T, typename NM>
  const typename KrigingDefaultModel<2u, T, NM>::Drifts
      KrigingDefaultModel<2u, T, NM>::drifts[3u] = {
          KrigingDefaultModel<2u, T, NM>::one,
          KrigingDefaultModel<2u, T, NM>::x, KrigingDefaultModel<2u, T, NM>::y};

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_KRIGINGDEFAULTMODEL2D_HXX */
