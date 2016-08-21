/*! 
 * \file  mfront/mtest/AbaqusStandardBehaviour.cxx
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
#include"MFront/Abaqus/Abaqus.hxx"
#include"MFront/Abaqus/AbaqusComputeStiffnessTensor.hxx"

#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/AbaqusStandardBehaviour.hxx"

namespace mtest
{

  std::string
  AbaqusStandardBehaviour::getBehaviourName(const std::string& b,
					    const Hypothesis h)
  {
    auto ends = [&b,h](const std::string& s){
      if(b.length()>=s.length()){
        return b.compare(b.length()-s.length(),s.length(),s)==0;
      }
      return false;
    };
    const auto s = [h]() -> std::string {
      if(h==tfel::material::ModellingHypothesis::AXISYMMETRICAL){
	return "_AXIS";
      } else if(h==tfel::material::ModellingHypothesis::PLANESTRAIN){
	return "_PSTRAIN";
      } else if(h==tfel::material::ModellingHypothesis::PLANESTRESS){
	return "_PSTRESS";
      } else if(h==tfel::material::ModellingHypothesis::TRIDIMENSIONAL){
	return "_3D";
      }
      throw(std::runtime_error("AbaqusStandardBehaviour::AbaqusStandardBehaviour: "
			       "invalid hypothesis."));
    }();
    if(!ends(s)){
      throw(std::runtime_error("AbaqusStandardBehaviour::AbaqusStandardBehaviour: "
			       "invalid function name."));
    }
    return {b.begin(),b.begin()+b.length()-s.length()};    
  }
  
  AbaqusStandardBehaviour::AbaqusStandardBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
						   const std::string& l,
						   const std::string& b)
    : UmatBehaviourBase(h,l,AbaqusStandardBehaviour::getBehaviourName(b,h))
  {
    using namespace std;
    using namespace tfel::material;
    auto& elm = tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto bn = AbaqusStandardBehaviour::getBehaviourName(b,h);
    this->fct = elm.getAbaqusExternalBehaviourFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,bn,this->hypothesis);
    bool eo = elm.getUMATRequiresStiffnessTensor(l,bn,this->hypothesis);
    bool to = elm.getUMATRequiresThermalExpansionCoefficientTensor(l,bn,this->hypothesis);
    unsigned short etype = elm.getUMATElasticSymmetryType(l,bn);
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
      if((h==ModellingHypothesis::PLANESTRESS)||
	 (h==ModellingHypothesis::PLANESTRAIN)||
	 (h==ModellingHypothesis::AXISYMMETRICAL)){
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
	throw(std::runtime_error("AbaqusStandardBehaviour::AbaqusStandardBehaviour : "
				 "unsupported modelling hypothesis"));
      }
    } else {
      throw(std::runtime_error("AbaqusStandardBehaviour::AbaqusStandardBehaviour : "
			       "unsupported behaviour type "
			       "(neither isotropic nor orthotropic)"));
    }
    this->mpnames.insert(this->mpnames.begin(),tmp.begin(),tmp.end());
  }

  tfel::math::tmatrix<3u,3u,real>
  AbaqusStandardBehaviour::getRotationMatrix(const tfel::math::vector<real>&,
					    const tfel::math::tmatrix<3u,3u,real>& r) const
  {
    return r;
  } // end of AbaqusStandardBehaviour::getRotationMatrix

  void
  AbaqusStandardBehaviour::allocate(BehaviourWorkSpace& wk) const
  {
    const auto ndv     = this->getDrivingVariablesSize();
    const auto nth     = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    wk.D.resize(nth,nth);
    wk.kt.resize(nth,ndv);
    wk.k.resize(nth,ndv);
    wk.mps.resize(this->mpnames.size()==0 ? 1u : this->mpnames.size(),real(0));
    wk.ivs.resize(nstatev);
    wk.nk.resize(nth,ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    mtest::allocate(wk.cs,this->shared_from_this());
  } // end of AbaqusStandardBehaviour::allocate

  StiffnessMatrixType
  AbaqusStandardBehaviour::getDefaultStiffnessMatrixType(void) const
  {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }
  
  std::pair<bool,real>
  AbaqusStandardBehaviour::computePredictionOperator(BehaviourWorkSpace& wk,
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
  AbaqusStandardBehaviour::integrate(CurrentState& s,
				     BehaviourWorkSpace& wk,
				     const real dt,
				     const StiffnessMatrixType ktype) const
  {
    return this->call_behaviour(wk.k,s,wk,dt,ktype,true);
  } // end of AbaqusStandardBehaviour::integrate

  AbaqusStandardBehaviour::~AbaqusStandardBehaviour()
  {}
  
} // end of namespace mtest
