/*! 
 * \file  LUException.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avr 2009
 */

#ifndef _LIB_TFEL_MATH_LUEXCEPTION_H_
#define _LIB_TFEL_MATH_LUEXCEPTION_H_ 

#include"TFEL/Exception/TFELException.hxx"

#include<string>
 
namespace tfel
{

  namespace math
  {
    
    struct LUException
      : public tfel::exception::TFELException
    {
      LUException(const char* const);
      LUException(const std::string&);
    }; // end of struct LUException
    
  } // end of namespace math

} // end of namespace tfelx

#endif /* _LIB_TFEL_MATH_LUEXCEPTION_H */

