/*!
 * \file   fill.hxx
 * \brief  This file implements the fill class.
 * \author Helfer Thomas
 * \date   30 Jun 2006
 */

#ifndef _LIB_TFEL_FILL_HXX_
#define _LIB_TFEL_FILL_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace fsalgo{

    /*!
     * \class fill
     * \brief Fill assigns the value value to every element in the range [first, last). That is, for every iterator i in [first, first+N), it performs the assignment *i = value.
     * \param N number of element to be filled.  
     *
     * This documentation is mostly inspired from:
     * http://www.sgi.com/tech/stl/fill.html
     * 
     * \author Helfer Thomas
     * \date   30 Jun 2006      
     */
    template<unsigned int N>
    struct fill
    {
      /*
       * \param  OutputIterator iterator to the elements being filled. 
       * \param  T type of the value.
       * \return void
       *
       * \pre
       * - ForwardIterator is a model of Forward Iterator.
       * - ForwardIterator is mutable.
       * - T is a model of Assignable.
       * - T is convertible to Forward Iterator's value type.
       */
      template<typename OutputIterator,typename T>
      static TFEL_FSALGORITHM_INLINE
      void exe(OutputIterator p, T q)
      {
	*p = q;
	fill<N-1>::exe(++p,q);
      }
    };  

    /*!
     * \brief partial specialisation of struct fill to end recursion.
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006      
     */
    template<>
    struct fill<0u>
    {
      /*!
       * \return void
       * \sa fill<N>::exe() for details
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template<typename OutputIterator,typename T>
      static TFEL_FSALGORITHM_INLINE
      void exe(OutputIterator, T)
      {}
    };
  
  } // end of namespace fsalgo

} // end of namespace tfel

#endif /* _LIB_TFEL_FILL_HXX */

