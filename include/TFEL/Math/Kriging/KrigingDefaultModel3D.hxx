/*! 
 * \file  KrigingDefaultModel3D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL3D_H_
#define _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL3D_H_ 

#include<cmath>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Kriging/KrigingVariable.hxx"
#include"TFEL/Math/Kriging/KrigingDefaultNuggetModel.hxx"

namespace tfel
{

  namespace math
  {

    template<typename T>
    struct KrigingDefaultModel<3u,T>
      : public KrigingDefaultNuggetModel<3u,T>
    {
      static TFEL_MATH_INLINE T
      one(const typename KrigingVariable<3u,T>::type&)
      {
	return T(1);
      }

      static TFEL_MATH_INLINE T
      x(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(0);
      }

      static TFEL_MATH_INLINE T
      y(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(1);
      }

      static TFEL_MATH_INLINE T
      z(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(2);
      }

      static TFEL_MATH_INLINE T
      xx(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(0)*v(0);
      }

      static TFEL_MATH_INLINE T
      xy(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(0)*v(1);
      }

      static TFEL_MATH_INLINE T
      xz(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(0)*v(2);
      }

      static TFEL_MATH_INLINE T
      yy(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(1)*v(1);
      }

      static TFEL_MATH_INLINE T
      yz(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(1)*v(2);
      }

      static TFEL_MATH_INLINE T
      zz(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(2)*v(2);
      }

      static TFEL_MATH_INLINE T
      covariance(const typename KrigingVariable<3u,T>::type& v)
      {
	using namespace std;
	return sqrt(v(0)*v(0)+v(1)*v(1)+v(2)*v(2));
      } // end of covariance

      typedef T (*Drifts)(const typename KrigingVariable<3u,T>::type&);

      static const unsigned short nb = 10u; /* number of drifts */
      static const Drifts drifts[nb];
    };

    template<typename T>
    const typename KrigingDefaultModel<3u,T>::Drifts
    KrigingDefaultModel<3u,T>::drifts[KrigingDefaultModel<3u,T>::nb] = {KrigingDefaultModel<3u,T>::one,
									KrigingDefaultModel<3u,T>::x,
									KrigingDefaultModel<3u,T>::y,
									KrigingDefaultModel<3u,T>::y,
									KrigingDefaultModel<3u,T>::xx,
									KrigingDefaultModel<3u,T>::xy,
									KrigingDefaultModel<3u,T>::xz,
									KrigingDefaultModel<3u,T>::yy,
									KrigingDefaultModel<3u,T>::yz,
									KrigingDefaultModel<3u,T>::zz};
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL3D<_H */

