/*!
 * \file   IsSuperClassOf.hxx
 * \brief  This file implements the IsSuperClassOf metafunction.
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#ifndef _LIB_TFEL_ISSUPERCLASSOF_H_
#define _LIB_TFEL_ISSUPERCLASSOF_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/IsSameType.hxx"
#include"TFEL/Metaprogramming/IsConvertibleTo.hxx"

namespace tfel{

  namespace meta{

    /*!
     * \brief  The metafunction IsSubClassOf test if a given class U 
     * publically inherits from type T.
     * This idea of this class was found in Andrei Alexandrescu's book
     * \latexonly\cite{alexandrescu01:_c_desig}\endlatexonly
     * \htmlonly, modern C++ Design : generic programming and 
     * design patterns applied\endhtmlonly.
     *
     * \param T, type to be tested.
     * \param U, we want to know if U publically inherits from T.
     * \return bool, true if U publically inherits to T, false otherwise.
     * \author Helfer Thomas
     * \date   28 Aug 2006
     */
    template<typename T, typename U>
    struct TFEL_VISIBILITY_LOCAL IsSuperClassOf{
      /*!
       * \brief the result of the metafunction. 
       * True if U inherits from T, false otherwise.
       */
      static const bool cond = tfel::meta::IsConvertibleTo<const U*,const T*>::cond&&
      (!tfel::meta::IsSameType<const T*, const void*>::cond);
    };

  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_ISSUPERCLASSOF_H */

