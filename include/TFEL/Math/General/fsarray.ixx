/*!
 * \file   include/TFEL/Math/General/fsarray.ixx
 * \brief  
 * \author Helfer Thomas
 * \date   30 Août 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

