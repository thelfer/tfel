/*! 
 * \file  LUException.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avr 2009
 */

#include"TFEL/Math/LU/LUException.hxx"

namespace tfel
{

  namespace math
  {
    
    LUException::LUException(const std::string& s)
      : tfel::exception::TFELException(s)
    {} // end of LUException::LUException

    LUException::LUException(const char* const s)
      : tfel::exception::TFELException(s)
    {} // end of LUException::LUException
    
  } // end of namespace math

} // end of namespace tfel
