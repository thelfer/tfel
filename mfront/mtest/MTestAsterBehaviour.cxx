/*! 
 * \file  MTestAsterBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#include<iostream>
#include<cmath>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterComputeStiffnessOperator.hxx"

#include"MFront/MTestUmatNormaliseTangentOperator.hxx"
#include"MFront/MTestAsterBehaviour.hxx"

namespace mfront
{

  MTestAsterBehaviour::MTestAsterBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
					   const std::string& l,
					   const std::string& b)
    : MTestUmatBehaviourBase(l,b),
      savesTangentOperator(false)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getAsterFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b);
    bool eo = elm.checkIfAsterBehaviourRequiresElasticMaterialPropertiesOffset(l,b);
    bool to = elm.checkIfAsterBehaviourRequiresThermalExpansionMaterialPropertiesOffset(l,b);
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
	  tmp.push_back("PoissonRatio13");
	  tmp.push_back("PoissonRatio23");
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
	string msg("MTestUmatBehaviourBase::MTestUmatBehaviourBase : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestUmatBehaviourBase::MTestUmatBehaviourBase : "
		 "unsupported behaviour type "
		 "(neither isotropic nor orthotropic)");
      throw(runtime_error(msg));
    }
    this->mpnames.insert(this->mpnames.begin(),tmp.begin(),tmp.end());
  }

  size_t
  MTestAsterBehaviour::getInternalStateVariablesSize(const unsigned short d) const
  {
    size_t s(0);
    if(this->savesTangentOperator){
      if(d==1){
	s+=9;
      } else if(d==2){ 
	s+=16;
      } else if(d==3){ 
	s+=36;
      }
    }
    return s+ MTestUmatBehaviourBase::getInternalStateVariablesSize(d);
  } // end of MTestAsterBehaviour::getInternalStateVariablesSize() const

  std::vector<std::string>
  MTestAsterBehaviour::getInternalStateVariablesDescriptions(const unsigned short d) const
  {
    using namespace std;
    vector<string> desc = MTestUmatBehaviourBase::getInternalStateVariablesDescriptions(d);
    if(this->savesTangentOperator){
      size_t s(0);
      if(d==1){
	s=3;
      } else if(d==2){ 
	s=4;
      } else if(d==3){ 
	s=6;
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
  } // end of MTestAsterBehaviour::getInternalStateVariablesDescriptions

  void
  MTestAsterBehaviour::allocate(const size_t ntens,
				const size_t nstatev)
  {
    this->D.resize(ntens,ntens);
    if(nstatev==0){
      this->iv.resize(1u,real(0));
    } else {
      this->iv.resize(nstatev);
    }
  } // end of MTestAsterBehaviour::allocate

  MTestStiffnessMatrixType::mtype
  MTestAsterBehaviour::getDefaultStiffnessMatrixType(void) const
  {
    return MTestStiffnessMatrixType::CONSISTANTTANGENTOPERATOR;
  }
  
  bool
  MTestAsterBehaviour::computePredictionOperator(tfel::math::matrix<real>& Kt,
						 const tfel::math::tmatrix<3u,3u,real>& r,
						 const tfel::math::stensor<3u,real>& e0,
						 const tfel::math::stensor<3u,real>& s0,
						 const tfel::math::vector<real>& mprops0,
						 const tfel::math::vector<real>& iv0,
						 const tfel::math::vector<real>& esv0,
						 const tfel::material::ModellingHypothesis::Hypothesis h,
						 const MTestStiffnessMatrixType::mtype ktype) const
  {
    using namespace tfel::math;
    stensor<3u,real> s1(s0);
    stensor<3u,real> de(real(0));
    vector<real> iv1(iv0);
    vector<real> desv(esv0.size(),real(0));
    return this->call_behaviour(Kt,s1,iv1,r,e0,de,s0,
				mprops0,iv0,esv0,desv,
				h,real(1),ktype,false);
  }

  bool
  MTestAsterBehaviour::integrate(tfel::math::matrix<real>& Kt,
				 tfel::math::stensor<3u,real>& s1,
				 tfel::math::vector<real>& iv1,
				 const tfel::math::tmatrix<3u,3u,real>& r,
				 const tfel::math::stensor<3u,real>& e0,
				 const tfel::math::stensor<3u,real>& de,
				 const tfel::math::stensor<3u,real>& s0,
				 const tfel::math::vector<real>& mp,
				 const tfel::math::vector<real>& iv0,
				 const tfel::math::vector<real>& ev0,
				 const tfel::math::vector<real>& dev,
				 const tfel::material::ModellingHypothesis::Hypothesis h,
				 const real dt,
				 const MTestStiffnessMatrixType::mtype ktype) const
  {
    return this->call_behaviour(Kt,s1,iv1,r,e0,de,s0,
				mp,iv0,ev0,dev,h,dt,
				ktype,true);
  } // end of MTestAsterBehaviour::integrate

  bool
  MTestAsterBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
				      tfel::math::stensor<3u,real>& s1,
				      tfel::math::vector<real>& iv1,
				      const tfel::math::tmatrix<3u,3u,real>& r,
				      const tfel::math::stensor<3u,real>& e0,
				      const tfel::math::stensor<3u,real>& de,
				      const tfel::math::stensor<3u,real>& s0,
				      const tfel::math::vector<real>& mp,
				      const tfel::math::vector<real>& iv0,
				      const tfel::math::vector<real>& ev0,
				      const tfel::math::vector<real>& dev,
				      const tfel::material::ModellingHypothesis::Hypothesis h,
				      const real dt,
				      const MTestStiffnessMatrixType::mtype ktype,
				      const bool b) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace aster;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    static const real sqrt2 = sqrt(real(2));
    unsigned short dimension;
    AsterInt ntens;
    AsterInt nprops = static_cast<AsterInt>(mp.size());
    AsterInt nstatv;
    if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      ntens = 3;
      dimension = 1u;
    } else if (h==MH::AXISYMMETRICAL){
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::PLANESTRESS){
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::PLANESTRAIN){
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::GENERALISEDPLANESTRAIN){
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::TRIDIMENSIONAL){
      ntens = 6;
      dimension = 3u;
    } else {
      string msg("MTestAsterBehaviour::call_beahviour : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    fill(this->D.begin(),this->D.end(),0.);
    // choosing the type of stiffness matrix
    if(b){
      if(ktype==MTestStiffnessMatrixType::NOSTIFFNESS){
	// do nothing
      } else if(ktype==MTestStiffnessMatrixType::ELASTIC){
	D(0,0) = real(1);
      } else if(ktype==MTestStiffnessMatrixType::SECANTOPERATOR){
	D(0,0) = real(2);
      } else if(ktype==MTestStiffnessMatrixType::TANGENTOPERATOR){
	D(0,0) = real(3);
      } else if(ktype==MTestStiffnessMatrixType::CONSISTANTTANGENTOPERATOR){
	D(0,0) = real(4);
      } else {
	string msg("MTestAsterBehaviour::call_behaviour : "
		   "invalid or unspecified stiffness matrix type");
	throw(runtime_error(msg));
      }
    } else {
      if(ktype==MTestStiffnessMatrixType::ELASTIC){
	D(0,0) = real(-1);
      } else if(ktype==MTestStiffnessMatrixType::SECANTOPERATOR){
	D(0,0) = real(-2);
      } else if(ktype==MTestStiffnessMatrixType::TANGENTOPERATOR){
	D(0,0) = real(-3);
      } else {
	string msg("MTestAsterBehaviour::call_behaviour : "
		   "invalid or unspecified stiffness matrix type");
	throw(runtime_error(msg));
      }
    }
    // using a local copy of internal state variables to handle the
    // case where iv0 is empty
    copy(iv0.begin(),iv0.end(),this->iv.begin());
    if(iv0.empty()){
      iv[0] = real(0.);
    }
    nstatv = static_cast<AsterInt>(iv.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot;
    tmatrix<3u,3u,real>::size_type i,j;
    for(i=0;i!=3u;++i){
      for(j=0;j!=3u;++j){
	drot(i,j) = r(j,i);
      }
    }
    stensor<3u,real> ue0(e0);
    stensor<3u,real> ude(de);
    copy(s0.begin(),s0.end(),s1.begin());
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i)  /= sqrt2;
      ue0(i) *= sqrt2;
      ude(i) *= sqrt2;
    }
    AsterReal ndt(1.);
    (this->fct)(&s1(0),&iv(0),&D(0,0),
		0,0,0,0,0,0,0,
		&ue0(0),&ude(0),0,&dt,
		&ev0(0),&dev(0),
		&ev0(0)+1,&dev(0)+1,
		0,0,0,&ntens,&nstatv,&mp(0),
		&nprops,0,&drot(0,0),&ndt,
		0,0,0,0,0,0,0,0,0);
    if(ndt<0.){
      return false;
    }
    if(ktype!=MTestStiffnessMatrixType::NOSTIFFNESS){
      MTestUmatNormaliseTangentOperator::exe(Kt,D,dimension);
    }
    if(b){
      if(!iv0.empty()){
	copy(iv.begin(),iv.end(),iv1.begin());
      }
      // turning things in standard conventions
      for(i=3;i!=static_cast<unsigned short>(ntens);++i){
	s1(i) *= sqrt2;
      }
    }
    return true;
  }


  MTestAsterBehaviour::~MTestAsterBehaviour()
  {}
  
} // end of namespace mfront









