/*! 
 * \file  LUDecomp.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 avr 2009
 */

#ifndef _LIB_TFEL_MATH_LUDECOMP_H_
#define _LIB_TFEL_MATH_LUDECOMP_H_ 

#include<cmath>
#include<limits>
#include<algorithm>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Matrix/MatrixConcept.hxx"
#include"TFEL/Math/LU/Permutation.hxx"
#include"TFEL/Math/LU/LUException.hxx"

namespace tfel{

  namespace math{

    /*!
     * structure in charge of computing the LU decomposition of a
     * matrix. Line permutation is performed.
     */ 
    struct LUDecomp
    {

      /*!
       * Compute the LU decomposition of a matrix. Line permutation is
       * performed.
       *
       * \param m   : the matrix to be decomposed
       * \param p   : the permutation vector
       * \param eps : numerical parameter used to detect null pivot
       * \return the number of permutation made
       * 
       * \note The decomposition is done in-place
       */ 
      template<typename MatrixType,typename PermutationType>
      static TFEL_MATH_INLINE2
      int
      exe(MatrixType& m,
	  PermutationType& p,
	  const typename MatrixTraits<MatrixType>::NumType eps =
	  100*std::numeric_limits<typename MatrixTraits<MatrixType>::NumType>::min())
      {
	using namespace std;
	typedef typename MatrixTraits<MatrixType>::IndexType IndexType;
	typedef typename MatrixTraits<MatrixType>::NumType NumType;
	if(m.getNbRows()!=m.getNbCols()){
	  throw(LUException("LUDecomp::exe : matrix is not square"));
	}
	if(m.getNbRows()!=p.size()){
	  throw(LUException("LUDecomp::exe : matrix size and permutation size does not match"));
	}
	if(m.getNbRows()==0){
	  throw(LUException("LUDecomp::exe : invalid matrix size"));
	}
	IndexType i,j,k;
	IndexType pi;
	IndexType pj;
	IndexType n = m.getNbRows();
	int d = 1;

	for(i=0;i!=n;++i){
	  // L update (column)
	  for(j=i;j!=n;++j){
	    pj = p(j);
	    for(k=0;k!=i;++k){
	      m(pj,i) -= m(pj,k)*m(p(k),i);
	    }
	  }
	  // search for pivot
	  NumType cmax = abs(m(p(i),i));
	  k = i;
	  for(j=i+1;j!=n;++j){
	    pj = p(j);
	    if(abs(m(pj,i))>cmax){
	      cmax = abs(m(pj,i));
	      k = j;
	    }
	  }
	  if(k!=i){
	    if(!((abs(m(p(i),i))>0.1*cmax)&&
		 (abs(m(p(i),i))>eps))){
	      d *= -1;
	      swap(p(k),p(i));
	    }
	  }
	  if(abs(m(p(i),i))<eps){
	    throw(LUException("LUDecomp::exe : null pivot"));
	  }
	  pi = p(i);
	  // U update
	  for(j=i+1;j!=n;++j){
	    for(k=0;k!=i;++k){
	      m(pi,j) -= m(pi,k)*m(p(k),j);
	    }
	    m(pi,j) /= m(pi,i);
	  }
	}
	return d;
      } // end of exe
      
    }; // end of struct LUDecomp

  }// end of namespace math

}// end of namespace tfel

#endif /* _LIB_TFEL_MATH_LUDECOMP_H */

