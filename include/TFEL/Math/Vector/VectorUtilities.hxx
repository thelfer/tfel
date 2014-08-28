/*!
 * \file   include/TFEL/Math/Vector/VectorUtilities.hxx  
 * \brief  This file implements various helper functions used by tvector.
 * \author Helfer Thomas
 * \date   05 May 2006
 */

#ifndef _LIB_TFEL_VECTOR_UTILITIES_H_
#define _LIB_TFEL_VECTOR_UTILITIES_H_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{
  
  namespace math {
    
    template<unsigned short N>
    struct VectorUtilities{

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void copy(const T& a, T2& b){
	b(N-1) = a(N-1);
	VectorUtilities<N-1>::copy(a,b);
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void PlusEqual(T& b, const T2& a){
	b(N-1) += a(N-1);
	VectorUtilities<N-1>::PlusEqual(b,a);
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void MinusEqual(T& b, const T2& a){
	b(N-1) -= a(N-1);
	VectorUtilities<N-1>::MinusEqual(b,a);
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void MultEqual(T& b, const T2& a){
	b(N-1) *= a(N-1);
	VectorUtilities<N-1>::MinusEqual(b,a);
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void assign_to_scalar(T& a, const T2& b){
	a(N-1) = b;
	VectorUtilities<N-1>::assign_to_scalar(a,b);	
      }
      
      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void scale(T& a, const T2& b){
	a(N-1) *= b;
	VectorUtilities<N-1>::scale(a,b);	
      }

    };

    template<>
    struct VectorUtilities<0u>{

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void copy(const T&, T2&){}

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void PlusEqual(T&, const T2&){}

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void MinusEqual(T&, const T2&){}

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void MultEqual(T&, const T2&){}

      template<typename T, typename T2>
      static TFEL_MATH_INLINE
      void assign_to_scalar(T&, const T2&){}

      template<typename T, typename T2>
      static TFEL_MATH_INLINE
      void scale(T&, const T2&){}
      
    };

    template<unsigned short N>
    struct vectorToTab{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T& src, T2& dest)
      {
	dest[N-1] = src(N-1);
	vectorToTab<N-1>::exe(src,dest);
      }
    };

    template<>
    struct vectorToTab<0u>{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T&, T2&)
      {}
    };

    template<>
    struct vectorToTab<1u>{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T& src, T2& dest)
      {
	dest[0] = src(0);
      }
    };

    template<>
    struct vectorToTab<2u>{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T& src, T2& dest)
      {
	dest[1] = src(1);
	dest[0] = src(0);
      }
    };

    template<>
    struct vectorToTab<3u>{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T& src, T2& dest)
      {
	dest[2] = src(2);
	dest[1] = src(1);
	dest[0] = src(0);
      }
    };

    template<>
    struct vectorToTab<4u>{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T& src, T2& dest)
      {
	dest[3] = src(3);
	dest[2] = src(2);
	dest[1] = src(1);
	dest[0] = src(0);
      }
    };

    template<>
    struct vectorToTab<5u>{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T& src, T2& dest)
      {
	dest[4] = src(4);
	dest[3] = src(3);
	dest[2] = src(2);
	dest[1] = src(1);
	dest[0] = src(0);
      }
    };

    template<>
    struct vectorToTab<6u>{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T& src, T2& dest)
      {
	dest[5] = src(5);
	dest[4] = src(4);
	dest[3] = src(3);
	dest[2] = src(2);
	dest[1] = src(1);
	dest[0] = src(0);
      }
    };

    template<>
    struct vectorToTab<7u>{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T& src, T2& dest)
      {
	dest[6] = src(6);	
	dest[5] = src(5);
	dest[4] = src(4);
	dest[3] = src(3);
	dest[2] = src(2);
	dest[1] = src(1);
	dest[0] = src(0);
      }
    };

    template<>
    struct vectorToTab<8u>{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T& src, T2& dest)
      {
	dest[7] = src(7);	
	dest[6] = src(6);	
	dest[5] = src(5);
	dest[4] = src(4);
	dest[3] = src(3);
	dest[2] = src(2);
	dest[1] = src(1);
	dest[0] = src(0);
      }
    };

    template<>
    struct vectorToTab<9u>{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T& src, T2& dest)
      {
	dest[8] = src(8);	
	dest[7] = src(7);	
	dest[6] = src(6);	
	dest[5] = src(5);
	dest[4] = src(4);
	dest[3] = src(3);
	dest[2] = src(2);
	dest[1] = src(1);
	dest[0] = src(0);
      }
    };

    template<>
    struct vectorToTab<10u>{
      template<typename T,typename T2>
      static TFEL_MATH_INLINE
      void exe(T& src, T2& dest)
      {
	dest[9] = src(9);	
	dest[8] = src(8);	
	dest[7] = src(7);	
	dest[6] = src(6);	
	dest[5] = src(5);
	dest[4] = src(4);
	dest[3] = src(3);
	dest[2] = src(2);
	dest[1] = src(1);
	dest[0] = src(0);
      }
    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_VECTOR_UTILITIES_H */

