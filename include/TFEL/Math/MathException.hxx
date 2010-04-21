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

    struct TFEL_VISIBILITY_EXPORT MathException
      : public tfel::exception::TFELException 
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void);
      
      MathException(const std::string& s);
    }; // end of struct MathException

    struct TFEL_VISIBILITY_EXPORT MathRunTimeException
      : public MathException
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string 
      getName(void);
      
      MathRunTimeException(const std::string& s);
    }; // end of struct MathRunTimeException

    struct TFEL_VISIBILITY_EXPORT MathDivergenceException
      : public MathException
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string 
      getName(void);
      
      MathDivergenceException(const std::string& s);
    }; // end of struct MathDivergenceException

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATHEXCEPTION_H */

