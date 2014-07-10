/*!
 * \file   MathException.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
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
    }; // end of struct MathException

    struct TFELMATH_VISIBILITY_EXPORT MathRunTimeException
      : public MathException
    {
      MathRunTimeException(const std::string& s);
    }; // end of struct MathRunTimeException

    struct TFELMATH_VISIBILITY_EXPORT MathDivergenceException
      : public MathException
    {
      MathDivergenceException(const std::string& s);
    }; // end of struct MathDivergenceException

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATHEXCEPTION_H */

