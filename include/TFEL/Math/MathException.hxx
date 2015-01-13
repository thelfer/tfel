/*!
 * \file   include/TFEL/Math/MathException.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATHEXCEPTION_H_
#define _LIB_TFEL_MATHEXCEPTION_H_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Exception/TFELException.hxx"

namespace tfel{

  namespace math{

    struct TFELMATH_VISIBILITY_EXPORT MathException
      : public tfel::exception::TFELException 
    {      
      MathException(const std::string& s);
      virtual ~MathException() throw();
    }; // end of struct MathException

    struct TFELMATH_VISIBILITY_EXPORT MathRunTimeException
      : public MathException
    {
      MathRunTimeException(const std::string& s);
      virtual ~MathRunTimeException() throw();
    }; // end of struct MathRunTimeException

    struct TFELMATH_VISIBILITY_EXPORT MathDivergenceException
      : public MathException
    {
      MathDivergenceException(const std::string& s);
      virtual ~MathDivergenceException() throw();
    }; // end of struct MathDivergenceException

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATHEXCEPTION_H */

