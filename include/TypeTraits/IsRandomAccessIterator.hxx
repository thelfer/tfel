/*!
 * \file   IsRandomaAccessIterator.hxx
 * \brief  This file declares the IsRandomAccessIterator class.
 * \author Helfer Thomas
 * \date   15 aoû 2006
 */

#ifndef _LIB_TFEL_ISRANDOMAACCESSITERATOR_HXX_
#define _LIB_TFEL_ISRANDOMAACCESSITERATOR_HXX_ 

#include<iterator>

#include<Metaprogramming/IsSameType.hxx>

namespace tfel{

  namespace typetraits{

    /*!
     * \brief  Traits class which says if its argument is a random access iterator
     *
     * \param  T, type to be tested.
     * \return bool cond, true if T stands for a random access iterator, false otherwise.
     *
     * \author Helfer Thomas
     * \date   04 Jul 2006
     */
    template<typename T>
    struct IsRandomAccessIterator
    {
      /*!
       *  Result
       */
      static const bool cond = tfel::meta::IsSameType<typename std::iterator_traits<T>::iterator_category,std::random_access_iterator_tag>::cond;
    };

  } // end of namespace typetraits

} // end of namespace typetraits

#endif /* _LIB_TFEL_ISRANDOMAACCESSITERATOR_HXX */

