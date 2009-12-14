/*!
 * \file   MatrixUtilities.hxx  
 * \brief  This file implements various helper functions used by tmatrix.
 * \author Helfer Thomas
 * \date   05 May 2006
 */

#ifndef _LIB_TFEL_MATRIX_UTILITIES_H_
#define _LIB_TFEL_MATRIX_UTILITIES_H_ 

#include "Config/TFELConfig.hxx"

namespace tfel{
  
  namespace math {
    
    template<unsigned short N,unsigned short M,unsigned short M_init>
    struct matrix_utilities{

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void copy(const T& a, T2& b){
	b(N-1,M-1) = a(N-1,M-1);
	matrix_utilities<N,M-1,M_init>::copy(a,b);
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void plusEqual(T& a,const T2& b){
	a(N-1,M-1) += b(N-1,M-1);
	matrix_utilities<N,M-1,M_init>::plusEqual(a,b);
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void minusEqual(T& a,const T2& b){
	a(N-1,M-1) -= b(N-1,M-1);
	matrix_utilities<N,M-1,M_init>::minusEqual(a,b);
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void assign_to_scalar(T& a, const T2 b){
	a(N-1,M-1) = b;
	matrix_utilities<N,M-1,M_init>::assign_to_scalar(a,b);	
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void multByScalar(T& a, const T2 b){
	a(N-1,M-1) *= b;
	matrix_utilities<N,M-1,M_init>::multByScalar(a,b);	
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void divByScalar(T& a, const T2 b){
	a(N-1,M-1) /= b;
	matrix_utilities<N,M-1,M_init>::divByScalar(a,b);	
      }
      
    };

    template<unsigned short N,unsigned short M_init>
    struct matrix_utilities<N,0u,M_init>{

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void copy(const T& a, T2& b){
	matrix_utilities<N-1,M_init,M_init>::copy(a,b);
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void plusEqual(T& a,const T2& b){
	matrix_utilities<N-1,M_init,M_init>::plusEqual(a,b);
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void minusEqual(T& a,const T2& b){
	matrix_utilities<N-1,M_init,M_init>::minusEqual(a,b);
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void assign_to_scalar(T& a, const T2 b){
	matrix_utilities<N-1,M_init,M_init>::assign_to_scalar(a,b);	
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void multByScalar(T& a, const T2 b){
	matrix_utilities<N-1,M_init,M_init>::multByScalar(a,b);	
      }

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void divByScalar(T& a, const T2 b){
	matrix_utilities<N-1,M_init,M_init>::divByScalar(a,b);	
      }

      
    };

    template<unsigned short M,unsigned short M_init>
    struct matrix_utilities<0u,M,M_init>{

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void copy(const T&,const T2&){}

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void plusEqual(const T&,const T2&){}

      template<typename T, typename T2>
      static TFEL_MATH_INLINE 
      void minusEqual(const T&,const T2&){}

      template<typename T, typename T2>
      static TFEL_MATH_INLINE
      void assign_to_scalar(const T&,const T2){}
      
      template<typename T, typename T2>
      static TFEL_MATH_INLINE
      void multByScalar(const T&,const T2){}

      template<typename T, typename T2>
      static TFEL_MATH_INLINE
      void divByScalar(const T&,const T2){}

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATRIX_UTILITIES_H */

