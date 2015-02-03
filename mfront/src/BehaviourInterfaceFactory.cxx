/*!
 * \file   mfront/src/BehaviourInterfaceFactory.cxx
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

#include"MFront/BehaviourInterfaceFactory.hxx"

namespace mfront{

  BehaviourInterfaceFactory&
  BehaviourInterfaceFactory::getBehaviourInterfaceFactory()
  {
    static BehaviourInterfaceFactory f;
    return f;
  } // end of BehaviourInterfaceFactory::getBehaviourInterfaceFactory

  BehaviourInterfaceFactory::InterfaceCreatorsContainer&
  BehaviourInterfaceFactory::getInterfaceCreatorsMap(void)
  {
    static InterfaceCreatorsContainer map;
    return map;
  } // end of BehaviourInterfaceFactory::getInterfaceCreatorsMap

  BehaviourInterfaceFactory::InterfaceContainer&
  BehaviourInterfaceFactory::getInterfacesMap(void)
  {
    static InterfaceContainer map;
    return map;
  } // end of BehaviourInterfaceFactory::getInterfacesMap

  BehaviourInterfaceFactory::BehaviourInterfaceFactory()
  {}

  std::vector<std::string>
  BehaviourInterfaceFactory::getRegistredInterfaces(void)
  {
    using namespace std;
    InterfaceCreatorsContainer::const_iterator p;
    vector<string> res;
    for(p  = this->getInterfaceCreatorsMap().begin();
	p != this->getInterfaceCreatorsMap().end();++p){
      res.push_back(p->first);
    }
    return res;
  }

  void 
  BehaviourInterfaceFactory::registerInterfaceCreator(const std::string& i,
							    const BehaviourInterfaceFactory::InterfaceCreator f)
  {
    using namespace std;
    auto& imap = this->getInterfaceCreatorsMap();
    if(imap.find(i)!=imap.end()){
      string msg("BehaviourInterfaceFactory::registerInterfaceCreator : ");
      msg += "interface creator '"+i+"' already declared";
      throw(runtime_error(msg));
    }
    imap.insert(make_pair(i,f));
  }

  void
  BehaviourInterfaceFactory::registerInterfaceAlias(const std::string& i,
							  const std::string& a)
  {
    using namespace std;
    auto& amap = this->getAliasesMap();
    if(amap.find(a)!=amap.end()){
      string msg("BehaviourInterfaceFactory::registerInterfaceCreator : ");
      msg += "interface alias '"+a+"' already declared";
      throw(runtime_error(msg));
    }
    amap.insert(make_pair(a,i));
  }

  AbstractBehaviourInterface* 
  BehaviourInterfaceFactory::getInterfacePtr(const std::string& interfaceName)
  {
    using namespace std;
    InterfaceCreatorsContainer::iterator p;
    AliasContainer::iterator p2;
    InterfaceContainer::iterator m;
    AbstractBehaviourInterface *i;
    m = this->getInterfacesMap().find(interfaceName);
    if(m==this->getInterfacesMap().end()){
      p2 = this->getAliasesMap().find(interfaceName);
      if(p2==this->getAliasesMap().end()){
	string msg = "BehaviourInterfaceFactory::createNewInterface : no interface named '";
	msg += interfaceName+"'.\n";
	msg += "Available interface are : \n";
	for(p2  = this->getAliasesMap().begin();
	    p2 != this->getAliasesMap().end();++p2){
	  msg += p2->first + " ";
	}
	throw(runtime_error(msg));
      }
      p = this->getInterfaceCreatorsMap().find(p2->second);
      assert(p!=this->getInterfaceCreatorsMap().end());
      InterfaceCreator c = p->second;
      i = c();
      this->getInterfacesMap().insert(make_pair(interfaceName,i));
    } else {
      i = m->second;
    }
    return i;
  }

  void
  BehaviourInterfaceFactory::clear(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      delete m->second;
    }
    this->getInterfacesMap().clear();
  } // end of BehaviourInterfaceFactory::clear(void)

  BehaviourInterfaceFactory::~BehaviourInterfaceFactory()
  {
    assert(this->getInterfacesMap().empty());
  } // end of BehaviourInterfaceFactory::~BehaviourInterfaceFactory()
  
  void
  BehaviourInterfaceFactory::reset(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      m->second->reset();
    }
  } // end of BehaviourInterfaceFactory::reset

  BehaviourInterfaceFactory::AliasContainer&
  BehaviourInterfaceFactory::getAliasesMap(void)
  {
    static AliasContainer map;
    return map;
  } // end of BehaviourInterfaceFactory::getAliasesMap

} // end of namespace mfront
