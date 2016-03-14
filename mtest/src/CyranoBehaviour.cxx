/*!
 * \file   mfront/mtest/CyranoBehaviour.cxx
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
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Cyrano/Cyrano.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/CyranoBehaviour.hxx"
#include"MFront/Cyrano/CyranoComputeStiffnessTensor.hxx"


namespace mtest
{

  CyranoBehaviour::CyranoBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
					     const std::string& l,
					     const std::string& b)
    : UmatBehaviourBase(h,l,b)
  {
    using namespace tfel::system;
    using namespace tfel::material;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    this->fct = elm.getCyranoFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b,this->hypothesis);
    const bool b1 = elm.getUMATRequiresStiffnessTensor(l,b,this->hypothesis);
    const bool b2 = elm.getUMATRequiresThermalExpansionCoefficientTensor(l,b,this->hypothesis);
    if(this->stype==0){
      if(b1){
	this->mpnames.push_back("YoungModulus");
	this->mpnames.push_back("PoissonRatio");
      }
      if(b2){
	this->mpnames.push_back("ThermalExpansion");
      }
    } else {
      std::vector<std::string> tmp;
      if((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
    	 (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
    	if(b1){
	  tmp.push_back("YoungModulus1");
	  tmp.push_back("YoungModulus2");
	  tmp.push_back("YoungModulus3");
	  tmp.push_back("PoissonRatio12");
	  tmp.push_back("PoissonRatio23");
	  tmp.push_back("PoissonRatio13");
	}
	if(b2){
	  tmp.push_back("ThermalExpansion1");
	  tmp.push_back("ThermalExpansion2");
	  tmp.push_back("ThermalExpansion3");
	}
      } else {
    	throw(std::runtime_error("UmatStandardBehaviour::UmatStandardBehaviour: "
				 "unsupported hypothesis"));
      }
      this->mpnames.insert(this->mpnames.begin(),tmp.begin(),tmp.end());
    }
  }

  void
  CyranoBehaviour::allocate(BehaviourWorkSpace& wk,
			    const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    const auto nstatev = this->getInternalStateVariablesSize(h);
    wk.D.resize(3u,3u);
    wk.kt.resize(3u,3u);
    wk.k.resize(3u,3u);
    wk.mps.resize(this->mpnames.size() ==0 ? 1u : this->mpnames.size(),real(0));
    wk.ivs.resize(nstatev==0 ? 1u : nstatev,real(0));
    wk.nk.resize(3u,3u);
    wk.ne.resize(3u);
    wk.ns.resize(3u);
    wk.nivs.resize(nstatev);
    mtest::allocate(wk.cs,*this,h);
  }

  tfel::math::tmatrix<3u,3u,real>
  CyranoBehaviour::getRotationMatrix(const tfel::math::vector<real>&,
					  const tfel::math::tmatrix<3u,3u,real>& r) const
  {
    return r;
  } // end of CyranoBehaviour::getRotationMatrix

  StiffnessMatrixType
  CyranoBehaviour::getDefaultStiffnessMatrixType(void) const
  {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  } // end of CyranoBehaviour::getDefaultStiffnessMatrixType

  void
  CyranoBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
  } // end of CyranoBehaviour::setDrivingVariablesDefaultInitialValue  

  std::pair<bool,real>
  CyranoBehaviour::computePredictionOperator(BehaviourWorkSpace& wk,
					     const CurrentState& s,
					     const tfel::material::ModellingHypothesis::Hypothesis h,
					     const StiffnessMatrixType ktype) const
  {
    if(ktype==StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      return {false,real(-1)};
    }
    wk.cs = s;
    return this->call_behaviour(wk.kt,wk.cs,wk,h,real(1),ktype,false);
  } // end of CyranoBehaviour::computePredictionOperator

  std::pair<bool,real>
  CyranoBehaviour::integrate(CurrentState& s,
			     BehaviourWorkSpace& wk,
			     const tfel::material::ModellingHypothesis::Hypothesis h,
			     const real dt,
			     const StiffnessMatrixType ktype) const
  {
    return this->call_behaviour(wk.k,s,wk,h,dt,ktype,true);
  } // end of CyranoBehaviour::integrate

  std::pair<bool,real>
  CyranoBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
				  CurrentState& s,
				  BehaviourWorkSpace& wk,
				  const tfel::material::ModellingHypothesis::Hypothesis h,
				  const real dt,
				  const StiffnessMatrixType ktype,
				  const bool b) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace cyrano;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    using cyrano::CyranoComputeStiffnessTensor;
    CyranoInt ntens;
    CyranoInt ndi;
    CyranoInt nprops = s.mprops1.size() == 0 ? 1 : static_cast<CyranoInt>(s.mprops1.size());
    CyranoInt nstatv;
    if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      ndi   = 1;
      ntens = 3;
    } else if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS){
      ndi   = 2;
      ntens = 3;
    } else {
      string msg("CyranoBehaviour::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    if((Kt.getNbRows()!=3u)||(Kt.getNbCols()!=3u)){
      throw(runtime_error("CyranoBehaviour::integrate: "
			  "invalid tangent operator size"));
    }
    if(((s.iv0.size()==0)&&(wk.ivs.size()!=1u))||
       ((s.iv0.size()!=0)&&(s.iv0.size()!=wk.ivs.size()))){
      throw(runtime_error("CyranoBehaviour::integrate: "
			  "the memory has not been allocated correctly"));
    }
    fill(wk.D.begin(),wk.D.end(),0.);
    // choosing the type of stiffness matrix
    UmatBehaviourBase::initializeTangentOperator(wk.D,ktype,b);
    // using a local copy of material properties to handle the
    // case where s.mprops1 is empty
    copy(s.mprops1.begin(),s.mprops1.end(),wk.mps.begin());
    if(s.mprops1.empty()){
      wk.mps[0] = real(0);
    }
    // state variable initial values
    copy(s.iv0.begin(),s.iv0.end(),wk.ivs.begin());
    if(s.iv0.empty()){
      wk.ivs[0]=real(0);
    }
    nstatv = static_cast<CyranoInt>(wk.ivs.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot(0.);
    tmatrix<3u,3u,real>::size_type i,j;
    for(i=0;i!=3u;++i){
      for(j=0;j!=3u;++j){
	if(i==j){
	  drot(i,j) = 1;
	} else {
	  drot(i,j) = 0;
	}
      }
    }
    CyranoInt kinc(1);
    stensor<1u,real> ue0(real(0));
    stensor<1u,real> ude(real(0));
    copy(s.e0.begin(),s.e0.end(),ue0.begin());
    for(i=0;i!=3u;++i){
      ude(i)  = s.e1(i)-s.e0(i);
    }
    copy(s.s0.begin(),s.s0.end(),s.s1.begin());
    // thermal strain
    for(i=0;i!=3u;++i){
      ue0(i) -= s.e_th0(i);
      ude(i) -= s.e_th1(i)-s.e_th0(i);
    }
    // turning to cyrano convention
    swap(s.s1(1),s.s1(2));
    swap(ue0(1),ue0(2));
    swap(ude(1),ude(2));
    // integration
    (this->fct)(&ntens,&dt,&drot(0,0),
		&(wk.D(0,0)),&ue0(0),&ude(0),
		&(s.esv0(0)),&(s.desv(0)),
		&wk.mps(0),&nprops,
		&(s.esv0(0))+1,&(s.desv(0))+1,
		&wk.ivs(0),&nstatv,&(s.s1(0)),
		&ndi,&kinc);
    if(kinc!=1){
      return {false,1};
    }
    if(!s.iv1.empty()){
      copy_n(wk.ivs.begin(), s.iv1.size(),s.iv1.begin());
    }
    // turning back to MFront conventions
    swap(s.s1(1),s.s1(2));
    // tangent operator (...)
    if(ktype!=StiffnessMatrixType::NOSTIFFNESS){ 
      // transpose (fortran -> c++)
      tmatrix<3u,3u,real> D2;
      for(unsigned short mi=0;mi!=3u;++mi){
	for(unsigned short mj=0;mj!=3u;++mj){
	  D2(mi,mj)=wk.D(mj,mi);
	}
      }
      // change to MTest conventions
      Kt(0,0)=D2(0,0);
      Kt(1,0)=D2(2,0);
      Kt(2,0)=D2(1,0);
      Kt(0,1)=D2(0,2);
      Kt(1,1)=D2(2,2);
      Kt(2,1)=D2(1,2);
      Kt(0,2)=D2(0,1);
      Kt(1,2)=D2(2,1);
      Kt(2,2)=D2(1,1);
    }
    return {true,1};
  } // end of CyranoBehaviour::integrate
      
  CyranoBehaviour::~CyranoBehaviour()
  {}
  
} // end of namespace mtest

