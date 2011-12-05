/*! 
 * \file  Discretization1D.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 05 déc. 2011
 */

#ifndef _LIB_TFEL_MATH_DISCRETIZATION1D_IXX_
#define _LIB_TFEL_MATH_DISCRETIZATION1D_IXX_ 

#include<cmath>
#include<limits>

namespace tfel
{

  namespace math
  {
    
    template<typename T>
    void
    geometricProgression(T& v,
			 const typename T::value_type xb,
			 const typename T::value_type xe,
			 const typename T::value_type db,
			 const typename T::value_type de,
			 const typename T::size_type  n)
    {
      using namespace std;
      typedef typename T::value_type real;
      typename T::iterator p;
      const real l    = xe - xb;
      if(abs(l)<100*numeric_limits<real>::min()){
	throw(GeometricDiscretizationInvalidLength());
      }
      if((abs(db)<100*numeric_limits<real>::min())||
	 (abs(de)<100*numeric_limits<real>::min())){
	throw(GeometricDiscretizationInvalidDensity());
      }
      if(abs(l)<100*numeric_limits<real>::min()){
	throw(GeometricDiscretizationInvalidLength());
      }
      if(n<=0){
	throw(GeometricDiscretizationInvalidNumberOfElements());
      }
      const real rdb  = db/l;
      const real rde  = de/l;
      const real xaux = 0.5*(rdb-rde)*(rdb-rde);
      real r;
      real f;
      real rf;
      real re = real(1);
      real s  = real(0);
      if(rde<rdb){
	r = 1. + xaux-sqrt(xaux*(2+xaux));
      } else {
	r = 1 + xaux+sqrt(xaux*(2+xaux));
      }
      if(abs(r-1)>1.e-5){
	rf=(1.-r)/(1.-pow(r,n));
      } else{
	rf=1./(static_cast<real>(n));
      }
      f=rf*l;
      v.resize(n+1);
      p = v.begin();
      (*p++) = xb;
      for(;p!=v.end();++p,re*=r){
	s+=f*re;
	*p = xb+s;
      }
      v.back() = xe; // not mandatory (xe is the value that shall be
		     // found) but this is more precise
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_DISCRETIZATION1D_IXX */

