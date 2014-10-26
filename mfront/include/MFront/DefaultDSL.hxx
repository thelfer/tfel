/*!
 * \file   mfront/include/MFront/DefaultDSL.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTPARSER_HXX_
#define _LIB_MFRONTPARSER_HXX_ 

#include<string>
#include"MFront/DefaultDSLBase.hxx"

namespace mfront{

  struct DefaultDSL
    : public DefaultDSLBase
  {

    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    DefaultDSL();

    ~DefaultDSL();

  }; // end of struct DefaultDSL

} // end of namespace mfront  

#endif /* _LIB_MFRONTPARSER_HXX */

