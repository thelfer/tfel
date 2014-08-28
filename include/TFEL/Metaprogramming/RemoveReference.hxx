/*!
 * \file   include/TFEL/Metaprogramming/RemoveReference.hxx
 * \brief  This file declares the RemoveReference metafunction
 * \author Helfer Thomas
 * \date   18 Oct 2006
 */

#ifndef _LIB_TFEL_REMOVEREFERENCE_H_
#define _LIB_TFEL_REMOVEREFERENCE_H_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{

    /*!
     * \class  RemoveReference
     * \brief  return a the same type without the 
     * reference qualifier.
     * \param  typename T. 
     *
     * \author Helfer Thomas
     * \date   01 Aout 2010
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL RemoveReference
    {
      //! the result
      typedef T type;
    };
    
    /*!
     * Partial specialisation for T&
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL RemoveReference<T&>
    {
      //! the result
      typedef T type;
    };
    
  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_REMOVEREFERENCE_H */

