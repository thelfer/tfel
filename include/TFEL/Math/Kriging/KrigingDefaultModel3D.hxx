/*!
 * \file  include/TFEL/Math/Kriging/KrigingDefaultModel3D.hxx
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

#ifndef LIB_TFEL_MATH_KRIGINGDEFAULTMODEL3D_HXX
#define LIB_TFEL_MATH_KRIGINGDEFAULTMODEL3D_HXX

#include <cmath>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Kriging/KrigingVariable.hxx"

namespace tfel::math {

  /*!
   * Partial specialisation in 3D
   * \param T  : numeric type
   * \param NM : nugget model
   */
  template <typename T, typename NM>
  struct KrigingDefaultModel<3u, T, NM> : public NM {
    static TFEL_MATH_INLINE T
    one(const typename KrigingVariable<3u, T>::type&) {
      return T(1);
    }

    static TFEL_MATH_INLINE T
    x(const typename KrigingVariable<3u, T>::type& v) {
      return v(0);
    }

    static TFEL_MATH_INLINE T
    y(const typename KrigingVariable<3u, T>::type& v) {
      return v(1);
    }

    static TFEL_MATH_INLINE T
    z(const typename KrigingVariable<3u, T>::type& v) {
      return v(2);
    }

    static TFEL_MATH_INLINE T
    covariance(const typename KrigingVariable<3u, T>::type& v) {
      return std::sqrt(v(0) * v(0) + v(1) * v(1) + v(2) * v(2));
    }  // end of covariance

    typedef T (*Drifts)(const typename KrigingVariable<3u, T>::type&);

    static const unsigned short nb = 4u; /* number of drifts */
    static const Drifts drifts[nb];
  };

  template <typename T, typename NM>
  const typename KrigingDefaultModel<3u, T, NM>::Drifts
      KrigingDefaultModel<3u,
                          T,
                          NM>::drifts[KrigingDefaultModel<3u, T, NM>::nb] = {
          KrigingDefaultModel<3u, T, NM>::one,
          KrigingDefaultModel<3u, T, NM>::x, KrigingDefaultModel<3u, T, NM>::y,
          KrigingDefaultModel<3u, T, NM>::z};

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_KRIGINGDEFAULTMODEL3D_HXX */
