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

#include"MTest/UmatNormaliseTangentOperator.hxx"
#include"MTest/AbaqusStandardBehaviour.hxx"

namespace mtest
{

  AbaqusStandardBehaviour::AbaqusStandardBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
						   const std::string& l,
						   const std::string& b)
    : UmatBehaviourBase(h,l,b)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    auto& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getAbaqusExternalBehaviourFunction(l,b);
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
      if((h==ModellingHypothesis::PLANESTRESS)||
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
  AbaqusStandardBehaviour::allocate(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    const auto ndv     = this->getDrivingVariablesSize(h);
    const auto nth     = this->getThermodynamicForcesSize(h);
    const auto nstatev = this->getInternalStateVariablesSize(h);
    this->D.resize(nth,ndv);
    this->mps.resize(this->mpnames.size()==0 ? 1u : this->mpnames.size(),real(0));
    this->ivs.resize(nstatev==0 ? 1u : nstatev,real(0));
  } // end of AbaqusStandardBehaviour::allocate

  StiffnessMatrixType::mtype
  AbaqusStandardBehaviour::getDefaultStiffnessMatrixType(void) const
  {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }
  
  bool
  AbaqusStandardBehaviour::computePredictionOperator(tfel::math::matrix<real>& Kt,
							    const tfel::math::tmatrix<3u,3u,real>& r,
							    const tfel::math::vector<real>& e0,
							    const tfel::math::vector<real>& s0,
							    const tfel::math::vector<real>& mprops0,
							    const tfel::math::vector<real>& iv0,
							    const tfel::math::vector<real>& esv0,
							    const tfel::material::ModellingHypothesis::Hypothesis h,
							    const StiffnessMatrixType::mtype ktype) const
  {
    using namespace tfel::math;
    vector<real> s1(s0);
    vector<real> e1(e0);
    vector<real> iv1(iv0);
    vector<real> desv(esv0.size(),real(0));
    return this->call_behaviour(Kt,s1,iv1,r,e0,e1,s0,
				mprops0,iv0,esv0,desv,
				h,real(1),ktype,false);
  }

  bool
  AbaqusStandardBehaviour::integrate(tfel::math::matrix<real>& Kt,
					    tfel::math::vector<real>& s1,
					    tfel::math::vector<real>& iv1,
					    const tfel::math::tmatrix<3u,3u,real>& r,
					    const tfel::math::vector<real>& e0,
					    const tfel::math::vector<real>& e1,
					    const tfel::math::vector<real>& s0,
					    const tfel::math::vector<real>& mp,
					    const tfel::math::vector<real>& iv0,
					    const tfel::math::vector<real>& ev0,
					    const tfel::math::vector<real>& dev,
					    const tfel::material::ModellingHypothesis::Hypothesis h,
					    const real dt,
					    const StiffnessMatrixType::mtype ktype) const
  {
    return this->call_behaviour(Kt,s1,iv1,r,e0,e1,s0,
				mp,iv0,ev0,dev,h,dt,
				ktype,true);
  } // end of AbaqusStandardBehaviour::integrate

  AbaqusStandardBehaviour::~AbaqusStandardBehaviour()
  {}
  
} // end of namespace mtest









