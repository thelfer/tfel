/*!
 * \file   CubicRoots.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 jui 2006
 */

#ifndef _LIB_TFEL_MATH_CUBICROOTS_HXX_
#define _LIB_TFEL_MATH_CUBICROOTS_HXX_ 

#include <cmath>
#include <limits>
#include <algorithm>

#include "Config/TFELConfig.hxx"

#include "Utilities/Info.hxx"

#include "Metaprogramming/EnableIf.hxx"

#include "TypeTraits/IsReal.hxx"
#include "TypeTraits/IsFundamentalNumericType.hxx"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* M_PI */

namespace tfel{
  
  namespace math{

    class cubicRoots{
    
      template<typename T>
      static bool improve_x(const T a3, const T a2, const T a1, const T a0, T& x)
      {
	T df = 3*a3*x*x+2*a2*x+a1;
	T f  = a3*x*x*x+a2*x*x+a1*x+a0;
	T x1;
	T prec = 10*std::max(std::numeric_limits<T>::min(),
			     std::abs(x)*std::numeric_limits<T>::epsilon());

	const unsigned short iter_max = 50;
	unsigned short iter;
	
	if(std::abs(df)<10*std::numeric_limits<T>::min()){
	  return false;
	}
	x1   = x-f/df;
	iter = 0;
	while((std::abs(x1-x)>prec)&&(iter<iter_max)){
	  x  = x1;
	  df = 3*a3*x*x+2*a2*x+a1;
	  f  = a3*x*x*x+a2*x*x+a1*x+a0;
	  if(std::abs(df)<10*std::numeric_limits<T>::min()){
	    return false;
	  }
	  x1 = x-f/df;
	  ++iter;
	}
	return true;
      }

    public:

      template<typename T>
      static TFEL_MATH_INLINE2 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsReal<T>::cond &&
        tfel::typetraits::IsFundamentalNumericType<T>::cond,
	bool
	>::type
      exe(const T a3,const T a2,const T a1,const T a0, T& x1, T& x2, T& x3)
      {	
      
	static const T M_1_3      = static_cast<T>(1)/3;
	static const T M_2_3      = static_cast<T>(2)/3;
	static const T M_2_27     = static_cast<T>(2)/27;
	static const T M_3SQRT3_2 = std::sqrt(static_cast<T>(3))*1.5f;
	static const T M_SQRT3_3  = std::sqrt(static_cast<T>(3))/3;
      
	const T prec = 100*std::numeric_limits<T>::min();
      
	if(std::abs(a3)<=prec){
	  TFEL_UTILITIES_INFO("coefficient a3 is too small");
	  return false;
	}
      
	T tmp   = static_cast<T>(1)/a3;
	T tmp2  = a2*tmp;
	T tmp3  = M_1_3*tmp2;
	T p     = tmp*(a1-tmp3*a2);
	T q     = tmp*(a0-tmp3*a1+M_2_27*tmp2*tmp2*a2);
	T delta = -4.f*p*p*p-27.f*q*q;
	T rho;
	T theta;
	T ux;
	T uy;
      
	if(delta<0){
	  if(delta<-100*std::numeric_limits<T>::epsilon()){
	    TFEL_UTILITIES_INFO("determinant is negative : " << delta);
	    return false;
	  } else {
	    delta = static_cast<T>(0.);
	  }
	}
      
	tmp  = -13.5f*q;
	tmp2 = M_3SQRT3_2*std::sqrt(delta);
	rho  = std::sqrt(tmp*tmp+tmp2*tmp2);

	if(rho<=prec){
	  theta = 0.;
	} else {
	  if(std::abs(tmp/rho+1.)<prec){
	    theta = static_cast<T>(M_PI);
	  } else {
	    theta = 2*std::atan(tmp2/(tmp+rho));
	  }
	}
 
	tmp  = std::pow(rho,M_1_3);
	ux   = tmp*std::cos(theta*M_1_3);
	uy   = tmp*std::sin(theta*M_1_3);

	x1 = -tmp3 + M_2_3*ux; 
	x2 = -tmp3 - M_1_3*ux - M_SQRT3_3*uy;
	x3 = -tmp3 - M_1_3*ux + M_SQRT3_3*uy;
      
	tmp3 = x1;
	if(improve_x(a3,a2,a1,a0,x1)==false){
	  x1 = tmp3;
	}

	tmp3 = x2;
	if(improve_x(a3,a2,a1,a0,x2)==false){
	  x2 = tmp3;
	}

	tmp3 = x3;
	if(improve_x(a3,a2,a1,a0,x3)==false){
	  x3 = tmp3;
	}

	return true;
      }
    
    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_CUBICROOTS_HXX */

