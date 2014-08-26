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
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Matrix/MatrixConcept.hxx"
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
	const IndexType n = m.getNbRows();
	int d = 1;

	for(IndexType i=0;i!=n;++i){
	  // L update (column)
	  if(p.isIdentity()){
	    for(IndexType j=i;j!=n;++j){
	      NumType v = NumType(0);
	      for(IndexType k=0;k!=i;++k){
		v += m(j,k)*m(k,i);
	      }
	      m(j,i) -= v;
	    }
	  } else {
	    for(IndexType j=i;j!=n;++j){
	      IndexType  pj = p(j);
	      NumType v = NumType(0);
	      for(IndexType k=0;k!=i;++k){
		v += m(pj,k)*m(p(k),i);
	      }
	      m(pj,i) -= v;
	    }
	  }
	  // search for pivot
	  IndexType piv = i;
	  if(p.isIdentity()){
	    NumType cmax = abs(m(i,i));
	    for(IndexType j=static_cast<IndexType>(i+1u);j!=n;++j){
	      if(abs(m(j,i))>cmax){
		cmax = abs(m(j,i));
		piv = j;
	      }
	    }
	    if(piv!=i){
	      if(!((abs(m(i,i))>0.1*cmax)&&
		   (abs(m(i,i))>eps))){
		d *= -1;
		p.swap(piv,i);
	      }
	    }
	  } else {
	    NumType cmax = abs(m(p(i),i));
	    for(IndexType j=static_cast<IndexType>(i+1u);j!=n;++j){
	      IndexType  pj = p(j);
	      if(abs(m(pj,i))>cmax){
		cmax = abs(m(pj,i));
		piv = j;
	      }
	    }
	    if(piv!=i){
	      if(!((abs(m(p(i),i))>0.1*cmax)&&
		   (abs(m(p(i),i))>eps))){
		d *= -1;
		p.swap(piv,i);
	      }
	    }
	  }
	  if(abs(m(p(i),i))<eps){
	    throw(LUException("LUDecomp::exe : null pivot"));
	  }
	  if(p.isIdentity()){
	    for(IndexType j=static_cast<IndexType>(i+1);j!=n;++j){
	      NumType v = NumType(0);
	      for(IndexType k=0;k!=i;++k){
		v += m(i,k)*m(k,j);
	      }
	      m(i,j) -= v;
	      m(i,j) /= m(i,i);
	    }
	  } else {
	    IndexType pi = p(i);
	    // U update
	    for(IndexType j=static_cast<IndexType>(i+1);j!=n;++j){
	      for(IndexType k=0;k!=i;++k){
		m(pi,j) -= m(pi,k)*m(p(k),j);
	      }
	      m(pi,j) /= m(pi,i);
	    }
	  }
	}
	return d;
      } // end of exe
      
    }; // end of struct LUDecomp

  }// end of namespace math

}// end of namespace tfel

#endif /* _LIB_TFEL_MATH_LUDECOMP_H */

