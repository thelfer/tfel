/*!
 * \file   MFrontParserProxy.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifndef _LIB_MFRONTPARSERPROXY_HXX_
#define _LIB_MFRONTPARSERPROXY_HXX_ 

#include<string>

#include"MFrontParserFactory.hxx"
#include"MFrontVirtualParser.hxx"

namespace mfront{

  template<typename ParserName>
  struct MFrontParserProxy
  {
    MFrontParserProxy();

    static MFrontVirtualParser* createParser();
  };

} // end of namespace mfront

#include"MFrontParserProxy.ixx"

#endif /* _LIB_MFRONTPARSERPROXY_HXX */

