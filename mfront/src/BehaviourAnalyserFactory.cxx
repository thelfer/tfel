/*!
 * \file   mfront/src/BehaviourAnalyserFactory.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>
#include<cassert>

#include"MFront/BehaviourAnalyserFactory.hxx"

namespace mfront{

  BehaviourAnalyserFactory&
  BehaviourAnalyserFactory::getBehaviourAnalyserFactory()
  {
    static BehaviourAnalyserFactory f;
    return f;
  } // end of BehaviourAnalyserFactory::getBehaviourAnalyserFactory

  BehaviourAnalyserFactory::AnalyserCreatorsContainer&
  BehaviourAnalyserFactory::getAnalyserCreatorsMap(void)
  {
    static AnalyserCreatorsContainer map;
    return map;
  } // end of BehaviourAnalyserFactory::getAnalyserCreatorsMap

  BehaviourAnalyserFactory::AnalyserContainer&
  BehaviourAnalyserFactory::getAnalysersMap(void)
  {
    static AnalyserContainer map;
    return map;
  } // end of BehaviourAnalyserFactory::getAnalysersMap

  BehaviourAnalyserFactory::BehaviourAnalyserFactory()
  {}

  std::vector<std::string>
  BehaviourAnalyserFactory::getRegistredAnalysers(void)
  {
    using namespace std;
    AnalyserCreatorsContainer::const_iterator p;
    vector<string> res;
    for(p  = this->getAnalyserCreatorsMap().begin();
	p != this->getAnalyserCreatorsMap().end();++p){
      res.push_back(p->first);
    }
    return res;
  }

  void 
  BehaviourAnalyserFactory::registerAnalyserCreator(const std::string& i,
							  const BehaviourAnalyserFactory::AnalyserCreator f)
  {
    using namespace std;
    auto& imap = this->getAnalyserCreatorsMap();
    if(imap.find(i)!=imap.end()){
      string msg("BehaviourAnalyserFactory::registerAnalyserCreator : ");
      msg += "analyser creator '"+i+"' already declared";
      throw(runtime_error(msg));
    }
    imap.insert(make_pair(i,f));
  }

  void
  BehaviourAnalyserFactory::registerAnalyserAlias(const std::string& i,
							 const std::string& a)
  {
    using namespace std;
    auto& amap = this->getAliasesMap();
    if(amap.find(a)!=amap.end()){
      string msg("BehaviourAnalyserFactory::registerAnalyserCreator : ");
      msg += "interface alias '"+a+"' already declared";
      throw(runtime_error(msg));
    }
    amap.insert(make_pair(a,i));
  }

  BehaviourAnalyser* 
  BehaviourAnalyserFactory::getAnalyserPtr(const std::string& analyserName)
  {
    using namespace std;
    AnalyserCreatorsContainer::iterator p;
    AliasContainer::iterator p2;
    AnalyserContainer::iterator m;
    BehaviourAnalyser *i;
    m = this->getAnalysersMap().find(analyserName);
    if(m==this->getAnalysersMap().end()){
      p2 = this->getAliasesMap().find(analyserName);
      if(p2==this->getAliasesMap().end()){
	string msg = "BehaviourAnalyserFactory::createNewAnalyser : no analyser named '";
	msg += analyserName+"'.\n";
	msg += "Available analyser are : \n";
	for(p2  = this->getAliasesMap().begin();
	    p2 != this->getAliasesMap().end();++p2){
	  msg += p2->first + " ";
	}
	throw(runtime_error(msg));
      }
      p = this->getAnalyserCreatorsMap().find(p2->second);
      assert(p!=this->getAnalyserCreatorsMap().end());
      AnalyserCreator c = p->second;
      i = c();
      this->getAnalysersMap().insert(make_pair(analyserName,i));
    } else {
      i = m->second;
    }
    return i;
  }

  BehaviourAnalyserFactory::~BehaviourAnalyserFactory()
  {
    assert(this->getAnalysersMap().empty());
  } // end of BehaviourAnalyserFactory::~BehaviourAnalyserFactory()
  
  void
  BehaviourAnalyserFactory::reset(void)
  {
    for(auto & a : this->getAnalysersMap()){
      a.second->reset();
    }
  } // end of BehaviourAnalyserFactory::reset

  BehaviourAnalyserFactory::AliasContainer&
  BehaviourAnalyserFactory::getAliasesMap(void)
  {
    static AliasContainer map;
    return map;
  } // end of BehaviourAnalyserFactory::getAliasesMap

} // end of namespace mfront
