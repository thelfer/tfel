/*!
 * \file   MFrontParserFactory.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifndef _LIB_MFRONTPARSERFACTORY_HXX_
#define _LIB_MFRONTPARSERFACTORY_HXX_ 

#include<vector>
#include<string>
#include<map>

#include"TFEL/Config/TFELConfig.hxx"

#include"MFront/MFrontVirtualParser.hxx"
#include"MFront/MFrontDefaultParser.hxx"

namespace mfront{

  struct TFEL_VISIBILITY_EXPORT MFrontParserFactory
  {
    typedef MFrontVirtualParser* (*ParserCreator)(void);
    typedef std::string (*DescriptionPtr)(void);
    
    static MFrontParserFactory&
    getMFrontParserFactory();

    std::vector<std::string>
    getRegistredParsers(void);

    void registerParserCreator(const std::string&,
			       ParserCreator,
			       DescriptionPtr);
    
    MFrontVirtualParser*
    createNewParser(const std::string& = MFrontDefaultParser::getName());

    std::string
    getParserDescription(const std::string&);
    
    ~MFrontParserFactory();

  private:

    typedef std::map<std::string,ParserCreator>  ParserCreatorsContainer;
    typedef std::map<std::string,DescriptionPtr> DescriptionPtrContainer;

    TFEL_VISIBILITY_LOCAL
    MFrontParserFactory();

    TFEL_VISIBILITY_LOCAL
    DescriptionPtrContainer&
    getDescriptionMap(void);

    TFEL_VISIBILITY_LOCAL
    ParserCreatorsContainer&
    getMap(void);

  }; // end of struct MFrontParserFactory

} // end of namespace mfront  

#endif /* _LIB_MFRONTPARSERFACTORY_HXX */

