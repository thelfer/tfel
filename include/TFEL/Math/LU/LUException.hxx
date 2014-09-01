/*! 
 * \file  include/TFEL/Math/LU/LUException.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avr 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_LUEXCEPTION_H_
#define _LIB_TFEL_MATH_LUEXCEPTION_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Exception/TFELException.hxx"

#include<string>
 
namespace tfel
{

  namespace math
  {
    
    struct TFELMATH_VISIBILITY_EXPORT LUException
      : public tfel::exception::TFELException
    {
      LUException(const char* const);
      LUException(const std::string&);
    }; // end of struct LUException
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_LUEXCEPTION_H */

