/*!
 * \file   src/Math/MathException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

