/*! 
 * \file  mfront/mtest/AsterStandardBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<sstream>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterComputeStiffnessTensor.hxx"

#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/UmatNormaliseTangentOperator.hxx"
#include"MTest/AsterStandardBehaviour.hxx"

namespace mtest
{

  AsterStandardBehaviour::AsterStandardBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
							   const std::string& l,
							   const std::string& b)
    : UmatBehaviourBase(h,l,b),
      savesTangentOperator(false)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    auto& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getAsterFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b,this->hypothesis);
    bool eo = elm.getUMATRequiresStiffnessTensor(l,b,this->hypothesis);
    bool to = elm.getUMATRequiresThermalExpansionCoefficientTensor(l,b,this->hypothesis);
    unsigned short etype = elm.getUMATElasticSymmetryType(l,b);
    this->savesTangentOperator = elm.checkIfAsterBehaviourSavesTangentOperator(l,b);
    vector<string> tmp;
    if(etype==0u){
      if(eo){
	tmp.push_back("YoungModulus");
	tmp.push_back("PoissonRatio");
      }
      if(to){
	tmp.push_back("ThermalExpansion");
      }
    } else if(etype==1u){
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	if(eo){
	  tmp.push_back("YoungModulus1");
	  tmp.push_back("YoungModulus2");
	  tmp.push_back("YoungModulus3");
	  tmp.push_back("PoissonRatio12");
	  tmp.push_back("PoissonRatio23");
	  tmp.push_back("PoissonRatio13");
	}
	if(to){
	  tmp.push_back("ThermalExpansion1");
	  tmp.push_back("ThermalExpansion2");
	  tmp.push_back("ThermalExpansion3");
	}
      } else if((h==ModellingHypothesis::PLANESTRESS)||
		(h==ModellingHypothesis::PLANESTRAIN)||
		(h==ModellingHypothesis::AXISYMMETRICAL)||
		(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	if(eo){
	  tmp.push_back("YoungModulus1");
	  tmp.push_back("YoungModulus2");
	  tmp.push_back("YoungModulus3");
	  tmp.push_back("PoissonRatio12");
	  tmp.push_back("PoissonRatio23");
	  tmp.push_back("PoissonRatio13");
	  tmp.push_back("ShearModulus12");
	}
	if(to){
	  tmp.push_back("ThermalExpansion1");
	  tmp.push_back("ThermalExpansion2");
	  tmp.push_back("ThermalExpansion3");
	}
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	if(eo){
	  tmp.push_back("YoungModulus1");
	  tmp.push_back("YoungModulus2");
	  tmp.push_back("YoungModulus3");
	  tmp.push_back("PoissonRatio12");
	  tmp.push_back("PoissonRatio23");
	  tmp.push_back("PoissonRatio13");
	  tmp.push_back("ShearModulus12");
	  tmp.push_back("ShearModulus23");
	  tmp.push_back("ShearModulus13");
	}
	if(to){
	  tmp.push_back("ThermalExpansion1");
	  tmp.push_back("ThermalExpansion2");
	  tmp.push_back("ThermalExpansion3");
	}
      } else { 
	string msg("AsterStandardBehaviour::AsterStandardBehaviour : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }
    } else {
      string msg("AsterStandardBehaviour::AsterStandardBehaviour : "
		 "unsupported behaviour type "
		 "(neither isotropic nor orthotropic)");
      throw(runtime_error(msg));
    }
    this->mpnames.insert(this->mpnames.begin(),tmp.begin(),tmp.end());
  }

  tfel::math::tmatrix<3u,3u,real>
  AsterStandardBehaviour::getRotationMatrix(const tfel::math::vector<real>&,
					    const tfel::math::tmatrix<3u,3u,real>& r) const
  {
    return r;
  } // end of AsterStandardBehaviour::getRotationMatrix

  size_t
  AsterStandardBehaviour::getInternalStateVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::material;
    size_t s(0);
    if(this->savesTangentOperator){
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	s=9;
      } else if((h==ModellingHypothesis::AXISYMMETRICAL)||(h==ModellingHypothesis::PLANESTRESS)||
		(h==ModellingHypothesis::PLANESTRAIN)||(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	s=16;
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	s=36;
      } else {
	string msg("AsterStandardBehaviour::getInternalStateVariablesSize : "
		   "invalid modelling hypothesis");
	throw(runtime_error(msg));
      }	
    }
    return s+ UmatBehaviourBase::getInternalStateVariablesSize(h);
  } // end of AsterStandardBehaviour::getInternalStateVariablesSize() const

  std::vector<std::string>
  AsterStandardBehaviour::getInternalStateVariablesDescriptions(const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::material;
    vector<string> desc = UmatBehaviourBase::getInternalStateVariablesDescriptions(h);
    if(this->savesTangentOperator){
      size_t s(0);
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	s=3;
      } else if((h==ModellingHypothesis::AXISYMMETRICAL)||(h==ModellingHypothesis::PLANESTRESS)||
		(h==ModellingHypothesis::PLANESTRAIN)||(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	s=4;
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	s=6;
      } else {
	string msg("AsterStandardBehaviour::getInternalStateVariablesDescriptions : "
		   "invalid modelling hypothesis");
	throw(runtime_error(msg));
      }
      for(unsigned short i=0;i!=s;++i){
	for(unsigned short j=0;j!=s;++j){
	  ostringstream n;
	  n << "component (" << j << "," << i << ") of the tangent operator";
	  desc.push_back(n.str());	  
	}
      }
    }
    return desc;
  } // end of AsterStandardBehaviour::getInternalStateVariablesDescriptions

  void
  AsterStandardBehaviour::allocate(BehaviourWorkSpace& wk,
				   const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    const auto ndv     = this->getDrivingVariablesSize(h);
    const auto nth     = this->getThermodynamicForcesSize(h);
    const auto nstatev = this->getInternalStateVariablesSize(h);
    wk.kt.resize(nth,ndv);
    wk.k.resize(nth,ndv);
    wk.D.resize(nth,ndv);
    wk.mps.resize(this->mpnames.size()==0 ? 1u : this->mpnames.size(),real(0));
    wk.ivs.resize(nstatev==0 ? 1u : nstatev,real(0));
    wk.nk.resize(nth,ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    mtest::allocate(wk.cs,*this,h);
  } // end of AsterStandardBehaviour::allocate

  StiffnessMatrixType
  AsterStandardBehaviour::getDefaultStiffnessMatrixType(void) const
  {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }
  
  std::pair<bool,real>
  AsterStandardBehaviour::computePredictionOperator(BehaviourWorkSpace& wk,
						    const CurrentState& s,
						    const tfel::material::ModellingHypothesis::Hypothesis h,
						    const StiffnessMatrixType ktype) const
  {
    if(ktype==StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      return {false,real(-1)};
    }
    wk.cs = s;
    return this->call_behaviour(wk.kt,wk.cs,wk,h,real(1),ktype,false);
  }

  std::pair<bool,real>
  AsterStandardBehaviour::integrate(CurrentState& s,
				    BehaviourWorkSpace& wk,
				    const tfel::material::ModellingHypothesis::Hypothesis h,
				    const real dt,
				    const StiffnessMatrixType ktype) const
  {
    return this->call_behaviour(wk.k,s,wk,h,dt,ktype,true);
  } // end of AsterStandardBehaviour::integrate

  AsterStandardBehaviour::~AsterStandardBehaviour()
  {}
  
} // end of namespace mtest
