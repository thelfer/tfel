/*!
 * \file   MFrontBehaviourInterfaceFactory.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#include<stdexcept>
#include<cassert>

#include<dlfcn.h>

#include"MFrontBehaviourInterfaceFactory.hxx"

namespace mfront{

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
    : verboseMode(false)
  {}

  void MFrontBehaviourInterfaceFactory::setVerboseMode(void)
  {
    this->verboseMode = true;
  }

  void MFrontBehaviourInterfaceFactory::setDebugMode(void)
  {
    this->debugMode = true;
  }

  void MFrontBehaviourInterfaceFactory::setWarningMode(void)
  {
    this->warningMode = true;
  }

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
  MFrontBehaviourInterfaceFactory::registerInterfaceCreator(const std::string& interfaceName,
							    const MFrontBehaviourInterfaceFactory::InterfaceCreator f)
  {
    using namespace std;
    this->getInterfaceCreatorsMap().insert(make_pair(interfaceName,f));
  }

  MFrontBehaviourVirtualInterface* 
  MFrontBehaviourInterfaceFactory::getInterfacePtr(const std::string& interfaceName)
  {
    using namespace std;
    InterfaceCreatorsContainer::iterator p;
    InterfaceContainer::iterator m;
    MFrontBehaviourVirtualInterface *i;
    m = this->getInterfacesMap().find(interfaceName);
    if(m==this->getInterfacesMap().end()){
      p = this->getInterfaceCreatorsMap().find(interfaceName);
      if(p==this->getInterfaceCreatorsMap().end()){
	string msg = "MFrontBehaviourInterfaceFactory::createNewInterface : no interface named ";
	msg += interfaceName+".\n";
	msg += "Available interface are : \n";
	for(p  = this->getInterfaceCreatorsMap().begin();
	    p != this->getInterfaceCreatorsMap().end();++p){
	  msg += p->first + " ";
	}
	throw(runtime_error(msg));
      }
      InterfaceCreator c = p->second;
      i = c();
      if(this->verboseMode){
	i->setVerboseMode();
      }
      if(this->debugMode){
	i->setDebugMode();
      }
      if(this->warningMode){
	i->setWarningMode();
      }
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

  MFrontBehaviourInterfaceFactory behaviourInterfaceFactory;

} // end of namespace mfront
