/*!
 * \file   generate.hxx
 * \brief  This file implements the generate class.
 * \author Helfer Thomas
 * \date   30 jun 2006
 */

#ifndef _LIB_TFEL_GENERATE_HXX_
#define _LIB_TFEL_GENERATE_HXX_ 

#include"Config/TFELConfig.hxx"

namespace tfel{

  namespace fsalgo{

    /*!
     * \class generate 
     * \brief generate assigns the result of invoking gen, a function object that takes no arguments, to each element in the range [first, last). 
     * \param N number of element to be which generate is applied.  
     *
     * This documentation is mostly inspired from:
     * http://www.sgi.com/tech/stl/generate.html
     *
     * \see generate.cxx for some elementary tests.
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006      
     */
    template<unsigned int N>
    struct generate
    {
      /*! 
       * \param ForwardIterator iterator to the elements
       * \param Generator generator type used
       *
       * \return void
       *
       * \pre
       * - ForwardIterator is a model of Forward Iterator. [2]
       * - ForwardIterator is mutable.
       * - Generator is a model of Generator.
       * - Generator's result type is convertible to ForwardIterator's value type.
       */
      template<typename ForwardIterator,typename Generator>
      static TFEL_FSALGORITHM_INLINE
      void exe(ForwardIterator p, Generator gen)
      {
	*p = gen();
	generate<N-1>::exe(++p,gen);
      }

    };  

    /*!
     * \brief partial specialisation used to end recursion when using the generate algorithm.
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006      
     */
    template<>
    struct generate<0u>
    {
      /*!
       * \return void
       * \sa generate<N>::exe() for details
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template<typename ForwardIterator,typename Generator>
      static TFEL_FSALGORITHM_INLINE
      void exe(ForwardIterator, Generator)
      {}
    };
  
  } // end of namespace fsalgo

} // end of namespace tfel
  

#endif /* _LIB_TFEL_GENERATE_HXX */

