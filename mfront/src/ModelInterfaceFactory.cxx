/*!
 * \file   mfront/src/ModelInterfaceFactory.cxx
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

#include"MFront/ModelInterfaceFactory.hxx"

namespace mfront{

  ModelInterfaceFactory&
  ModelInterfaceFactory::getModelInterfaceFactory()
  {
    static ModelInterfaceFactory f;
    return f;
  } // end of ModelInterfaceFactory::getModelInterfaceFactory

  ModelInterfaceFactory::InterfaceCreatorsContainer&
  ModelInterfaceFactory::getInterfaceCreatorsMap(void)
  {
    static InterfaceCreatorsContainer map;
    return map;
  } // end of ModelInterfaceFactory::getInterfaceCreatorsMap

  ModelInterfaceFactory::InterfaceContainer&
  ModelInterfaceFactory::getInterfacesMap(void)
  {
    static InterfaceContainer map;
    return map;
  } // end of ModelInterfaceFactory::getInterfacesMap

  ModelInterfaceFactory::ModelInterfaceFactory()
  {}

  std::vector<std::string>
  ModelInterfaceFactory::getRegistredInterfaces(void)
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
  ModelInterfaceFactory::registerInterfaceCreator(const std::string& i,
							const ModelInterfaceFactory::InterfaceCreator f)
  {
    using namespace std;
    InterfaceCreatorsContainer& imap = this->getInterfaceCreatorsMap();
    if(imap.find(i)!=imap.end()){
      string msg("ModelInterfaceFactory::registerInterfaceCreator : ");
      msg += "interface creator '"+i+"' already declared";
      throw(runtime_error(msg));
    }
    imap.insert(make_pair(i,f));
  }

  AbstractModelInterface* 
  ModelInterfaceFactory::getInterfacePtr(const std::string& interfaceName)
  {
    using namespace std;
    InterfaceCreatorsContainer::iterator p;
    InterfaceContainer::iterator m;
    AbstractModelInterface *i;
    m = this->getInterfacesMap().find(interfaceName);
    if(m==this->getInterfacesMap().end()){
      p = this->getInterfaceCreatorsMap().find(interfaceName);
      if(p==this->getInterfaceCreatorsMap().end()){
	string msg = "ModelInterfaceFactory::createNewInterface : no interface named ";
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
      this->getInterfacesMap().insert(make_pair(interfaceName,i));
    } else {
      i = m->second;
    }
    return i;
  }

  void
  ModelInterfaceFactory::clear(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      delete m->second;
    }
    this->getInterfacesMap().clear();
  } // end of ModelInterfaceFactory::clear(void)

  ModelInterfaceFactory::~ModelInterfaceFactory()
  {
    assert(this->getInterfacesMap().empty());
  } // end of ModelInterfaceFactory::~ModelInterfaceFactory()
  
  void
  ModelInterfaceFactory::reset(void)
  {
    InterfaceContainer::iterator m;
    for(m = this->getInterfacesMap().begin();m!= this->getInterfacesMap().end();++m){
      m->second->reset();
    }
  } // end of ModelInterfaceFactory::reset

} // end of namespace mfront
