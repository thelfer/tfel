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
    TinyPermutation<N>::TinyPermutation()
      : is_identity(true)
    {
      tfel::fsalgo::iota<N>::exe(this->begin(),0u);
    }

    template<unsigned short N>
    const unsigned short&
    TinyPermutation<N>::operator[](const unsigned short i) const
    {
      return tvector<N,unsigned short>::operator[](i);
    } // end of TinyPermutation<N>::operator[]

    template<unsigned short N>
    const unsigned short&
    TinyPermutation<N>::operator()(const unsigned short i) const
    {
      return tvector<N,unsigned short>::operator[](i);
    } // end of TinyPermutation<N>::operator[]
      
    template<unsigned short N>
    void
    TinyPermutation<N>::swap(const unsigned short i,
			     const unsigned short j)
    {
      std::swap(tvector<N,unsigned short>::operator[](i),
		tvector<N,unsigned short>::operator[](j));
      this->is_identity = false;
    }

    template<unsigned short N>
    bool
    TinyPermutation<N>::isIdentity() const
    {
      return this->is_identity;
    }

    template<unsigned short N>
    template<typename T>
    void TinyPermutation<N>::exe(tvector<N,T>& vec) const
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

