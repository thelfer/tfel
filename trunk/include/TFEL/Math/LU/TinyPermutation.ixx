/*!
 * \file   TinyPermutation.ixx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   01 Aug 2006
 */

#ifndef _LIB_TFEL_MATH_TINYPERMUTATION_I_
#define _LIB_TFEL_MATH_TINYPERMUTATION_I_ 

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

namespace tfel{

  namespace math{

    template<unsigned short N>
    TFEL_MATH_INLINE TinyPermutation<N>::TinyPermutation()
    {
      static const unsigned short zero = 0u;
      tfel::fsalgo::iota<N>::exe(this->begin(),zero);
    }

    template<unsigned short N>
    template<typename T>
    TFEL_MATH_INLINE void TinyPermutation<N>::exe(tvector<N,T>& vec)
    {
      tvector<N,T> tmp;
      unsigned short i;
      for(i=0;i<N;++i){
	tmp(i) = vec(*(this->v+i));
      }
      vec = tmp;
    }

  } // end of namespace math

} // end of namespace tfel


#endif /* _LIB_TFEL_MATH_TINYPERMUTATION_I_ */

