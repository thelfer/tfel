/*!
 * \file   CovarianceMatrix.ixx
 * \brief    
 * \author Étienne Castelier
 * \date   10 oct. 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_COVARIANCEMATRIX_IXX
#define LIB_COVARIANCEMATRIX_IXX

#include<stdexcept>

namespace tfel{
  
  namespace math{

    namespace anderson{
  
      // Default constructor
      template<typename real>
      CovarianceMatrix<real>::CovarianceMatrix() = default;

      // Constructor
      // Nmax Matrix size
      template<typename real>
      CovarianceMatrix<real>::CovarianceMatrix(const size_type n)
	: N(0)
      {
	this->resize(n);
      }

      // Resize the matrix
      // Nmax Matrix size
      template<typename real>
      void CovarianceMatrix<real>::resize(const size_type n) {
	this->Nmax = n;
	if(this->Nmax){
	  this->C.resize((this->Nmax*(this->Nmax+1))/2);
	  this->a.resize((this->Nmax*(this->Nmax-1))/2);
	  this->ne.resize(this->Nmax);
	}
      }

      // Return the matrix size
      template<typename real>
      typename CovarianceMatrix<real>::size_type
      CovarianceMatrix<real>::getSize() const {
	return this->N;
      }

      // Shift the covariances
      template<typename real>
      void CovarianceMatrix<real>::shift() {
	pointer Cij=C.begin();
	for (size_type i=1; i<N; ++i) {
	  for (size_type j=0; j<i; ++j,++Cij) {
	    *Cij=Cij[i+1];
	  }
	}
      }

      // N Line index
      // return the address of the ith line
      template<typename real>
      typename CovarianceMatrix<real>::pointer
      CovarianceMatrix<real>::getLine(const size_type i) {
	if (i >= this->Nmax) {
	  throw(std::runtime_error("CovarianceMatrix<real>::getLine: "
				   "line index cannot exceed the matrix size"));
	}
	if (i>this->N) {
	  throw(std::runtime_error("CovarianceMatrix<real>::getLine: "
				   "internal error (missing lines)"));
	}
	this->N=i+1;
	return this->C.begin()+(i*(i+1))/2;
      }

      // Gram-Schmidt Factorisation
      // Descendent order
      template<typename real>
      void CovarianceMatrix<real>::GSFactorD() {
	pointer ai = this->a.begin();
	const_pointer Di  = this->C.begin()+((N+2)*(N-1))/2;
	const_pointer Dii = Di;
	for (size_type i=0; i<N; ai+=i,Dii-=N-i,++i,--Di) {
  
	  // Ratio ei.xj/ej.ej
	  pointer aij=ai;
	  const_pointer Dij=Di;
	  pointer ej=ne.begin();
	  for (size_type j=0; j<i; ++j,++aij,++ej,Dij-=N-j) {
	    if (*ej > 0) {
	      *aij=*Dij;
	      const_pointer Dik = Di;
	      const_pointer ajk = this->a.begin()+(j*(j-1))/2;
	      for (size_type k=0; k<j; ++k,++ajk,Dik-=N-k) {
		*aij-=*ajk*(*Dik);
	      }
	      *aij/=*ej;
	    }
	    else {
	      *aij=0;
	    }
	  }

	  // Alpha(ji) Coefficient
	  aij=ai;
	  for (size_type j=0; j<i; ++j,++aij) {
	    size_type k;
	    const_pointer akj = this->a.begin()+((j+3)*j)/2;
	    const_pointer aik;
	    for (k=j+1,aik=ai+k; k<i; akj+=k,++aik,++k) {
	      *aij-=*aik*(*akj);
	    }
	  }

	  // Norm
	  ej=ne.begin()+i;
	  *ej=*Dii;
	  aij=ai;
	  Dij=Di;
	  for (size_type j=0; j<i; ++j,++aij,Dij-=N-j) {
	    *ej-=*aij*(*Dij);
	  }

	  // Degeneration
	  if (*ej < C[0]*1e-14) {
	    // cerr << "Dégénéré " << (int)i << endl;
	    *ej=0;
	  }
	}
      }


      // Anderson weights by Gram-Schmidt process
      // Descendent order
      // w: Anderson weights
      template<typename real>
      void
      CovarianceMatrix<real>::weightsGSchmidtD(std::vector<real>& w) {
	this->GSFactorD();
	pointer wN=w.begin()+this->N-1;
	size_type i = 0;
	pointer iw = wN;
	const_pointer nei =ne.begin();
	for (;i<this->N;++i,--iw,++nei) {
	  if (*nei > 0) {
	    *iw=1;
	    const_pointer aij=this->a.begin()+(i*(i-1))/2;
	    for (size_type j=0; j<i; ++j,++aij) {
	      *iw-=*aij;
	    }
	    *iw/=*nei;
	  }
	  else {
	    *iw=0;
	  }
	}

	real d=0;
	for (i=0,iw=wN; i<N; ++i,--iw) {
	  const_pointer aji=this->a.begin()+((i+3)*i)/2;
	  size_type j;
	  const_pointer jw;
	  for (j=i+1,jw=iw-1; j<N; aji+=j,++j,--jw) {
	    *iw-=*aji*(*jw);
	  }
	  d+=*iw;
	}

	for (i=0,iw=w.begin(); i<N; ++i,++iw) {
	  *iw/=d;
	}
      }

      //! Destructor
      template<typename real>
      CovarianceMatrix<real>::~CovarianceMatrix() = default;
      
    } // end of  namespace anderson
  
  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_COVARIANCEMATRIX_IXX */
