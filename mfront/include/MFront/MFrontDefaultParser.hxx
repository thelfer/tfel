/*!
 * \file   mfront/include/MFront/MFrontDefaultParser.hxx
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
#include"MFront/MFrontDefaultParserBase.hxx"

namespace mfront{

  struct MFrontDefaultParser
    : public MFrontDefaultParserBase
  {

    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    MFrontDefaultParser();

    ~MFrontDefaultParser();

  }; // end of struct MFrontDefaultParser

} // end of namespace mfront  

#endif /* _LIB_MFRONTPARSER_HXX */

