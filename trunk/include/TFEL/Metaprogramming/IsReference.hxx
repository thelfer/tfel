/*! 
 * \file  IsReference.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 01 aoû 2010
 */

#ifndef   _LIB_TFEL_METAPROGRAMMMING_ISREFERENCE_H_
#define   _LIB_TFEL_METAPROGRAMMMING_ISREFERENCE_H_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{

    /*!
     * \brief  The class IsReference test if a given 
     * class A is reference
     *
     * \param A, type to be tested.
     * \return true if A is a reference
     *
     * \author Helfer Thomas
     * \date   01 Aug 2010
     */
    template <typename A>
    struct TFEL_VISIBILITY_LOCAL IsReference
    {
      static const bool cond = false;
    }; // end of struct IsReference

    /*!
     * Partial specialisation for references
     */
    template <typename A>
    struct TFEL_VISIBILITY_LOCAL IsReference<A&>
    {
      static const bool cond = true;
    }; // end of struct IsReference

    /*!
     * Partial specialisation for constant references
     */
    template <typename A>
    struct TFEL_VISIBILITY_LOCAL IsReference<const A&>
    {
      static const bool cond = true;
    }; // end of struct IsReference

  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_METAPROGRAMMMING_ISREFERENCE_H */

