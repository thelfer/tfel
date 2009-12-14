/*! 
 * \file  KrigingDefaultModel3D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL3D_H_
#define _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL3D_H_ 

#include"Math/Kriging/KrigingDefaultNuggetModel.hxx"

namespace tfel
{

  namespace math
  {

    template<typename T>
    struct KrigingDefaultModel<3u,T>
      : public KrigingDefaultNuggetModel<3u,T>
    {
      static T
      one(const typename KrigingVariable<3u,T>::type&)
      {
	return T(1);
      }

      static T
      x(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(0);
      }

      static T
      y(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(1);
      }

      static T
      z(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(2);
      }

      static T
      xx(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(0)*v(0);
      }

      static T
      xy(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(0)*v(1);
      }

      static T
      xz(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(0)*v(2);
      }

      static T
      yy(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(1)*v(1);
      }

      static T
      yz(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(1)*v(2);
      }

      static T
      zz(const typename KrigingVariable<3u,T>::type& v)
      {
	return v(2)*v(2);
      }

      static T
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

