/*! 
 * \file  include/TFEL/Math/Matrix/TinyMatrixInvert.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 08 mars 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_TINYMATRIXINVERT_IXX_
#define LIB_TFEL_MATH_TINYMATRIXINVERT_IXX_ 

namespace tfel{
  
  namespace math{

    template<unsigned short N,
	     typename T>
    void
    TinyMatrixInvert<N,T>::exe(tfel::math::tmatrix<N,N,T>& m,
			       const T eps)
    {
      constexpr const auto zero = T(0);
      constexpr const auto one  = T(1);
      tmatrix<N,N,T> t(m);
      TinyPermutation<N> p;
      TinyMatrixSolve<N,T>::decomp(t,p,eps);
      typename tmatrix<N,N,T>::size_type i;
      typename tmatrix<N,N,T>::size_type j;
      tvector<N,T> e(zero);
      for(i=0;i!=N;++i){
	e(i) = one;
	TinyMatrixSolve<N,T>::back_substitute(t,p,e);
	for(j=0;j!=N;++j){
	  m(j,i)=e(j);
	}
	e(i) = zero;
      }
    } // end of TinyMatrixInvert::exe

  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_TINYMATRIXINVERT_IXX_ */

