/*! 
 * \file   mfront/src/AbstractBehaviourBrickFactory.cxx
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

#include"MFront/AbstractBehaviourBrick.hxx"
#include"MFront/ElasticityBehaviourBrick.hxx"
#include"MFront/AbstractBehaviourBrickFactory.hxx"

namespace mfront
{

  template<typename T>
  static std::shared_ptr<AbstractBehaviourBrick>
  buildBehaviourBrickConstructor(AbstractBehaviourDSL& dsl,
				 BehaviourDescription& mb,
				 const AbstractBehaviourBrick::Parameters& p)
  {
    return std::shared_ptr<AbstractBehaviourBrick>(new T(dsl,mb,p));
  } // end of buildAlgoritmConstructor

  AbstractBehaviourBrickFactory&
  AbstractBehaviourBrickFactory::getFactory()
  {
    static AbstractBehaviourBrickFactory factory;
    return factory;
  }
  
  std::shared_ptr<AbstractBehaviourBrick>
  AbstractBehaviourBrickFactory::get(const std::string& a,
			     AbstractBehaviourDSL& dsl,
			     BehaviourDescription& mb,
			     const AbstractBehaviourBrick::Parameters& p) const
  {
    auto pc = this->constructors.find(a);
    if(pc==this->constructors.end()){
      throw(std::runtime_error("AbstractBehaviourBrickFactory::getAbstractBehaviourBrick : "
			       "no AbstractBehaviourBrick '"+a+"' registred"));
    }
    return (*(pc->second))(dsl,mb,p);
  }

  AbstractBehaviourBrickFactory::AbstractBehaviourBrickFactory()
  {
    this->registerAbstractBehaviourBrick("StandardElasticity",
					 buildBehaviourBrickConstructor<ElasticityBehaviourBrick>);
  } // end of AbstractBehaviourBrickFactory::AbstractBehaviourBrickFactory

  void
  AbstractBehaviourBrickFactory::registerAbstractBehaviourBrick(const std::string& a,
			       const constructor c)
  {
    if(!this->constructors.insert({a,c}).second){
      throw(std::runtime_error("AbstractBehaviourBrickFactory::registerAbstractBehaviourBrick : "
			       "BehaviourBrick '"+a+"' already declared"));
    }
  } // end of AbstractBehaviourBrickFactory::registerAbstractBehaviourBrick

} // end of namespace mfront
