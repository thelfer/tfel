/*! 
 * \file  mfront/mtest/EuroplexusStandardBehaviour.cxx
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
#include"MFront/Europlexus/Europlexus.hxx"
#include"MFront/Europlexus/EuroplexusComputeStiffnessTensor.hxx"

#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/EuroplexusStandardBehaviour.hxx"

namespace mtest
{

  EuroplexusStandardBehaviour::EuroplexusStandardBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
							   const std::string& l,
							   const std::string& b)
    : UmatBehaviourBase(h,l,b)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    using ELM = tfel::system::ExternalLibraryManager;
    auto& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getEuroplexusFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b,this->hypothesis);
    bool eo = elm.getUMATRequiresStiffnessTensor(l,b,this->hypothesis);
    bool to = elm.getUMATRequiresThermalExpansionCoefficientTensor(l,b,this->hypothesis);
    unsigned short etype = elm.getUMATElasticSymmetryType(l,b);
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
	throw(runtime_error("EuroplexusStandardBehaviour::EuroplexusStandardBehaviour: "
			    "unsupported modelling hypothesis"));
      }
    } else {
      throw(runtime_error("EuroplexusStandardBehaviour::EuroplexusStandardBehaviour: "
			  "unsupported behaviour type "
			  "(neither isotropic nor orthotropic)"));
    }
    this->mpnames.insert(this->mpnames.begin(),tmp.begin(),tmp.end());
  }

  tfel::math::tmatrix<3u,3u,real>
  EuroplexusStandardBehaviour::getRotationMatrix(const tfel::math::vector<real>&,
						 const tfel::math::tmatrix<3u,3u,real>& r) const
  {
    return r;
  } // end of EuroplexusStandardBehaviour::getRotationMatrix

  void
  EuroplexusStandardBehaviour::allocate(BehaviourWorkSpace& wk) const
  {
    const auto ndv     = this->getDrivingVariablesSize();
    const auto nth     = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    wk.kt.resize(nth,ndv);
    wk.k.resize(nth,ndv);
    wk.D.resize(nth,ndv);
    // wk.mps.resize(this->mpnames.size(),real(0));
    // wk.ivs.resize(nstatev,real(0));
    wk.nk.resize(nth,ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    mtest::allocate(wk.cs,this->shared_from_this());
  } // end of EuroplexusStandardBehaviour::allocate

  StiffnessMatrixType
  EuroplexusStandardBehaviour::getDefaultStiffnessMatrixType() const
  {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }
  
  std::pair<bool,real>
  EuroplexusStandardBehaviour::computePredictionOperator(BehaviourWorkSpace& wk,
							 const CurrentState& s,
							 const StiffnessMatrixType ktype) const
  {
    if(ktype==StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      return {false,real(-1)};
    }
    wk.cs = s;
    return this->call_behaviour(wk.kt,wk.cs,wk,real(1),ktype,false);
  }

  std::pair<bool,real>
  EuroplexusStandardBehaviour::integrate(CurrentState& s,
					 BehaviourWorkSpace& wk,
					 const real dt,
					 const StiffnessMatrixType ktype) const
  {
    return this->call_behaviour(wk.k,s,wk,dt,ktype,true);
  } // end of EuroplexusStandardBehaviour::integrate

  EuroplexusStandardBehaviour::~EuroplexusStandardBehaviour()
  {}
  
} // end of namespace mtest
