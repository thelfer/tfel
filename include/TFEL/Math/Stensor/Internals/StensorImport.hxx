/*!
 * \file   include/TFEL/Math/Stensor/Internals/StensorImport.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   26 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_STENSORIMPORT_H_
#define _LIB_TFEL_STENSORIMPORT_H_ 

#include<cmath>
#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/TypeTraits/IsAssignableTo.hxx"
#include"TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace tfel{

  namespace math {

    namespace internals{
      
      template<unsigned int N>
      struct ImportFromVoigt;

      template<unsigned int N>
      struct ImportFromTab;

      template<>
      struct TFEL_VISIBILITY_LOCAL ImportFromVoigt<1u>{
	template<typename T,typename T2>
	static TFEL_MATH_INLINE typename std::enable_if<tfel::typetraits::IsAssignableTo<T2,T>::cond,void>::type
	exe(T* const target, const T2* const src)
	{
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	}
      };

      template<>
      struct TFEL_VISIBILITY_LOCAL ImportFromVoigt<2u>{
	template<typename T,typename T2>
	static TFEL_MATH_INLINE typename std::enable_if<tfel::typetraits::IsAssignableTo<T2,T>::cond,void>::type
	exe(T* const target, const T2* const src){
	  using real = typename tfel::typetraits::BaseType<T>::type;
	  using tfel::math::constexpr_fct::sqrt;
	  constexpr real CSTE_1_SQRT2 = real{1}/sqrt(real{2});
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	  target[3] = src[3]*CSTE_1_SQRT2;
	}
      };

      template<>
      struct TFEL_VISIBILITY_LOCAL ImportFromVoigt<3u>{
	template<typename T,typename T2>
	static TFEL_MATH_INLINE typename std::enable_if<tfel::typetraits::IsAssignableTo<T2,T>::cond,void>::type
	exe(T* const target, const T2 * const src){
	  using real = typename tfel::typetraits::BaseType<T>::type;
	  using tfel::math::constexpr_fct::sqrt;
	  constexpr real CSTE_1_SQRT2 = real{1}/sqrt(real{2});
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	  target[3] = src[3]*CSTE_1_SQRT2;
	  target[4] = src[4]*CSTE_1_SQRT2;
	  target[5] = src[5]*CSTE_1_SQRT2;
	}
      };

      template<>
      struct TFEL_VISIBILITY_LOCAL ImportFromTab<1u>{
	template<typename T,typename T2>
	static TFEL_MATH_INLINE typename std::enable_if<
	  tfel::typetraits::IsAssignableTo<T2,T>::cond,
	  void>::type
	exe(T* const target, const T2 * const src)
	{
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	}
      };

      template<>
      struct TFEL_VISIBILITY_LOCAL ImportFromTab<2u>{
	template<typename T,typename T2>
	static TFEL_MATH_INLINE typename std::enable_if<tfel::typetraits::IsAssignableTo<T2,T>::cond,void>::type
	exe(T* const target, const T2* const src){
	  using real = typename tfel::typetraits::BaseType<T>::type;
	  using tfel::math::constexpr_fct::sqrt;
	  constexpr real CSTE_SQRT2 = sqrt(real{2});
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	  target[3] = src[3]*CSTE_SQRT2;
	}
      };

      template<>
      struct TFEL_VISIBILITY_LOCAL ImportFromTab<3u>{
	template<typename T,typename T2>
	static TFEL_MATH_INLINE typename std::enable_if<tfel::typetraits::IsAssignableTo<T2,T>::cond,void>::type
	exe(T* const target, const T2* const src){
	  using real = typename tfel::typetraits::BaseType<T>::type;
	  using tfel::math::constexpr_fct::sqrt;
	  constexpr real CSTE_SQRT2 = sqrt(real{2});
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	  target[3] = src[3]*CSTE_SQRT2;
	  target[4] = src[4]*CSTE_SQRT2;
	  target[5] = src[5]*CSTE_SQRT2;
	}
      };
      
    } //end of namespace internals
    
  } //end of namespace math

} // end of namespace tfel


#endif /* _LIB_TFEL_STENSORIMPORT_H */

