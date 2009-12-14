/*!
 * \file   IsUnaryOperator.hxx
 * \brief  This file declares the IsUnaryOperator traits class.
 * \author Helfer Thomas
 * \date   07 Jun 2006
 */

#ifndef _LIB_TFEL_ISOPERATOR_H_
#define _LIB_TFEL_ISOPERATOR_H_ 

namespace tfel{

  namespace typetraits{

    /*!
     * \brief  Traits class which says if its argument stands for a unary operator
     *
     * \param  T, type to be tested.
     * \return bool cond, true if T stands for a unary operator, false otherwise.
     *
     * \author Helfer Thomas
     * \date   08/01/07
     */
    template<typename T>
    struct IsUnaryOperator
    {
      /*!
       *  Result
       */
      static const bool cond = false;
    };

  } // end of namespace typetraits

} // end of namespace tfel

#endif /* _LIB_TFEL_ISOPERATOR_H */

