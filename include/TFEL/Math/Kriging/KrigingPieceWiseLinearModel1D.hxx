/*! 
 * \file  include/TFEL/Math/Kriging/KrigingPieceWiseLinearModel1D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_KRIGINGPIECEWISELINEARMODEL1D_H_
#define _LIB_TFEL_MATH_KRIGINGPIECEWISELINEARMODEL1D_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Kriging/KrigingVariable.hxx"
#include"TFEL/Math/Kriging/KrigingDefaultNuggetModel.hxx"

namespace tfel
{

  namespace math
  {

    template<typename T>
    struct KrigingPieceWiseLinearModel1D
      : public KrigingDefaultNuggetModel<1u,T>
    {

      static TFEL_MATH_INLINE T
      one(const typename KrigingVariable<1u,T>::type&)
      {
	return T(1);
      }

      TFEL_MATH_INLINE T
      covariance(const typename KrigingVariable<1u,T>::type& v) const
      {
	using namespace std;
	return abs(v);
      } // end of covariance

      typedef T (*Drifts)(const typename KrigingVariable<1u,T>::type&);

      static const unsigned short nb = 1u; /* number of drifts */
      static const Drifts drifts[1u];

    };

    template<typename T>
    const typename KrigingPieceWiseLinearModel1D<T>::Drifts
    KrigingPieceWiseLinearModel1D<T>::drifts[1u] = {KrigingPieceWiseLinearModel1D<T>::one};

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_KRIGINGPIECEWISELINEARMODEL1D_H */

