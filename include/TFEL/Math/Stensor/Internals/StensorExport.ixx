/*!
 * \file   include/TFEL/Math/Stensor/Internals/StensorExport.ixx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   07 Aug 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
      struct TFEL_VISIBILITY_LOCAL ExportToTab<1u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	}
      };

      template<>
      struct TFEL_VISIBILITY_LOCAL ExportToTab<2u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  static constexpr typename tfel::typetraits::BaseType<T>::type CSTE_1_SQRT2 = 1./std::sqrt(static_cast<typename tfel::typetraits::BaseType<T>::type>(2.));
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	  target[3] = src[3]*CSTE_1_SQRT2;
	}
      };

      template<>
      struct TFEL_VISIBILITY_LOCAL ExportToTab<3u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  static constexpr typename tfel::typetraits::BaseType<T>::type CSTE_1_SQRT2 = 1./std::sqrt(static_cast<typename tfel::typetraits::BaseType<T>::type>(2.));
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	  target[3] = src[3]*CSTE_1_SQRT2;
	  target[4] = src[4]*CSTE_1_SQRT2;
	  target[5] = src[5]*CSTE_1_SQRT2;
	}
      };

      template<>
      struct TFEL_VISIBILITY_LOCAL ExportToVoigt<1u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	}
      };

      template<>
      struct TFEL_VISIBILITY_LOCAL ExportToVoigt<2u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  static constexpr typename tfel::typetraits::BaseType<T>::type CSTE_SQRT2 = std::sqrt(static_cast<typename tfel::typetraits::BaseType<T>::type>(2.));
	  target[0] = src[0];
	  target[1] = src[1];
	  target[2] = src[2];
	  target[3] = src[3]*CSTE_SQRT2;
	}
      };

      template<>
      struct TFEL_VISIBILITY_LOCAL ExportToVoigt<3u>{
	template<typename T>
	TFEL_MATH_INLINE static void exe(const T* const src,T * const target)
	{
	  static constexpr typename tfel::typetraits::BaseType<T>::type CSTE_SQRT2 = std::sqrt(static_cast<typename tfel::typetraits::BaseType<T>::type>(2.));
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

