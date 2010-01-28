/*!
 * \file   StensorExport.ixx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   07 Aug 2006
 */

#ifndef _LIB_TFEL_STENSOREXPORT_I_
#define _LIB_TFEL_STENSOREXPORT_I_ 

#include<cmath>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"

namespace tfel{

  namespace math {

    namespace internals{

      template<>
      struct ExportToTab<1u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	}
      };

      template<>
      struct ExportToTab<2u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  static const typename tfel::typetraits::BaseType<T>::type CSTE_1_SQRT2 = 1./std::sqrt(static_cast<typename tfel::typetraits::BaseType<T>::type>(2.));
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	  target[3] = src[3]*CSTE_1_SQRT2;
	}
      };

      template<>
      struct ExportToTab<3u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  static const typename tfel::typetraits::BaseType<T>::type CSTE_1_SQRT2 = 1./std::sqrt(static_cast<typename tfel::typetraits::BaseType<T>::type>(2.));
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	  target[3] = src[3]*CSTE_1_SQRT2;
	  target[4] = src[4]*CSTE_1_SQRT2;
	  target[5] = src[5]*CSTE_1_SQRT2;
	}
      };

      template<>
      struct ExportToVoigt<1u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	}
      };

      template<>
      struct ExportToVoigt<2u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  static const typename tfel::typetraits::BaseType<T>::type CSTE_SQRT2 = std::sqrt(static_cast<typename tfel::typetraits::BaseType<T>::type>(2.));
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	  target[3] = src[3]*CSTE_SQRT2;
	}
      };

      template<>
      struct ExportToVoigt<3u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  static const typename tfel::typetraits::BaseType<T>::type CSTE_SQRT2 = std::sqrt(static_cast<typename tfel::typetraits::BaseType<T>::type>(2.));
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


#endif /* _LIB_TFEL_STENSOREXPORT_I_ */

