/*!
 * \file   Implements.hxx
 * \brief  This file describes the Implements metafunction  
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#ifndef _LIB_TFEL_IMPLEMENTS_H_
#define _LIB_TFEL_IMPLEMENTS_H_ 

#include"Metaprogramming/IsSuperClassOf.hxx"

namespace tfel{

  namespace meta{
    
    /*!
     * \class Implements
     *
     * \param T, class to be tested.
     * \param concept, concept template class. 
     * \return bool.
     *
     * \see concepts_sec for more details about how concepts are modelled in TFEL.
     *
     * \brief the Implements metafunction returns true if the 
     * class T is a subclass of concept<T>.
     * \author Helfer Thomas
     * \date   28 Aug 2006
     */
    template<typename T, template<typename> class concept>
    struct Implements
    {
      /*
       * The result of the metafunction.
       */
      static const bool cond = tfel::meta::IsSuperClassOf<concept<T>,T>::cond;
    };

  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_IMPLEMENTS_H */

