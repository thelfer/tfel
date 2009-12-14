/*!
 * \file   floating_point_exceptions.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 Jul 2006
 */

#ifndef _LIB_TFEL_FLOATING_POINT_EXCEPTIONS_HXX_
#define _LIB_TFEL_FLOATING_POINT_EXCEPTIONS_HXX_ 

#ifdef HAVE_FENV
#include "fenv.h"
#endif /* HAVE_FENV */

namespace tfel{
  
  namespace math{

    /*!
     * this function enables C99 floating point rounding and exception handling:
     * - FE_DIVBYZERO: the DivideByZero exception occurs when an operation on finite numbers produces infinity as exact answer.
     * - FE_UNDERFLOW: the  Overflow  exception  occurs  when  a result has to be represented as a floating point number, but has (much) larger absolute value than the largest (finite) floating point number that is representable.
     * - FE_OVERFLOW: the Underflow exception occurs when a result has to be represented as a floating point number,  but  has  smaller absolute  value  than  the  smallest positive normalized floating point number (and would lose much accuracy when represented as a denormalized number).
     * - FE_INEXACT: the Inexact exception occurs when the rounded result of an operation is  not  equal  to  the  infinite  precision result.  It may occur whenever Overflow or Underflow occurs.
     */
    void init_floating_point_exceptions(void)
    {
#ifdef HAVE_FENV
      feenableexcept(FE_DIVBYZERO); //	division by zero
      feenableexcept(FE_UNDERFLOW); //	result not representable due to underflow
      feenableexcept(FE_OVERFLOW);  //	result not representable due to overflow
      feenableexcept(FE_INVALID);   //	invalid operation
      fedisableexcept(FE_INEXACT);  // The Inexact exception occurs when the  rounded  result  of  an operation  is  not equal to the infinite precision result.
#endif /* HAVE_FENV */
    }    
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_FLOATING_POINT_EXCEPTIONS_HXX */

