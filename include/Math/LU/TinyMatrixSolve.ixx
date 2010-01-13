/*!
 * \file   TinyMatrixSolve.ixx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   02 Aug 2006
 */

#ifndef _LIB_TFEL_TINYMATRIXSOLVE_I_
#define _LIB_TFEL_TINYMATRIXSOLVE_I_ 

#include"Math/LU/LUDecomp.hxx"
#include"Math/LU/TinyPermutation.hxx"

namespace tfel{

  namespace math{


    template<unsigned short N,typename T>
    TFEL_MATH_INLINE2
    void
    TinyMatrixSolve<N,T>::exe(tmatrix<N,N,T>& m, tvector<N,T>& b,
			      const T eps)
    {
      typedef typename MatrixTraits<tmatrix<N,N,T> >::IndexType IndexType;
      TinyPermutation<N> p;
      tvector<N,T> x;
      IndexType i,j;
      IndexType pi;
      IndexType pi2;
      LUDecomp::exe(m,p);
      x = b;
      for(i=0;i!=N;++i){
	pi = p(i);
	for(j=0;j!=i;++j){
	  x(pi) -= m(pi,j)*x(p(j));
	}
	if(std::abs(m(pi,i))<eps){
	  throw(LUException("TinyMatrixSolve<N,T>::exe : null pivot"));
	}
	x(pi) /= m(pi,i);
      }
      b(N-1) = x(p(N-1));
      for(i=N-1;i!=0;--i){
	pi2 = i-1;
	pi  = p(pi2);
	b(pi2) = x(pi);
	for(j=i;j!=N;++j){
	  b(pi2) -= m(pi,j)*b(j);
	}
      }
    } // end of TinyMatrixSolve<N,T>::exe

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_TINYMATRIXSOLVE_I_ */

