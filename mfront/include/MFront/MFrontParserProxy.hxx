/*!
 * \file   mfront/include/MFront/MFrontParserProxy.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTPARSERPROXY_HXX_
#define _LIB_MFRONTPARSERPROXY_HXX_ 

#include<string>

#include"MFront/MFrontParserFactory.hxx"
#include"MFront/MFrontVirtualParser.hxx"

namespace mfront{

  template<typename ParserName>
  struct MFrontParserProxy
  {
    MFrontParserProxy();

    static MFrontVirtualParser* createParser();
  };

} // end of namespace mfront

#include"MFront/MFrontParserProxy.ixx"

#endif /* _LIB_MFRONTPARSERPROXY_HXX */

