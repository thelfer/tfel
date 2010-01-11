/*!
 * \file   MaterialLawException.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Aug 2006
 */

#ifndef _LIB_TFEL_MATERIALLAWEXCEPTION_H_
#define _LIB_TFEL_MATERIALLAWEXCEPTION_H_ 

#include<string>

#include"Exception/TFELException.hxx"

namespace tfel{
  
  namespace materiallaw {
    
    struct MaterialLawException
      : public tfel::exception::TFELException 
    {
      static const std::string 
      getName(void);
      
      MaterialLawException(const std::string& s);
    };

    struct DivergenceException
      : public tfel::materiallaw::MaterialLawException
    {
      static const std::string 
      getName(void);
      
      DivergenceException(const std::string& s);
    };

    struct OutOfBoundsException
      : public tfel::materiallaw::MaterialLawException
    {
      static const std::string 
      getName(void);
      
      OutOfBoundsException(const std::string& s);
    };

    
  } // end of namespace materiallaw

} // end of namespace tfel


#endif /* _LIB_TFEL_MATERIALLAWEXCEPTION_H */

