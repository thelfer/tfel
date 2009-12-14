/*!
 * \file   LUSolve.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 Jul 2006
 */

#ifndef _LIB_TFEL_MATH_LUSOLVE_HXX_
#define _LIB_TFEL_MATH_LUSOLVE_HXX_ 

#include"Config/TFELConfig.hxx"
#include"Math/Matrix/MatrixConcept.hxx"
#include"Math/LU/LUException.hxx"
#include"Math/LU/Permutation.hxx"
#include"Math/LU/LUDecomp.hxx"

namespace tfel{

  namespace math{

    struct LUSolve
    {

      template<typename MatrixType,typename VectorType>
      static TFEL_MATH_INLINE2
      void exe(MatrixType& m,
	       VectorType& b)
      {
	using namespace std;
	typedef typename MatrixTraits<MatrixType>::IndexType IndexType;
	typedef typename MatrixTraits<MatrixType>::NumType NumType;
	if(m.getNbRows()!=m.getNbCols()){
	  throw(LUException("matrix is not square"));
	}
	if(m.getNbRows()!=b.size()){
	  throw(LUException("matrix size and vector size does not match"));
	}
	if(m.getNbRows()==0){
	  throw(LUException("invalid matrix size"));
	}
	Permutation<IndexType> p(m.getNbRows());
	LUDecomp::exe(m,p);

	IndexType i,j;
	IndexType pi;
	IndexType pi2;
	IndexType n = m.getNbRows();

	VectorType x(n);
	copy(b.begin(),b.end(),x.begin());

	for(i=0;i!=n;++i){
	  pi = p(i);
	  for(j=0;j!=i;++j){
	    x(pi) -= m(pi,j)*x(p(j));
	  }
	  x(pi) /= m(pi,i);
	}
	
	b(n-1) = x(p(n-1));
	for(i=n-1;i!=0;--i){
	  pi2 = i-1;
	  pi  = p(pi2);
	  b(pi2) = x(pi);
	  for(j=i;j!=n;++j){
	    b(pi2) -= m(pi,j)*b(j);
	  }
	}

      } // end of LUSolve::exe	

    }; // end of struct LUSolve

  } // end of namespace math

} // end of namespace tfel
  

#endif /* _LIB_TFEL_MATH_LUSOLVE_HXX */

