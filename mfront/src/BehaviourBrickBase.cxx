/*! 
 * \file  BehaviourBrickBase.cxx
 * \brief
 * \author Helfer Thomas
 * \date   21 oct. 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>
#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"MFront/AbstractBehaviourDSL.hxx"
#include"MFront/BehaviourDescription.hxx"
#include"MFront/BehaviourBrickBase.hxx"

namespace mfront{

  BehaviourBrickBase::BehaviourBrickBase(AbstractBehaviourDSL& dsl_,
					 BehaviourDescription& bd_)
    : dsl(dsl_),
      bd(bd_)
  {} // end of BehaviourBrickBase::BehaviourBrickBase

  void
  BehaviourBrickBase::addRequirements(bbrick::RequirementManager&,
				      const Hypothesis) const
  {} // end of BehaviourBrickBase::addRequirements
  
  void
  BehaviourBrickBase::checkThatParameterHasNoValue(const Parameter& p) const
  {
    using namespace std;
    if(!p.second.empty()){
      string msg("BehaviourBrickBase::checkThatParameterHasNoValue : "
		 "parameter '"+p.first+"' shall not have any value");
      throw(runtime_error(msg));
    }
  } // end of BehaviourBrickBase::checkThatParameterHasNoValue
  
  void
  BehaviourBrickBase::addMaterialPropertyIfNotDefined(const std::string& t,
						      const std::string& n,
						      const std::string& g) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH; 
    typedef MH::Hypothesis Hypothesis; 
    // treating material properties
    const pair<bool,bool> b = this->bd.checkVariableExistence(n);
    if(b.first){
      if(!b.second){
	string msg("BehaviourBrickBase::addMaterialPropertyIfNotDefined : "
		   "'"+n+"' is not declared for all specialisation of the behaviour");
	throw(runtime_error(msg));
      }
      this->bd.checkVariableExistence(n,"MaterialProperty");
      this->bd.checkVariableGlossaryName(n,g);
    } else {
      const Hypothesis h = MH::UNDEFINEDHYPOTHESIS;
      this->dsl.registerVariable(n,false);
      VariableDescription v(t,n,1u,0u);
      this->bd.addMaterialProperty(h,v);
      this->bd.setGlossaryName(h,n,g);
    }
  } // end of BehaviourBrickBase::addMaterialPropertyIfNotDefined

  void
  BehaviourBrickBase::addLocalVariable(const std::string& t,
				       const std::string& n) const
  {
    typedef tfel::material::ModellingHypothesis MH; 
    typedef MH::Hypothesis Hypothesis; 
    const Hypothesis h = MH::UNDEFINEDHYPOTHESIS;
    this->dsl.registerVariable(n,false);
    VariableDescription v(t,n,1u,0u);
    this->bd.addLocalVariable(h,v);
  } // end of BehaviourBrickBase::addLocalVariable

  BehaviourBrickBase::~BehaviourBrickBase()
  {} // end of BehaviourBrickBase::~BehaviourBrickBase

}
