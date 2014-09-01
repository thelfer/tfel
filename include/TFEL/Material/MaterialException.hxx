/*!
 * \file   include/TFEL/Material/MaterialException.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Aug 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

