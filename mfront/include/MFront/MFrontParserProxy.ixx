/*!
 * \file   mfront/include/MFront/MFrontParserProxy.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTPARSERPROXY_IXX_
#define _LIB_MFRONTPARSERPROXY_IXX_ 

namespace mfront{

  template<typename ParserName>
  MFrontParserProxy<ParserName>::MFrontParserProxy()
  {
    MFrontParserFactory& parserFactory = MFrontParserFactory::getMFrontParserFactory();
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

