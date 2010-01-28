/*!
 * \file   IsSameType.hxx
 * \brief  This file implements the IsSameType metafunction.
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#ifndef _LIB_TFEL_ISSAMETYPE_H_
#define _LIB_TFEL_ISSAMETYPE_H_ 

namespace tfel{

  namespace meta{

     /*!
      * \brief  The class IsConvertibleTo test if a given 
      * class A is the same as the class B.
      *
      * \param A, type to be tested.
      * \param B, we want to know if A is the same as B.
      * \return bool, true if A is the same as B, false otherwise.
      *
      * \author Helfer Thomas
      * \date   28 Aug 2006
      */
    template <typename A, typename B>
    struct IsSameType{
      /*
       * \brief The result of the metafunction.
       * This is false in the general case.
       */
      static const bool cond = false;
    };

    /*! 
     * Partial specialisation when the two parameters are the same.
     */
    template <typename A>
    struct IsSameType<A,A>{
      /*
       * \brief The result of the metafunction.
       * Here, it is set to true.
       */      
      static const bool cond = true;
    };

  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_ISSAMETYPE_H */

