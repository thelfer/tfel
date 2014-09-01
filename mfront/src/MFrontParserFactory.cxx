/*!
 * \file   mfront/src/MFrontParserFactory.cxx
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

#include<vector>
#include<string>
#include<utility>

#include"MFront/MFrontParserFactory.hxx"
#include"MFront/MFrontParserProxy.hxx"

namespace mfront{

  MFrontParserFactory&
  MFrontParserFactory::getMFrontParserFactory()
  {
    static MFrontParserFactory f;
    return f;
  }

  MFrontParserFactory::DescriptionPtrContainer&
  MFrontParserFactory::getDescriptionMap(void)
  {
    static DescriptionPtrContainer map;
    return map;
  } // end of MFrontParserFactory::getDescriptionMap

  MFrontParserFactory::ParserCreatorsContainer&
  MFrontParserFactory::getMap(void)
  {
    static ParserCreatorsContainer map;
    return map;
  } // end of MFrontParserFactory::getMap

  MFrontParserFactory::MFrontParserFactory()
  {} // end of MFrontParserFactory::MFrontParserFactory

  std::vector<std::string>
  MFrontParserFactory::getRegistredParsers(void)
  {
    using namespace std;
    ParserCreatorsContainer::const_iterator p;
    vector<string> res;
    for(p  = this->getMap().begin();
	p != this->getMap().end();++p){
      res.push_back(p->first);
    }
    return res;
  }

  void
  MFrontParserFactory::registerParserCreator(const std::string& parserName,
					     const MFrontParserFactory::ParserCreator f,
					     const MFrontParserFactory::DescriptionPtr f2)
  {
    using namespace std;
    if(!this->getMap().insert(make_pair(parserName,f)).second){
      string msg("MFrontParserFactory::registerParserCreator : ");
      msg += "a parser named "+parserName+" has already been registred";
      throw(runtime_error(msg));
    }
    if(!this->getDescriptionMap().insert(make_pair(parserName,f2)).second){
      string msg("MFrontParserFactory::registerParserCreator : ");
      msg += "a parser named "+parserName+" has already been registred";
      throw(runtime_error(msg));
    }
  } // end of MFrontParserFactory::registerParserCreator

  std::string
  MFrontParserFactory::getParserDescription(const std::string& parserName)
  {
    using namespace std;
    DescriptionPtrContainer::const_iterator p;
    p = this->getDescriptionMap().find(parserName);
    if(p==this->getDescriptionMap().end()){
      string msg = "MFrontParserFactory::getParserDescription : no parser named ";
      msg += parserName;
      throw(runtime_error(msg));
    }
    DescriptionPtr c = p->second;
    return c();
  } // end of 

  MFrontVirtualParser * 
  MFrontParserFactory::createNewParser(const std::string& parserName)
  {
    using namespace std;
    ParserCreatorsContainer::iterator p;
    p = this->getMap().find(parserName);
    if(p==this->getMap().end()){
      string msg = "MFrontParserFactory::createNewParser : no parser named '";
      msg += parserName+"'";
      throw(runtime_error(msg));
    }
    ParserCreator c = p->second;
    return c();
  }

  MFrontParserFactory::~MFrontParserFactory()
  {} // end of MFrontParserFactory::~MFrontParserFactory

} // end of namespace mfront
