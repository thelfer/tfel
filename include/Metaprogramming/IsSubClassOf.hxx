/*!
 * \file   IsSubClassOf.hxx
 * \brief  This file implements the IsSubClassOf metafunction.
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#ifndef _LIB_TFEL_ISSUBCLASSOF_H_
#define _LIB_TFEL_ISSUBCLASSOF_H_ 

#include"Metaprogramming/IsConvertibleTo.hxx"
#include"Metaprogramming/IsSuperClassOf.hxx"

namespace tfel{

  namespace meta{
    
    /*!
     * \brief  The metafunction IsSubClassOf test if a given 
     * class T publically inherits from type U.
     * This idea of this class was found in Andrei 
     * Alexandrescu's book
     * \latexonly\cite{alexandrescu01:_c_desig}\endlatexonly
     * \htmlonly, modern C++ Design : generic programming 
     * and design patterns applied\endhtmlonly.
     *
     * \param T, type to be tested.
     * \param U, we want to know if T publically inherits from U.
     * \return bool, true if T publically inherits to U, false otherwise.
     * \author Helfer Thomas
     * \date   28 Aug 2006
     */
    template<typename T, typename U>
    struct IsSubClassOf{
      /*!
       * \brief the result of the metafunction. 
       * True if T is publically inherits from U, false otherwise.
       * \see IsSuperClassOf
       */
      static const bool cond = tfel::meta::IsSuperClassOf<U,T>::cond;
    };

  } // end of namespace meta

} // end of namespace tfel


#endif /* _LIB_TFEL_ISSUBCLASSOF_H */

