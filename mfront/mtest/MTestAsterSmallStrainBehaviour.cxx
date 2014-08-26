/*! 
 * \file  MTestAsterBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#include<cmath>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterComputeStiffnessTensor.hxx"

#include"MFront/MTestUmatNormaliseTangentOperator.hxx"
#include"MFront/MTestAsterSmallStrainBehaviour.hxx"

namespace mfront
{

  MTestAsterSmallStrainBehaviour::MTestAsterSmallStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& l,
								 const std::string& b)
    : MTestUmatBehaviourBase(h,l,b),
      savesTangentOperator(false)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getAsterFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b,this->hypothesis);
    bool eo = elm.getUMATRequiresStiffnessTensor(l,b,this->hypothesis);
    bool to = elm.getUMATRequiresThermalExpansionCoefficientTensor(l,b,this->hypothesis);
    unsigned short etype = elm.getUMATElasticSymmetryType(l,b);
    this->savesTangentOperator = elm.checkIfAsterBehaviourSavesTangentOperator(l,b);
    vector<string> tmp;
    if(this->type==1u){
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
	  string msg("MTestAsterSmallStrainBehaviour::MTestAsterSmallStrainBehaviour : "
		     "unsupported modelling hypothesis");
	  throw(runtime_error(msg));
	}
      } else {
	string msg("MTestAsterSmallStrainBehaviour::MTestAsterSmallStrainBehaviour : "
		   "unsupported behaviour type "
		   "(neither isotropic nor orthotropic)");
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestAsterSmallStrainBehaviour::MTestAsterSmallStrainBehaviour : ");
      msg += "unsupported behaviour type";
      throw(runtime_error(msg));
    }
    this->mpnames.insert(this->mpnames.begin(),tmp.begin(),tmp.end());
  }

  tfel::math::tmatrix<3u,3u,real>
  MTestAsterSmallStrainBehaviour::getRotationMatrix(const tfel::math::vector<real>&,
						    const tfel::math::tmatrix<3u,3u,real>& r) const
  {
    return r;
  } // end of MTestAsterSmallStrainBehaviour::getRotationMatrix

  void
  MTestAsterSmallStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
  } // end of MTestAsterSmallStrainBehaviour::setDrivingVariablesDefaultInitialValue  

  size_t
  MTestAsterSmallStrainBehaviour::getInternalStateVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis h) const
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
	string msg("MTestAsterSmallStrainBehaviour::getInternalStateVariablesSize : "
		   "invalid modelling hypothesis");
	throw(runtime_error(msg));
      }	
    }
    return s+ MTestUmatBehaviourBase::getInternalStateVariablesSize(h);
  } // end of MTestAsterSmallStrainBehaviour::getInternalStateVariablesSize() const

  std::vector<std::string>
  MTestAsterSmallStrainBehaviour::getInternalStateVariablesDescriptions(const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::material;
    vector<string> desc = MTestUmatBehaviourBase::getInternalStateVariablesDescriptions(h);
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
	string msg("MTestAsterSmallStrainBehaviour::getInternalStateVariablesDescriptions : "
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
  } // end of MTestAsterSmallStrainBehaviour::getInternalStateVariablesDescriptions

  void
  MTestAsterSmallStrainBehaviour::allocate(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    const unsigned short ndv   = this->getDrivingVariablesSize(h);
    const unsigned short nth   = this->getThermodynamicForcesSize(h);
    const unsigned short nstatev = this->getInternalStateVariablesSize(h);
    this->D.resize(nth,ndv);
    if(nstatev==0){
      this->iv.resize(1u,real(0));
    } else {
      this->iv.resize(nstatev);
    }
  } // end of MTestAsterSmallStrainBehaviour::allocate

  MTestStiffnessMatrixType::mtype
  MTestAsterSmallStrainBehaviour::getDefaultStiffnessMatrixType(void) const
  {
    return MTestStiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }
  
  bool
  MTestAsterSmallStrainBehaviour::computePredictionOperator(tfel::math::matrix<real>& Kt,
							    const tfel::math::tmatrix<3u,3u,real>& r,
							    const tfel::math::vector<real>& e0,
							    const tfel::math::vector<real>& s0,
							    const tfel::math::vector<real>& mprops0,
							    const tfel::math::vector<real>& iv0,
							    const tfel::math::vector<real>& esv0,
							    const tfel::material::ModellingHypothesis::Hypothesis h,
							    const MTestStiffnessMatrixType::mtype ktype) const
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
  MTestAsterSmallStrainBehaviour::integrate(tfel::math::matrix<real>& Kt,
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
					    const MTestStiffnessMatrixType::mtype ktype) const
  {
    return this->call_behaviour(Kt,s1,iv1,r,e0,e1,s0,
				mp,iv0,ev0,dev,h,dt,
				ktype,true);
  } // end of MTestAsterSmallStrainBehaviour::integrate

  bool
  MTestAsterSmallStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
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
      string msg("MTestAsterSmallStrainBehaviour::call_beahviour : ");
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
      } else if(ktype==MTestStiffnessMatrixType::CONSISTENTTANGENTOPERATOR){
	D(0,0) = real(4);
      } else {
	string msg("MTestAsterSmallStrainBehaviour::call_behaviour : "
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
	string msg("MTestAsterSmallStrainBehaviour::call_behaviour : "
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
    stensor<3u,real> ue0(real(0));
    stensor<3u,real> ude(real(0));
    copy(e0.begin(),e0.end(),ue0.begin());
    for(i=0;i!=e1.size();++i){
      ude(i) = e1(i)-e0(i);
    }
    copy(s0.begin(),s0.end(),s1.begin());
    if(this->type==1u){
      for(i=3;i!=static_cast<unsigned short>(ntens);++i){
	s1(i)  /= sqrt2;
	ue0(i) *= sqrt2;
	ude(i) *= sqrt2;
      }
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
      if(this->type==1u){
	for(i=3;i!=static_cast<unsigned short>(ntens);++i){
	  s1(i) *= sqrt2;
	}
      }
    }
    return true;
  }

  MTestAsterSmallStrainBehaviour::~MTestAsterSmallStrainBehaviour()
  {}
  
} // end of namespace mfront









