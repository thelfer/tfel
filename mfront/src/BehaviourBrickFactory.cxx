/*! 
 * \file   mfront/src/BehaviourBrickFactory.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  October 20, 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>

#include"MFront/BehaviourBrick.hxx"
#include"MFront/ElasticityBehaviourBrick.hxx"
#include"MFront/BehaviourBrickFactory.hxx"

namespace mfront
{

  template<typename T>
  static std::shared_ptr<BehaviourBrick>
  buildBehaviourBrickConstructor(AbstractBehaviourDSL& dsl,
			BehaviourDescription& mb,
			const BehaviourBrick::Parameters& p)
  {
    return std::shared_ptr<BehaviourBrick>(new T(dsl,mb,p));
  } // end of buildAlgoritmConstructor

  BehaviourBrickFactory&
  BehaviourBrickFactory::getFactory()
  {
    static BehaviourBrickFactory factory;
    return factory;
  }
  
  std::shared_ptr<BehaviourBrick>
  BehaviourBrickFactory::get(const std::string& a,
			     AbstractBehaviourDSL& dsl,
			     BehaviourDescription& mb,
			     const BehaviourBrick::Parameters& p) const
  {
    using namespace std;
    map<string,constructor>::const_iterator pc;
    pc = this->constructors.find(a);
    if(pc==this->constructors.end()){
      string msg("BehaviourBrickFactory::getBehaviourBrick : "
		 "no BehaviourBrick '"+a+"' registred");
      throw(runtime_error(msg));
    }
    return (*(pc->second))(dsl,mb,p);
  }

  BehaviourBrickFactory::BehaviourBrickFactory()
  {
    this->registerBehaviourBrick("StandardElasticity",
				 buildBehaviourBrickConstructor<ElasticityBehaviourBrick>);
  } // end of BehaviourBrickFactory::BehaviourBrickFactory

  void
  BehaviourBrickFactory::registerBehaviourBrick(const std::string& a,
			       const constructor c)
  {
    using namespace std;
    typedef map<string,constructor>::value_type value_type;
    if(!this->constructors.insert(value_type(a,c)).second){
      string msg("BehaviourBrickFactory::registerBehaviourBrick : "
		 "BehaviourBrick '"+a+"' already declared");
      throw(runtime_error(msg));
    }
  } // end of BehaviourBrickFactory::registerBehaviourBrick

} // end of namespace mfront
