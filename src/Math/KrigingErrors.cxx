/*! 
 * \file  Kriging.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
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

