/*! 
 * \file  Kriging.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#include"TFEL/Math/Kriging/KrigingErrors.hxx"

namespace tfel
{

  namespace math
  {

    KrigingErrorInvalidLength::KrigingErrorInvalidLength()
      : std::runtime_error("number of locations is different from number of values")
    {} // end of KrigingErrorInvalidLength::KrigingErrorInvalidLength()

    KrigingErrorNoDataSpecified::KrigingErrorNoDataSpecified()
      : std::runtime_error("no data specified")
    {} // end of KrigingErrorNoDataSpecified::KrigingErrorNoDataSpecified()

    KrigingErrorInsufficientData::KrigingErrorInsufficientData()
      : std::runtime_error("insufficient data")
    {} // end of KrigingErrorInsufficientData::KrigingErrorInsufficientData()

  } // end of namespace math

} // end of namespace tfel

