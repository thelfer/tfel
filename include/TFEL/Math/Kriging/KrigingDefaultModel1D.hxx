/*! 
 * \file  KrigingDefaultModel1D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL1D_H_
#define _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL1D_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Kriging/KrigingVariable.hxx"
#include"TFEL/Math/Kriging/KrigingDefaultNuggetModel.hxx"

namespace tfel
{

  namespace math
  {

    template<typename T>
    struct KrigingDefaultModel<1u,T>
      : public KrigingDefaultNuggetModel<1u,T>
    {

      static TFEL_MATH_INLINE T
      one(const typename KrigingVariable<1u,T>::type&)
      {
	return T(1);
      }

      static TFEL_MATH_INLINE T
      x(const typename KrigingVariable<1u,T>::type& v)
      {
	return v;
      }

      TFEL_MATH_INLINE T
      covariance(const typename KrigingVariable<1u,T>::type& v) const
      {
	using namespace std;
	return abs(v*v*v);
      } // end of covariance

      typedef T (*Drifts)(const typename KrigingVariable<1u,T>::type&);

      static const unsigned short nb = 2u; /* number of drifts */
      static const Drifts drifts[2u];

    };

    template<typename T>
    const typename KrigingDefaultModel<1u,T>::Drifts KrigingDefaultModel<1u,T>::drifts[2u] = {KrigingDefaultModel<1u,T>::one,
											      KrigingDefaultModel<1u,T>::x};

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL1D_H */

