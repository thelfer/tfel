/*!
 * \file   MFrontParserProxy.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifndef _LIB_MFRONTPARSERPROXY_IXX_
#define _LIB_MFRONTPARSERPROXY_IXX_ 

namespace mfront{

  template<typename ParserName>
  MFrontParserProxy<ParserName>::MFrontParserProxy()
  {
    parserFactory.registerParserCreator(ParserName::getName(),
					&MFrontParserProxy<ParserName>::createParser,
					&ParserName::getDescription);
  }

  template<typename ParserName>
  MFrontVirtualParser* 
  MFrontParserProxy<ParserName>::createParser()
  {
    return new ParserName;
  }

} // end of namespace mfront

#endif /* _LIB_MFRONTPARSERPROXY_IXX */

