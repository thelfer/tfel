/*!
 * \file   MathException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2007
 */

#include"TFEL/Math/MathException.hxx"

namespace tfel{

  namespace math{

    MathException::MathException(const std::string& s)
      :  tfel::exception::TFELException(s)
    {}

    MathRunTimeException::MathRunTimeException(const std::string& s)
      :  MathException(s)
    {}

    MathDivergenceException::MathDivergenceException(const std::string& s)
      :  MathException(s)
    {}

  } // end of namespace math

} // end of namespace tfel

