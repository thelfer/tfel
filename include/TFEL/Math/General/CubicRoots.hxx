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
#include <iostream>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Utilities/Info.hxx"

#include"TFEL/Metaprogramming/EnableIf.hxx"

#include"TFEL/TypeTraits/IsReal.hxx"
#include"TFEL/TypeTraits/IsFundamentalNumericType.hxx"

namespace tfel{
  
  namespace math{

    /*!
     * struct gathering tools function to compute the roots of a cubic
     * polynomial
     */
    struct CubicRoots
    {

#if not (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#if defined _BSD_SOURCE || defined _SVID_SOURCE || defined _ISOC99_SOURCE || ((defined _XOPEN_SOURCE) && (_XOPEN_SOURCE >= 600))  || ((defined _POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L))
      /*!
       * \return the cubic root of a real number of type float
       * \param[in] x : value
       */
      static TFEL_MATH_INLINE float
      cbrt(const float x){
	return ::cbrtf(x);
      }
      /*!
       * \return the cubic root of a real number of type double
       * \param[in] x : value
       */
      static TFEL_MATH_INLINE double
      cbrt(const double x){
	return ::cbrt(x);
      }
      /*!
       * \return the cubic root of a real number of type long double
       * \param[in] x : value
       */
      static TFEL_MATH_INLINE long double
      cbrt(const long double x){
	return ::cbrtl(x);
      }
#endif
#endif /* not (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
      /*!
       * \return the cubic root of a real number
       * \param[in] x : value
       */
      template<typename T>
      static TFEL_MATH_INLINE T
      cbrt(const T x){
	return ::pow(x,T(1)/T(3));
      }

      /*!
       * find the real roots of a cubic polynomial
       * \return the number of real roots.
       * \param[out] x1 : first  root
       * \param[out] x2 : second root
       * \param[out] x3 : third  root
       * \param[in]  a3 : coefficient of the polynom
       * \param[in]  a2 : coefficient of the polynom
       * \param[in]  a1 : coefficient of the polynom
       * \param[in]  a0 : coefficient of the polynom
       * \note due to numerical errors, this function may find only one real
       * root when three are expected (for example, when searching the
       * eigen values of a symmetric tensor). If only one real root is
       * found:
       * - x1 contains this real root
       * - x2 and x3 contain the real part of the two other roots
       */
      template<typename T>
      static TFEL_MATH_INLINE2 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsReal<T>::cond &&
        tfel::typetraits::IsFundamentalNumericType<T>::cond,
	unsigned short
	>::type
      find_roots(T& x1,
		 T& x2,
		 T& x3,
		 const T a3,
		 const T a2,
		 const T a1,
		 const T a0)
      {	
      	static const T C_PI       = T(4)*std::atan(T(1));
	static const T C_1_3      = T(1)/T(3);
	static const T C_2_3      = T(2)*C_1_3;
	static const T C_m1_27    = T(-1)/T(27);
	static const T C_2_27     = T(2)/T(27);
	static const T C_27_2     = T(27)/T(2);
	static const T C_3SQRT3_2 = std::sqrt(T(3))*T(3)/T(2);
	static const T C_SQRT3_3  = std::sqrt(T(3))*C_1_3;
      
	const T prec = 100*std::numeric_limits<T>::min();
      
	if(std::abs(a3)<=prec){
	  TFEL_UTILITIES_INFO("coefficient a3 is too small");
	  return 0u;
	}
      
	T tmp   = T(1)/a3;
	T tmp2  = a2*tmp;
	T tmp3  = C_1_3*tmp2;
	const T p = tmp*(a1-tmp3*a2);
	const T q  = tmp*(a0-tmp3*a1+C_2_27*tmp2*tmp2*a2);
	T delta = -T(4)*p*p*p-T(27)*q*q;

	if(delta<0){
	  const T tmp4 = std::sqrt(C_m1_27*delta);
	  const T u = CubicRoots::cbrt((-q+tmp4)/(T(2)));
	  const T v = CubicRoots::cbrt((-q-tmp4)/(T(2)));
	  const T upv = u+v;
	  x1 = upv-tmp3;
	  x2 = -upv/2-tmp3;
	  x3 = x2;
	  if(std::abs(u-v)<100*std::abs(upv)*std::numeric_limits<T>::epsilon()){
	    return 3u;
	  } else {
	    return 1u;
	  }
	}

	if(std::abs(delta)<prec){
	  if(std::abs(p)>prec){
	    const T tmp5 = 3*q/p;
	    x1 = tmp5   -tmp3;
	    x2 = -tmp5/2-tmp3;
	    x3 =  x2;
	    return 3u;
	  } else {
	    x1 = -tmp3;
	    x2 = -tmp3;
	    x3 = -tmp3;
	    return 3u;
	  }
	}

	tmp  = -C_27_2*q;
	tmp2 = C_3SQRT3_2*std::sqrt(delta);
	const T rho  = std::sqrt(tmp*tmp+tmp2*tmp2);

	T theta;
	if(rho<=prec){
	  theta = T(0);
	} else {
	  if(std::abs(tmp+rho)<prec){
	    theta = C_PI;
	  } else {
	    theta = T(2)*std::atan(tmp2/(tmp+rho));
	  }
	}
 
	tmp  = CubicRoots::cbrt(rho);
	const T ux = tmp*std::cos(theta*C_1_3);
	const T uy = tmp*std::sin(theta*C_1_3);

	x1 = -tmp3 + C_2_3*ux; 
	x2 = -tmp3 - C_1_3*ux - C_SQRT3_3*uy;
	x3 = -tmp3 - C_1_3*ux + C_SQRT3_3*uy;
      
	return 3u;
      }

      /*!
       * \return the number of real roots.
       * \param[out] x1 : first  root
       * \param[out] x2 : second root
       * \param[out] x3 : third  root
       * \param[in]  a3 : coefficient of the polynom
       * \param[in]  a2 : coefficient of the polynom
       * \param[in]  a1 : coefficient of the polynom
       * \param[in]  a0 : coefficient of the polynom
       * \param[in] b   : if true, use a newton raphson algorithm to
       *                  refine the roots found
       */
      template<typename T>
      static TFEL_MATH_INLINE2 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsReal<T>::cond &&
        tfel::typetraits::IsFundamentalNumericType<T>::cond,
	unsigned short
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
	const unsigned short nb = CubicRoots::find_roots(x1,x2,x3,a3,a2,a1,a0);
	if((nb>0)&&(b)){
	  T tmp3 = x1;
	  if(improve_x(a3,a2,a1,a0,x1)==false){
	    x1 = tmp3;
	  }
	  if(nb==3u){
	    tmp3 = x2;
	    if(improve_x(a3,a2,a1,a0,x2)==false){
	      x2 = tmp3;
	    }
	    tmp3 = x3;
	    if(improve_x(a3,a2,a1,a0,x3)==false){
	      x3 = tmp3;
	    }
	  }
	}
	return nb;
      } // end of exe

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

