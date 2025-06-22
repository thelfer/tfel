/*!
 * \file  include/TFEL/Math/Kriging/KrigingDefaultModel1D.hxx
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

#ifndef LIB_TFEL_MATH_KRIGINGDEFAULTMODEL1D_HXX
#define LIB_TFEL_MATH_KRIGINGDEFAULTMODEL1D_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/Kriging/KrigingVariable.hxx"

namespace tfel {

  namespace math {

    /*!
     * Partial specialisation in 1D
     * \param T  : numeric type
     * \param NM : nugget model
     */
    template <typename T, typename NM>
    struct KrigingDefaultModel<1u, T, NM> : public NM {
      static TFEL_MATH_INLINE T
      one(const typename KrigingVariable<1u, T>::type&) {
        return T(1);
      }

      static TFEL_MATH_INLINE T
      x(const typename KrigingVariable<1u, T>::type& v) {
        return v;
      }

      TFEL_MATH_INLINE T
      covariance(const typename KrigingVariable<1u, T>::type& v) const {
        using namespace std;
        return abs(v * v * v);
      }  // end of covariance

      typedef T (*Drifts)(const typename KrigingVariable<1u, T>::type&);

      static const unsigned short nb = 2u; /* number of drifts */
      static const Drifts drifts[2u];
    };

    template <typename T, typename NM>
    const typename KrigingDefaultModel<1u, T, NM>::Drifts
        KrigingDefaultModel<1u, T, NM>::drifts[2u] = {
            KrigingDefaultModel<1u, T, NM>::one,
            KrigingDefaultModel<1u, T, NM>::x};

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_KRIGINGDEFAULTMODEL1D_HXX */
