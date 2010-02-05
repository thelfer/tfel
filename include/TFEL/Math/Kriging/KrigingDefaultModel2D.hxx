/*! 
 * \file  KrigingDefaultModel2D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL2D_H_
#define _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL2D_H_ 

#include<cmath>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Kriging/KrigingVariable.hxx"
#include"TFEL/Math/Kriging/KrigingDefaultNuggetModel.hxx"

namespace tfel
{

  namespace math
  {

    template<typename T>
    struct KrigingDefaultModel<2u,T>
      : public KrigingDefaultNuggetModel<2u,T>
    {
      static TFEL_MATH_INLINE T
      one(const typename KrigingVariable<2u,T>::type&)
      {
	return T(1);
      }

      static TFEL_MATH_INLINE T
      x(const typename KrigingVariable<2u,T>::type& v)
      {
	return v(0);
      }

      static TFEL_MATH_INLINE  T
      y(const typename KrigingVariable<2u,T>::type& v)
      {
	return v(1);
      }

      static TFEL_MATH_INLINE T
      covariance(const typename KrigingVariable<2u,T>::type& v)
      {
	using namespace std;
	T h = sqrt(v(0)*v(0)+v(1)*v(1));
	return h*h*log(h);
      } // end of covariance

      typedef T (*Drifts)(const typename KrigingVariable<2u,T>::type&);

      static const unsigned short nb = 3u; /* number of drifts */
      static const Drifts drifts[nb];
    };

    template<typename T>
    const typename KrigingDefaultModel<2u,T>::Drifts
    KrigingDefaultModel<2u,T>::drifts[KrigingDefaultModel<2u,T>::nb] = {KrigingDefaultModel<2u,T>::one,
									KrigingDefaultModel<2u,T>::x,
									KrigingDefaultModel<2u,T>::y};

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL2D_H */

