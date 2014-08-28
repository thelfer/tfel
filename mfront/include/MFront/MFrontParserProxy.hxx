/*!
 * \file   mfront/include/MFront/MFrontParserProxy.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
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

