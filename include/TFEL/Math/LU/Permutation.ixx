/*!
 * \file   Permutation.ixx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   01 Aug 2006
 */

#ifndef _LIB_TFEL_PERMUTATION_I_
#define _LIB_TFEL_PERMUTATION_I_ 

#include<algorithm>

#include"TFEL/Math/MathException.hxx"

namespace tfel{

  namespace math{

    template<typename T>
    Permutation<T>::Permutation()
    {}

    template<typename T>
    Permutation<T>::Permutation(const typename vector<T>::size_type n)
      : vector<T>(n)
    {
      this->reset();
    }

    template<typename T>
    bool
    Permutation<T>::isIdentity(void) const
    {
      return this->is_identity;
    } // end of Permutation<T>::isIdendity

    template<typename T>
    void
    Permutation<T>::swap(const typename vector<T>::size_type i,
			 const typename vector<T>::size_type j)
    {
      std::swap(vector<T>::operator[](i),
		vector<T>::operator[](j));
      this->is_identity = false;
    }

    template<typename T>
    const T&
    Permutation<T>::operator[](const typename vector<T>::size_type i) const
    {
      return vector<T>::operator[](i);
    }

    template<typename T>
    const T&
    Permutation<T>::operator()(const typename vector<T>::size_type i) const
    {
      return vector<T>::operator[](i);
    }

    template<typename T>
    void
    Permutation<T>::reset()
    {
      typename vector<T>::size_type i;
      for(i=0;i!=this->size();++i){
	vector<T>::operator[](i) = i;
      }
      this->is_identity = true;
    }

    template<typename T>
    void
    Permutation<T>::resize(const typename vector<T>::size_type s)
    {
      vector<T>::resize(s);
      this->reset();
    }

  } // end of namespace math

} // end of namespace tfel


#endif /* _LIB_TFEL_PERMUTATION_I_ */

