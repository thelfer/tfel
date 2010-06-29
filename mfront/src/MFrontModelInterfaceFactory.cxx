/*!
 * \file   MFrontModelInterfaceFactory.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#include<stdexcept>
#include<cassert>

#include<dlfcn.h>

#include"MFront/MFrontModelInterfaceFactory.hxx"

namespace mfront{

  MFrontModelInterfaceFactory&
  MFrontModelInterfaceFactory::getMFrontModelInterfaceFactory()
  {
    static MFrontModelInterfaceFactory f;
    return f;
  } // end of MFrontModelInterfaceFactory::getMFrontModelInterfaceFactory

  MFrontModelInterfaceFactory::InterfaceCreatorsContainer&
  MFrontModelInterfaceFactory::getInterfaceCreatorsMap(void)
  {
    static InterfaceCreatorsContainer map;
    return map;
  } // end of MFrontModelInterfaceFactory::getInterfaceCreatorsMap

  MFrontModelInterfaceFactory::InterfaceContainer&
  MFrontModelInterfaceFactory::getInterfacesMap(void)
  {
    static InterfaceContainer map;
    return map;
  } // end of MFrontModelInterfaceFactory::getInterfacesMap

  MFrontModelInterfaceFactory::MFrontModelInterfaceFactory()
    : verboseMode(false)
  {}

  void MFrontModelInterfaceFactory::setVerboseMode(void)
  {
    this->verboseMode = true;
  }

  void MFrontModelInterfaceFactory::setDebugMode(void)
  {
    this->debugMode = true;
  }

  void MFrontModelInterfaceFactory::setWarningMode(void)
  {
    this->warningMode = true;
  }

  std::vector<std::string>
  MFrontModelInterfaceFactory::getRegistredInterfaces(void)
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
  MFrontModelInterfaceFactory::registerInterfaceCreator(const std::string& interfaceName,
							const MFrontModelInterfaceFactory::InterfaceCreator f)
  {
    using namespace std;
    this->getInterfaceCreatorsMap().insert(make_pair(interfaceName,f));
  }

  MFrontModelVirtualInterface* 
  MFrontModelInterfaceFactory::getInterfacePtr(const std::string& interfaceName)
  {
    using namespace std;
    InterfaceCreatorsContainer::iterator p;
    InterfaceContainer::iterator m;
    MFrontModelVirtualInterface *i;
    m = this->getInterfacesMap().find(interfaceName);
    if(m==this->getInterfacesMap().end()){
      p = this->getInterfaceCreatorsMap().find(interfaceName);
      if(p==this->getInterfaceCreatorsMap().end()){
	string msg = "MFrontModelInterfaceFactory::createNewInterface : no interface named ";
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
  MFrontModelInterfaceFactory::clear(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      delete m->second;
    }
    this->getInterfacesMap().clear();
  } // end of MFrontModelInterfaceFactory::clear(void)

  MFrontModelInterfaceFactory::~MFrontModelInterfaceFactory()
  {
    assert(this->getInterfacesMap().empty());
  } // end of MFrontModelInterfaceFactory::~MFrontModelInterfaceFactory()
  
  void
  MFrontModelInterfaceFactory::reset(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      m->second->reset();
    }
  } // end of MFrontModelInterfaceFactory::reset

} // end of namespace mfront
