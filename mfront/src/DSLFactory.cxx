/*!
 * \file   mfront/src/DSLFactory.cxx
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
#include<stdexcept>

#include"MFront/DSLFactory.hxx"
#include"MFront/DSLProxy.hxx"

namespace mfront{

  DSLFactory&
  DSLFactory::getDSLFactory()
  {
    static DSLFactory f;
    return f;
  }

  DSLFactory::DescriptionPtrContainer&
  DSLFactory::getDescriptionMap()
  {
    static DescriptionPtrContainer map;
    return map;
  } // end of DSLFactory::getDescriptionMap

  DSLFactory::ParserCreatorsContainer&
  DSLFactory::getMap()
  {
    static ParserCreatorsContainer map;
    return map;
  } // end of DSLFactory::getMap

  DSLFactory::DSLFactory()
  {} // end of DSLFactory::DSLFactory

  std::vector<std::string>
  DSLFactory::getRegistredParsers()
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
  DSLFactory::registerParserCreator(const std::string& parserName,
				    const DSLFactory::ParserCreator f,
				    const DSLFactory::DescriptionPtr f2)
  {
    using namespace std;
    if(!this->getMap().insert(make_pair(parserName,f)).second){
      string msg("DSLFactory::registerParserCreator : ");
      msg += "a parser named "+parserName+" has already been registred";
      throw(runtime_error(msg));
    }
    if(!this->getDescriptionMap().insert(make_pair(parserName,f2)).second){
      string msg("DSLFactory::registerParserCreator : ");
      msg += "a parser named "+parserName+" has already been registred";
      throw(runtime_error(msg));
    }
  } // end of DSLFactory::registerParserCreator

  std::string
  DSLFactory::getParserDescription(const std::string& n)
  {
    auto p = this->getDescriptionMap().find(n);
    if(p==this->getDescriptionMap().end()){
      throw(std::runtime_error("DSLFactory::getParserDescription: "
			       "no parser named '"+n+"'"));
    }
    auto c = p->second;
    return c();
  } // end of 

  std::shared_ptr<AbstractDSL>
  DSLFactory::createNewParser(const std::string& n)
  {
    return this->createNewDSL(n);
  }
    
  std::shared_ptr<AbstractDSL>
  DSLFactory::createNewDSL(const std::string& n)
  {
    auto p = this->getMap().find(n);
    if(p==this->getMap().end()){
      throw(std::runtime_error("DSLFactory::createNewDSL: "
			       "no parser named '"+n+"'"));
    }
    auto c = p->second;
    return c();
  }

  DSLFactory::~DSLFactory()
  {} // end of DSLFactory::~DSLFactory

} // end of namespace mfront
