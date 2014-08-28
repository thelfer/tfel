/*! 
 * \file  include/TFEL/Metaprogramming/IsPointer.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 01 aoû 2010
 */

#ifndef   _LIB_TFEL_METAPROGRAMMMING_ISPOINTER_H_
#define   _LIB_TFEL_METAPROGRAMMMING_ISPOINTER_H_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{

    /*!
     * \brief  The class IsPointer test if a given 
     * class A is pointer
     *
     * \param A, type to be tested.
     * \return true if A is a pointer
     *
     * \author Helfer Thomas
     * \date   01 Aug 2010
     */
    template <typename A>
    struct TFEL_VISIBILITY_LOCAL IsPointer
    {
      static const bool cond = false;
    }; // end of struct IsPointer

    /*!
     * Partial specialisation for pointers
     */
    template <typename A>
    struct TFEL_VISIBILITY_LOCAL IsPointer<A*>
    {
      static const bool cond = true;
    }; // end of struct IsPointer

    /*!
     * Partial specialisation for pointers
     */
    template <typename A>
    struct TFEL_VISIBILITY_LOCAL IsPointer<const A*>
    {
      static const bool cond = true;
    }; // end of struct IsPointer

    /*!
     * Partial specialisation for constant pointers
     */
    template <typename A>
    struct TFEL_VISIBILITY_LOCAL IsPointer<A* const>
    {
      static const bool cond = true;
    }; // end of struct IsPointer

    /*!
     * Partial specialisation for constant pointers
     */
    template <typename A>
    struct TFEL_VISIBILITY_LOCAL IsPointer<const A* const>
    {
      static const bool cond = true;
    }; // end of struct IsPointer

  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_METAPROGRAMMMING_ISPOINTER_H */

