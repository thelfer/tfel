/*!
 * \file   mfront/src/MFrontBehaviourInterfaceFactory.cxx
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

#include"MFront/MFrontBehaviourInterfaceFactory.hxx"

namespace mfront{

  MFrontBehaviourInterfaceFactory&
  MFrontBehaviourInterfaceFactory::getMFrontBehaviourInterfaceFactory()
  {
    static MFrontBehaviourInterfaceFactory f;
    return f;
  } // end of MFrontBehaviourInterfaceFactory::getMFrontBehaviourInterfaceFactory

  MFrontBehaviourInterfaceFactory::InterfaceCreatorsContainer&
  MFrontBehaviourInterfaceFactory::getInterfaceCreatorsMap(void)
  {
    static InterfaceCreatorsContainer map;
    return map;
  } // end of MFrontBehaviourInterfaceFactory::getInterfaceCreatorsMap

  MFrontBehaviourInterfaceFactory::InterfaceContainer&
  MFrontBehaviourInterfaceFactory::getInterfacesMap(void)
  {
    static InterfaceContainer map;
    return map;
  } // end of MFrontBehaviourInterfaceFactory::getInterfacesMap

  MFrontBehaviourInterfaceFactory::MFrontBehaviourInterfaceFactory()
  {}

  std::vector<std::string>
  MFrontBehaviourInterfaceFactory::getRegistredInterfaces(void)
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
  MFrontBehaviourInterfaceFactory::registerInterfaceCreator(const std::string& i,
							    const MFrontBehaviourInterfaceFactory::InterfaceCreator f)
  {
    using namespace std;
    InterfaceCreatorsContainer& imap = this->getInterfaceCreatorsMap();
    if(imap.find(i)!=imap.end()){
      string msg("MFrontBehaviourInterfaceFactory::registerInterfaceCreator : ");
      msg += "interface creator '"+i+"' already declared";
      throw(runtime_error(msg));
    }
    imap.insert(make_pair(i,f));
  }

  void
  MFrontBehaviourInterfaceFactory::registerInterfaceAlias(const std::string& i,
							  const std::string& a)
  {
    using namespace std;
    AliasContainer& amap = this->getAliasesMap();
    if(amap.find(a)!=amap.end()){
      string msg("MFrontBehaviourInterfaceFactory::registerInterfaceCreator : ");
      msg += "interface alias '"+a+"' already declared";
      throw(runtime_error(msg));
    }
    amap.insert(make_pair(a,i));
  }

  MFrontBehaviourVirtualInterface* 
  MFrontBehaviourInterfaceFactory::getInterfacePtr(const std::string& interfaceName)
  {
    using namespace std;
    InterfaceCreatorsContainer::iterator p;
    AliasContainer::iterator p2;
    InterfaceContainer::iterator m;
    MFrontBehaviourVirtualInterface *i;
    m = this->getInterfacesMap().find(interfaceName);
    if(m==this->getInterfacesMap().end()){
      p2 = this->getAliasesMap().find(interfaceName);
      if(p2==this->getAliasesMap().end()){
	string msg = "MFrontBehaviourInterfaceFactory::createNewInterface : no interface named '";
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
  MFrontBehaviourInterfaceFactory::clear(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      delete m->second;
    }
    this->getInterfacesMap().clear();
  } // end of MFrontBehaviourInterfaceFactory::clear(void)

  MFrontBehaviourInterfaceFactory::~MFrontBehaviourInterfaceFactory()
  {
    assert(this->getInterfacesMap().empty());
  } // end of MFrontBehaviourInterfaceFactory::~MFrontBehaviourInterfaceFactory()
  
  void
  MFrontBehaviourInterfaceFactory::reset(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      m->second->reset();
    }
  } // end of MFrontBehaviourInterfaceFactory::reset

  MFrontBehaviourInterfaceFactory::AliasContainer&
  MFrontBehaviourInterfaceFactory::getAliasesMap(void)
  {
    static AliasContainer map;
    return map;
  } // end of MFrontBehaviourInterfaceFactory::getAliasesMap

} // end of namespace mfront
