/*! 
 * \file  mfront/src/ModelDSL.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 jun 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/ModelDSL.hxx"
#include"MFront/ModelInterfaceFactory.hxx"

namespace mfront
{

  std::string
  ModelDSL::getName(void)
  {
    return "Model";
  } // end of MaterialPropertyDSL::getName(void)

  std::string
  ModelDSL::getDescription(void)
  {
    return "this parser is used to define simple material models";
  } // end of ModelDSL::getDescription

  void
  ModelDSL::setInterfaces(const std::set<std::string>& f)
  {
    using namespace std;
    ModelDSLCommon::setInterfaces(f);
    vector<string>::const_iterator p;
    typedef ModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getModelInterfaceFactory();
    for(p=this->interfaces.begin();p!=this->interfaces.end();++p){
      AbstractModelInterface *interface = mmif.getInterfacePtr(*p);
      interface->declareReservedNames(this->reservedNames);
    }
  } // end of ModelDSL::setInterfaces

  void
  ModelDSL::treatFile(const std::string& f,
			       const std::vector<std::string>& ecmds)
  {
    using namespace std;
    if(this->interfaces.empty()){
      string msg("ModelDSL::treatFile : ");
      msg += "no interface defined";
      throw(runtime_error(msg));
    }
    ModelDSLBase<ModelDSL>::treatFile(f,ecmds);
  }

} // end of namespace mfront
