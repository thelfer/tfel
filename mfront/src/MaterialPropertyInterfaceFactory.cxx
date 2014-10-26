/*!
 * \file   mfront/src/MaterialPropertyInterfaceFactory.cxx
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
#include<algorithm>
#include<cassert>

#include"MFront/MaterialPropertyInterfaceFactory.hxx"

namespace mfront{

  MaterialPropertyInterfaceFactory&
  MaterialPropertyInterfaceFactory::getMaterialPropertyInterfaceFactory()
  {
    static MaterialPropertyInterfaceFactory f;
    return f;
  } // end of MaterialPropertyInterfaceFactory::getMaterialPropertyInterfaceFactory

  MaterialPropertyInterfaceFactory::InterfaceDependencyContainer&
  MaterialPropertyInterfaceFactory::getDependenciesMap(void)
  {
    static InterfaceDependencyContainer map;
    return map;
  } // end of MaterialPropertyInterfaceFactory::getDependenciesMap

  MaterialPropertyInterfaceFactory::InterfaceCreatorsContainer&
  MaterialPropertyInterfaceFactory::getInterfaceCreatorsMap(void)
  {
    static InterfaceCreatorsContainer map;
    return map;
  } // end of MaterialPropertyInterfaceFactory::getInterfaceCreatorsMap

  MaterialPropertyInterfaceFactory::InterfaceContainer&
  MaterialPropertyInterfaceFactory::getInterfacesMap(void)
  {
    static InterfaceContainer map;
    return map;
  } // end of MaterialPropertyInterfaceFactory::getInterfacesMap

  MaterialPropertyInterfaceFactory::AliasContainer&
  MaterialPropertyInterfaceFactory::getAliasesMap(void)
  {
    static AliasContainer map;
    return map;
  } // end of MaterialPropertyInterfaceFactory::getAliasesMap

  MaterialPropertyInterfaceFactory::MaterialPropertyInterfaceFactory()
  {}

  std::vector<std::string>
  MaterialPropertyInterfaceFactory::getRegistredInterfaces(void)
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
  MaterialPropertyInterfaceFactory::registerInterfaceCreator(const std::string& i,
						      const MaterialPropertyInterfaceFactory::InterfaceCreator f)
  {
    using namespace std;
    InterfaceCreatorsContainer& imap = this->getInterfaceCreatorsMap();
    if(imap.find(i)!=imap.end()){
      string msg("MaterialPropertyInterfaceFactory::registerInterfaceCreator : ");
      msg += "interface creator '"+i+"' already declared";
      throw(runtime_error(msg));
    }
    imap.insert(make_pair(i,f));
  }

  void
  MaterialPropertyInterfaceFactory::registerInterfaceAlias(const std::string& i,
						    const std::string& a)
  {
    using namespace std;
    AliasContainer& amap = this->getAliasesMap();
    if(amap.find(a)!=amap.end()){
      string msg("MaterialPropertyInterfaceFactory::registerInterfaceCreator : ");
      msg += "interface alias '"+a+"' already declared";
      throw(runtime_error(msg));
    }
    amap.insert(make_pair(a,i));
  }

  void
  MaterialPropertyInterfaceFactory::registerInterfaceDependency(const std::string& name,
							 const std::string& dep)
  {
    using namespace std;
    this->getDependenciesMap()[name].push_back(dep);
  } // end of MaterialPropertyInterfaceFactory::registerInterfaceDependency
  
  std::vector<std::string>
  MaterialPropertyInterfaceFactory::getInterfaceDependencies(const std::string& name)
  {
    using namespace std;
    vector<string> res;
    vector<string> tmp;
    AliasContainer::const_iterator p;
    vector<string>::const_iterator p2;
    p = this->getAliasesMap().find(name);
    if(p==this->getAliasesMap().end()){
      string msg = "MaterialPropertyInterfaceFactory::getInterfaceDependencies : no interface named '";
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
  } // end of MaterialPropertyInterfaceFactory::getInterfaceDependencies

  AbstractMaterialPropertyInterface* 
  MaterialPropertyInterfaceFactory::getInterfacePtr(const std::string& interfaceName)
  {
    using namespace std;
    InterfaceCreatorsContainer::iterator p;
    AliasContainer::iterator p2;
    InterfaceContainer::iterator m;
    AbstractMaterialPropertyInterface *i;
    m = this->getInterfacesMap().find(interfaceName);
    if(m==this->getInterfacesMap().end()){
      p2 = this->getAliasesMap().find(interfaceName);
      if(p2==this->getAliasesMap().end()){
	string msg = "MaterialPropertyInterfaceFactory::createNewInterface : no interface named '";
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
  MaterialPropertyInterfaceFactory::clear(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      delete m->second;
    }
    this->getInterfacesMap().clear();
  } // end of MaterialPropertyInterfaceFactory::clear(void)

  MaterialPropertyInterfaceFactory::~MaterialPropertyInterfaceFactory()
  {
    assert(this->getInterfacesMap().empty());
  } // end of MaterialPropertyInterfaceFactory::~MaterialPropertyInterfaceFactory()
  
  void
  MaterialPropertyInterfaceFactory::reset(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      m->second->reset();
    }
  } // end of MaterialPropertyInterfaceFactory::reset

} // end of namespace mfront
