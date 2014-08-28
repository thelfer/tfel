/*!
 * \file   include/TFEL/Material/MaterialException.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Aug 2006
 */

#ifndef _LIB_TFEL_MATERIALEXCEPTION_H_
#define _LIB_TFEL_MATERIALEXCEPTION_H_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Exception/TFELException.hxx"

namespace tfel{
  
  namespace material {
    
    struct TFELMATERIAL_VISIBILITY_EXPORT MaterialException
      : public tfel::exception::TFELException 
    {
      static std::string 
      getName(void);
      
      MaterialException(const std::string& s);
    };

    struct TFELMATERIAL_VISIBILITY_EXPORT DivergenceException
      : public tfel::material::MaterialException
    {
      static std::string 
      getName(void);
      
      DivergenceException(const std::string& s);
    };
    
    struct TFELMATERIAL_VISIBILITY_EXPORT OutOfBoundsException
      : public tfel::material::MaterialException
    {
      static std::string 
      getName(void);
      
      OutOfBoundsException(const std::string& s);
    };
    
  } // end of namespace material

} // end of namespace tfel


#endif /* _LIB_TFEL_MATERIALEXCEPTION_H */

