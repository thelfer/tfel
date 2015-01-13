/*! 
 * \file  src/Math/LUException.cxx
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
    
    LUException::~LUException() throw()
    {}

  } // end of namespace math

} // end of namespace tfel
