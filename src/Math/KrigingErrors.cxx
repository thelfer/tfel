/*! 
 * \file  src/Math/KrigingErrors.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<string>
#include"TFEL/Math/Kriging/KrigingErrors.hxx"

namespace tfel
{

  namespace math
  {

    KrigingErrorInvalidLength::KrigingErrorInvalidLength()
      : std::runtime_error(std::string("number of locations is different from number of values"))
    {} // end of KrigingErrorInvalidLength::KrigingErrorInvalidLength()

    KrigingErrorNoDataSpecified::KrigingErrorNoDataSpecified()
      : std::runtime_error(std::string("no data specified"))
    {} // end of KrigingErrorNoDataSpecified::KrigingErrorNoDataSpecified()
    
    KrigingErrorInsufficientData::KrigingErrorInsufficientData()
      : std::runtime_error(std::string("insufficient data"))
    {} // end of KrigingErrorInsufficientData::KrigingErrorInsufficientData()

  } // end of namespace math

} // end of namespace tfel

