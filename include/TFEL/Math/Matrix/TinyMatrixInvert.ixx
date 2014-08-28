/*! 
 * \file  include/TFEL/Math/Matrix/TinyMatrixInvert.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 08 mars 2013
 */

#ifndef _LIB_TFEL_MATH_TINYMATRIXINVERT_IXX_
#define _LIB_TFEL_MATH_TINYMATRIXINVERT_IXX_ 

namespace tfel{
  
  namespace math{

    template<unsigned short N,
	     typename T>
    void
    TinyMatrixInvert<N,T>::exe(tfel::math::tmatrix<N,N,T>& m,
			       const T eps)
    {
      tmatrix<N,N,T> t(m);
      TinyPermutation<N> p;
      TinyMatrixSolve<N,T>::decomp(t,p,eps);
      typename tmatrix<N,N,T>::size_type i;
      typename tmatrix<N,N,T>::size_type j;
      for(i=0;i!=N;++i){
	tvector<N,T> e(T(0));
	e(i) = 1.;
	TinyMatrixSolve<N,T>::back_substitute(t,p,e);
	for(j=0;j!=N;++j){
	  m(j,i)=e(j);
	}
      }
    } // end of TinyMatrixInvert::exe

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TINYMATRIXINVERT_IXX */

