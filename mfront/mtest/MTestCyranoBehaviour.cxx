/*!
 * \file   MTestCyranoBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#include<cmath>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Cyrano/Cyrano.hxx"
#include"MFront/MTestCyranoBehaviour.hxx"
#include"MFront/MTestUmatNormaliseTangentOperator.hxx"
#include"MFront/Cyrano/CyranoComputeStiffnessTensor.hxx"


namespace mfront
{

  MTestCyranoBehaviour::MTestCyranoBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
					     const std::string& l,
					     const std::string& b)
    : MTestUmatBehaviourBase(h,l,b)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
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
      vector<string> tmp;
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
    	string msg("MTestUmatStandardBehaviour::MTestUmatStandardBehaviour : ");
    	msg += "unsupported hypothesis";
    	throw(runtime_error(msg));
      }
      this->mpnames.insert(this->mpnames.begin(),tmp.begin(),tmp.end());
    }
  }

  void
  MTestCyranoBehaviour::allocate(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    const unsigned short nstatev = this->getInternalStateVariablesSize(h);
    this->D.resize(3u,3u);
    this->iv.resize(nstatev);
    if(iv.size()==0){
      iv.push_back(0.);
    }
  }

  tfel::math::tmatrix<3u,3u,real>
  MTestCyranoBehaviour::getRotationMatrix(const tfel::math::vector<real>&,
					  const tfel::math::tmatrix<3u,3u,real>& r) const
  {
    return r;
  } // end of MTestCyranoBehaviour::getRotationMatrix

  MTestStiffnessMatrixType::mtype
  MTestCyranoBehaviour::getDefaultStiffnessMatrixType(void) const
  {
    return MTestStiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  } // end of MTestCyranoBehaviour::getDefaultStiffnessMatrixType

  void
  MTestCyranoBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
  } // end of MTestCyranoBehaviour::setDrivingVariablesDefaultInitialValue  

  bool
  MTestCyranoBehaviour::computePredictionOperator(tfel::math::matrix<real>& Kt,
						  const tfel::math::tmatrix<3u,3u,real>&,
						  const tfel::math::vector<real>& e0,
						  const tfel::math::vector<real>& s0,
						  const tfel::math::vector<real>& mp,
						  const tfel::math::vector<real>& iv0,
						  const tfel::math::vector<real>& esv0,
						  const tfel::material::ModellingHypothesis::Hypothesis h,
						  const MTestStiffnessMatrixType::mtype ktype) const
  {
    using namespace tfel::math;
    if(ktype!=MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      vector<real> s1(s0);
      vector<real> e1(e0);
      vector<real> iv1(iv0);
      vector<real> desv(esv0.size(),real(0));
      return this->call_behaviour(Kt,s1,iv1,e0,e1,s0,
				  mp,iv0,esv0,desv,
				  h,real(1),ktype,false);
    }
    this->computeElasticStiffness(Kt,mp,h);
    return true;
  } // end of MTestCyranoBehaviour::computePredictionOperator

  bool
  MTestCyranoBehaviour::integrate(tfel::math::matrix<real>& Kt,
					    tfel::math::vector<real>& s1,
					    tfel::math::vector<real>& iv1,
					    const tfel::math::tmatrix<3u,3u,real>&,
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
    return this->call_behaviour(Kt,s1,iv1,e0,e1,s0,
				mp,iv0,ev0,dev,h,dt,
				ktype,true);
  } // end of MTestCyranoBehaviour::integrate

  bool
  MTestCyranoBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
						tfel::math::vector<real>& s1,
						tfel::math::vector<real>& iv1,
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
    using namespace cyrano;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    using cyrano::CyranoComputeStiffnessTensor;
    CyranoInt ntens;
    CyranoInt ndi;
    CyranoInt nprops = static_cast<CyranoInt>(mp.size());
    CyranoInt nstatv;
    if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      ndi   = 1;
      ntens = 3;
    } else if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS){
      ndi   = 2;
      ntens = 3;
    } else {
      string msg("MTestCyranoBehaviour::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    if((Kt.getNbRows()!=3u)||
       (Kt.getNbCols()!=3u)){
      string msg("MTestCyranoBehaviour::integrate : ");
      msg += "invalid tangent operator size";
      throw(runtime_error(msg));
    }
    if(((iv0.size()==0)&&(this->iv.size()!=1u))||
       ((iv0.size()!=0)&&(iv0.size()!=this->iv.size()))){
      string msg("MTestCyranoBehaviour::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    fill(this->D.begin(),this->D.end(),0.);
    // choosing the type of stiffness matrix
    MTestUmatBehaviourBase::initializeTangentOperator(ktype,b);
    // state variable initial values
    if(iv0.size()!=0){
      copy(iv0.begin(),iv0.end(),
	   this->iv.begin());
    }
    nstatv = static_cast<CyranoInt>(iv.size());
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
    copy(e0.begin(),e0.end(),ue0.begin());
    for(i=0;i!=3u;++i){
      ude(i) = e1(i)-e0(i);
    }
    copy(s0.begin(),s0.end(),s1.begin());
    // turning to cyrano convention
    swap(s1(1),s1(2));
    swap(ue0(1),ue0(2));
    swap(ude(1),ude(2));
    // integration
    (this->fct)(&ntens,&dt,&drot(0,0),
		&D(0,0),&ue0(0),&ude(0),
		&ev0(0),&dev(0),
		&mp(0),&nprops,
		&ev0(0)+1,&dev(0)+1,
		&iv(0),&nstatv,&s1(0),
		&ndi,&kinc);
    if(kinc!=1){
      return false;
    }
    if(!iv1.empty()){
      copy(iv.begin(),iv.end(),iv1.begin());
    }
    // turning back to MFront conventions
    swap(s1(1),s1(2));
    // tangent operator (...)
    if(ktype!=MTestStiffnessMatrixType::NOSTIFFNESS){ 
      if(ktype==MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
	this->computeElasticStiffness(Kt,mp,h);
      } else {
	// transpose (fortran -> c++)
	tmatrix<3u,3u,real> D2;
	for(unsigned short mi=0;mi!=3u;++mi){
	  for(unsigned short mj=0;mj!=3u;++mj){
	    D2(mi,mj)=D(mj,mi);
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
    }
    return true;
  } // end of MTestCyranoBehaviour::integrate

  void
  MTestCyranoBehaviour::computeElasticStiffness(tfel::math::matrix<real>& Kt,
						const tfel::math::vector<real>& mp,
						const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::math;
    using cyrano::CyranoComputeStiffnessTensor;
    typedef tfel::material::ModellingHypothesis MH;
    tmatrix<3u,3u,real>::size_type i,j;
    if(this->stype==0u){
      if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	CyranoComputeStiffnessTensor<MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				     cyrano::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS){
	st2tost2<1u,real> De;
	CyranoComputeStiffnessTensor<MH::AXISYMMETRICALGENERALISEDPLANESTRESS,
				     cyrano::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else {
	string msg("MTestCyranoBehaviour::integrate : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else if(this->stype==1u){
      if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	CyranoComputeStiffnessTensor<MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				     cyrano::ORTHOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS){
	st2tost2<1u,real> De;
	CyranoComputeStiffnessTensor<MH::AXISYMMETRICALGENERALISEDPLANESTRESS,
				     cyrano::ORTHOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else {
	string msg("MTestCyranoBehaviour::integrate : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestCyranoBehaviour::integrate : ");
      msg += "invalid behaviour type (neither "
	"isotropic or orthotropic)";
      throw(runtime_error(msg));
    }

  }
      
  MTestCyranoBehaviour::~MTestCyranoBehaviour()
  {}
  
} // end of namespace mfront

