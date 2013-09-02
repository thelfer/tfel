/*!
 * \file   fsarray.ixx
 * \brief  
 * \author Helfer Thomas
 * \date   30 Août 2013
 */

#ifndef _LIB_TFEL_TINY_FSARRAY_IXX_
#define _LIB_TFEL_TINY_FSARRAY_IXX_ 

namespace tfel{
  
  namespace math{

    template<unsigned short N, typename T>
    typename fsarray<N,T>::iterator 
    fsarray<N,T>::begin(void)
    {
      return this->v;
    }

    template<unsigned short N, typename T>
    typename fsarray<N,T>::const_iterator 
    fsarray<N,T>::begin(void) const
    {
      return this->v;
    }

    template<unsigned short N, typename T>
    typename fsarray<N,T>::iterator 
    fsarray<N,T>::end(void)
    {
      return this->v+N;
    }

    template<unsigned short N, typename T>
    typename fsarray<N,T>::const_iterator 
    fsarray<N,T>::end(void) const
    {
      return this->v+N;
    }

    template<unsigned short N, typename T>
    typename fsarray<N,T>::reverse_iterator 
    fsarray<N,T>::rbegin(void)
    {
      return reverse_iterator(this->v+N);
    }

    template<unsigned short N, typename T>
    typename fsarray<N,T>::const_reverse_iterator 
    fsarray<N,T>::rbegin(void) const
    {
      return const_reverse_iterator(this->v+N);
    }

    template<unsigned short N, typename T>
    typename fsarray<N,T>::reverse_iterator 
    fsarray<N,T>::rend(void)
    {
      return reverse_iterator(this->v);
    }

    template<unsigned short N, typename T>
    typename fsarray<N,T>::const_reverse_iterator 
    fsarray<N,T>::rend(void) const
    {
      return const_reverse_iterator(this->v);
    }

    template<unsigned short N, typename T>
    unsigned short
    fsarray<N,T>::size(void) const
    {
      return N;
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_TINY_FSARRAY_IXX */

