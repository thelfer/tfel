/*! 
 * \file  LUDecomp.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 avr 2009
 */

#ifndef _LIB_LUDECOMP_H_
#define _LIB_LUDECOMP_H_ 

#include<algorithm>
#include<limits>
#include<cmath>

#include"Config/TFELConfig.hxx"
#include"Math/Matrix/MatrixConcept.hxx"
#include"Math/LU/Permutation.hxx"
#include"Math/LU/LUException.hxx"

namespace tfel{

  namespace math{

    struct LUDecomp
    {

      template<typename MatrixType,typename PermutationType>
      static TFEL_MATH_INLINE2
      short
      exe(MatrixType& m,PermutationType& p)
      {
	using namespace std;
	typedef typename MatrixTraits<MatrixType>::IndexType IndexType;
	typedef typename MatrixTraits<MatrixType>::NumType NumType;
	if(m.getNbRows()!=m.getNbCols()){
	  throw(LUException("matrix is not square"));
	}
	if(m.getNbRows()!=p.size()){
	  throw(LUException("matrix size and permutation size does not match"));
	}
	if(m.getNbRows()==0){
	  throw(LUException("invalid matrix size"));
	}
	IndexType i,j,k;
	IndexType pi;
	IndexType pj;
	IndexType n = m.getNbRows();
	NumType mabs = abs(m(0,0));
	short d = 1;
	for(i=0;i!=n;++i){
	  for(j=0;j!=n;++j){
	    if(abs(m(i,j))>mabs){
	      mabs = abs(m(i,j));
	    }
	  }
	}

	NumType prec = mabs*numeric_limits<NumType>::epsilon();
	
	if(prec<=100*numeric_limits<NumType>::min()){
	  throw(LUException("absolute value is too small"));
	}

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
	    d *= -1;
	    swap(p(k),p(i));
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

#endif /* _LIB_LUDECOMP_H */

