/*!
 * \file   MFrontLawInterfaceFactory.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#include<stdexcept>
#include<algorithm>
#include<cassert>

#include<dlfcn.h>

#include"MFrontLawInterfaceFactory.hxx"

namespace mfront{

  MFrontLawInterfaceFactory&
  MFrontLawInterfaceFactory::getMFrontLawInterfaceFactory()
  {
    static MFrontLawInterfaceFactory f;
    return f;
  } // end of MFrontLawInterfaceFactory::getMFrontLawInterfaceFactory

  MFrontLawInterfaceFactory::InterfaceDependencyContainer&
  MFrontLawInterfaceFactory::getDependenciesMap(void)
  {
    static InterfaceDependencyContainer map;
    return map;
  } // end of MFrontLawInterfaceFactory::getDependenciesMap

  MFrontLawInterfaceFactory::InterfaceCreatorsContainer&
  MFrontLawInterfaceFactory::getInterfaceCreatorsMap(void)
  {
    static InterfaceCreatorsContainer map;
    return map;
  } // end of MFrontLawInterfaceFactory::getInterfaceCreatorsMap

  MFrontLawInterfaceFactory::InterfaceContainer&
  MFrontLawInterfaceFactory::getInterfacesMap(void)
  {
    static InterfaceContainer map;
    return map;
  } // end of MFrontLawInterfaceFactory::getInterfacesMap

  MFrontLawInterfaceFactory::AliasContainer&
  MFrontLawInterfaceFactory::getAliasesMap(void)
  {
    static AliasContainer map;
    return map;
  } // end of MFrontLawInterfaceFactory::getAliasesMap

  MFrontLawInterfaceFactory::MFrontLawInterfaceFactory()
    : verboseMode(false)
  {}

  void MFrontLawInterfaceFactory::setVerboseMode(void)
  {
    this->verboseMode = true;
  }

  void MFrontLawInterfaceFactory::setDebugMode(void)
  {
    this->debugMode = true;
  }

  void MFrontLawInterfaceFactory::setWarningMode(void)
  {
    this->warningMode = true;
  }

  std::vector<std::string>
  MFrontLawInterfaceFactory::getRegistredInterfaces(void)
  {
    using namespace std;
    AliasContainer::const_iterator p;
    vector<string> res;
    for(p  = this->getAliasesMap().begin();
	p != this->getAliasesMap().end();++p){
      res.push_back(p->first);
    }
    return res;
  }

  void 
  MFrontLawInterfaceFactory::registerInterfaceCreator(const std::string& interfaceName,
						      const MFrontLawInterfaceFactory::InterfaceCreator f)
  {
    using namespace std;
    this->getInterfaceCreatorsMap().insert(make_pair(interfaceName,f));
  }

  void
  MFrontLawInterfaceFactory::registerInterfaceAlias(const std::string& interfaceName,
						    const std::string& alias)
  {
    using namespace std;
    this->getAliasesMap().insert(make_pair(alias,interfaceName));
  }

  void
  MFrontLawInterfaceFactory::registerInterfaceDependency(const std::string& name,
							 const std::string& dep)
  {
    using namespace std;
    this->getDependenciesMap()[name].push_back(dep);
  } // end of MFrontLawInterfaceFactory::registerInterfaceDependency
  
  std::vector<std::string>
  MFrontLawInterfaceFactory::getInterfaceDependencies(const std::string& name)
  {
    using namespace std;
    vector<string> res;
    vector<string> tmp;
    AliasContainer::const_iterator p;
    vector<string>::const_iterator p2;
    p = this->getAliasesMap().find(name);
    if(p==this->getAliasesMap().end()){
      string msg = "MFrontLawInterfaceFactory::getInterfaceDependencies : no interface named '";
      msg += name+"'.\n";
      msg += "Available interface are : \n";
      for(p=this->getAliasesMap().begin();p!=this->getAliasesMap().end();++p){
	msg += p->first + " ";
      }
      throw(runtime_error(msg));
    }
    const vector<string>& deps = this->getDependenciesMap()[p->second];
    copy(deps.begin(),deps.end(),back_inserter(tmp));
    for(p2=deps.begin();p2!=deps.end();++p2){
      const vector<string>& deps2 = this->getInterfaceDependencies(*p2);
      copy(deps2.begin(),deps2.end(),back_inserter(tmp));
    }
    unique_copy(tmp.begin(),tmp.end(),back_inserter(res));
    return res;
  } // end of MFrontLawInterfaceFactory::getInterfaceDependencies

  MFrontLawVirtualInterface* 
  MFrontLawInterfaceFactory::getInterfacePtr(const std::string& interfaceName)
  {
    using namespace std;
    InterfaceCreatorsContainer::iterator p;
    AliasContainer::iterator p2;
    InterfaceContainer::iterator m;
    MFrontLawVirtualInterface *i;

    m = this->getInterfacesMap().find(interfaceName);
    if(m==this->getInterfacesMap().end()){
      p2 = this->getAliasesMap().find(interfaceName);
      if(p2==this->getAliasesMap().end()){
	string msg = "MFrontLawInterfaceFactory::createNewInterface : no interface named '";
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
  MFrontLawInterfaceFactory::clear(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      delete m->second;
    }
    this->getInterfacesMap().clear();
  } // end of MFrontLawInterfaceFactory::clear(void)

  MFrontLawInterfaceFactory::~MFrontLawInterfaceFactory()
  {
    assert(this->getInterfacesMap().empty());
  } // end of MFrontLawInterfaceFactory::~MFrontLawInterfaceFactory()
  
  void
  MFrontLawInterfaceFactory::reset(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      m->second->reset();
    }
  } // end of MFrontLawInterfaceFactory::reset

} // end of namespace mfront
