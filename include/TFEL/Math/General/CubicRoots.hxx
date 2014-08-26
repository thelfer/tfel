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

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Utilities/Info.hxx"

#include"TFEL/Metaprogramming/EnableIf.hxx"

#include"TFEL/TypeTraits/IsReal.hxx"
#include"TFEL/TypeTraits/IsFundamentalNumericType.hxx"

namespace tfel{
  
  namespace math{

    struct CubicRoots
    {

#if not (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#if defined _BSD_SOURCE || defined _SVID_SOURCE || defined _ISOC99_SOURCE || ((defined _XOPEN_SOURCE) && (_XOPEN_SOURCE >= 600))  || ((defined _POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L))

      static TFEL_MATH_INLINE float
      cbrt(const float x){
	return ::cbrtf(x);
      }
    
      static TFEL_MATH_INLINE double
      cbrt(const double x){
	return ::cbrt(x);
      }

      static TFEL_MATH_INLINE long double
      cbrt(const long double x){
	return ::cbrtl(x);
      }
#endif
#endif /* not (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */

      template<typename T>
      static TFEL_MATH_INLINE T
      cbrt(const T x){
	return ::pow(x,T(1)/T(3));
      }

      template<typename T>
      static TFEL_MATH_INLINE2 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsReal<T>::cond &&
        tfel::typetraits::IsFundamentalNumericType<T>::cond,
	bool
	>::type
      exe(T& x1,
	  T& x2,
	  T& x3,
	  const T a3,
	  const T a2,
	  const T a1,
	  const T a0,
	  const bool b = false)
      {	
      	static const T C_PI       = T(2)*std::atan(T(1));
	static const T C_1_3      = T(1)/3;
	static const T C_2_3      = T(2)*C_1_3;
	static const T C_2_27     = T(2)/T(27);
	static const T C_27_2     = T(27)/T(2);
	static const T C_3SQRT3_2 = std::sqrt(T(3))*T(3)/T(2);
	static const T C_SQRT3_3  = std::sqrt(T(3))/T(3);
      
	const T prec = 100*std::numeric_limits<T>::min();
      
	if(std::abs(a3)<=prec){
	  TFEL_UTILITIES_INFO("coefficient a3 is too small");
	  return false;
	}
      
	T tmp   = T(1)/a3;
	T tmp2  = a2*tmp;
	T tmp3  = C_1_3*tmp2;
	T p     = tmp*(a1-tmp3*a2);
	T q     = tmp*(a0-tmp3*a1+C_2_27*tmp2*tmp2*a2);
	T delta = -T(4)*p*p*p-T(27)*q*q;
	T rho;
	T theta;
	T ux;
	T uy;
      
	if(delta<0){
	  if((std::abs(delta)<-100*std::abs(T(27)*q*q)*std::numeric_limits<T>::epsilon())&&
	     (std::abs(delta)<-100*std::abs(T(4)*p*p*p)*std::numeric_limits<T>::epsilon())){
	    TFEL_UTILITIES_INFO("determinant is negative : " << delta);
	    return false;
	  } else {
	    delta = T(0.);
	  }
	}
      
	tmp  = -C_27_2*q;
	tmp2 = C_3SQRT3_2*std::sqrt(delta);
	rho  = std::sqrt(tmp*tmp+tmp2*tmp2);

	if(rho<=prec){
	  theta = 0.;
	} else {
	  if(std::abs(tmp/rho+1.)<prec){
	    theta = T(C_PI);
	  } else {
	    theta = T(2)*std::atan(tmp2/(tmp+rho));
	  }
	}
 
	tmp  = CubicRoots::cbrt(rho);
	ux   = tmp*std::cos(theta*C_1_3);
	uy   = tmp*std::sin(theta*C_1_3);

	x1 = -tmp3 + C_2_3*ux; 
	x2 = -tmp3 - C_1_3*ux - C_SQRT3_3*uy;
	x3 = -tmp3 - C_1_3*ux + C_SQRT3_3*uy;
      
	if(b){
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
	}

	return true;
      }

    protected:

      template<typename T>
      static bool improve_x(const T a3, const T a2, const T a1, const T a0, T& x)
      {
	T df = T(3)*a3*x*x+T(2)*a2*x+a1;
	T f  = a3*x*x*x+a2*x*x+a1*x+a0;
	T x1;
	T prec = 10*std::max(std::numeric_limits<T>::min(),
			     std::abs(x)*std::numeric_limits<T>::epsilon());

	const unsigned short iter_max = 50;
	unsigned short iter;
	
	if(std::abs(df)<100*std::numeric_limits<T>::min()){
	  return false;
	}
	x1   = x-f/df;
	iter = 0;
	while((std::abs(x1-x)>prec)&&(iter<iter_max)){
	  x  = x1;
	  df = T(3)*a3*x*x+T(2)*a2*x+a1;
	  f  = a3*x*x*x+a2*x*x+a1*x+a0;
	  if(std::abs(df)<100*std::numeric_limits<T>::min()){
	    return false;
	  }
	  x1 = x-f/df;
	  ++iter;
	}
	return true;
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_CUBICROOTS_HXX */

