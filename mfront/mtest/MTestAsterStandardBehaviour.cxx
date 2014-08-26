/*! 
 * \file  MTestAsterStandardBehaviour.cxx
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
#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterComputeStiffnessTensor.hxx"

#include"MFront/MTestUmatNormaliseTangentOperator.hxx"
#include"MFront/MTestAsterStandardBehaviour.hxx"

namespace mfront
{

  MTestAsterStandardBehaviour::MTestAsterStandardBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
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
	string msg("MTestAsterStandardBehaviour::MTestAsterStandardBehaviour : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestAsterStandardBehaviour::MTestAsterStandardBehaviour : "
		 "unsupported behaviour type "
		 "(neither isotropic nor orthotropic)");
      throw(runtime_error(msg));
    }
    this->mpnames.insert(this->mpnames.begin(),tmp.begin(),tmp.end());
  }

  tfel::math::tmatrix<3u,3u,real>
  MTestAsterStandardBehaviour::getRotationMatrix(const tfel::math::vector<real>&,
						    const tfel::math::tmatrix<3u,3u,real>& r) const
  {
    return r;
  } // end of MTestAsterStandardBehaviour::getRotationMatrix

  size_t
  MTestAsterStandardBehaviour::getInternalStateVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis h) const
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
	string msg("MTestAsterStandardBehaviour::getInternalStateVariablesSize : "
		   "invalid modelling hypothesis");
	throw(runtime_error(msg));
      }	
    }
    return s+ MTestUmatBehaviourBase::getInternalStateVariablesSize(h);
  } // end of MTestAsterStandardBehaviour::getInternalStateVariablesSize() const

  std::vector<std::string>
  MTestAsterStandardBehaviour::getInternalStateVariablesDescriptions(const tfel::material::ModellingHypothesis::Hypothesis h) const
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
	string msg("MTestAsterStandardBehaviour::getInternalStateVariablesDescriptions : "
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
  } // end of MTestAsterStandardBehaviour::getInternalStateVariablesDescriptions

  void
  MTestAsterStandardBehaviour::allocate(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    const unsigned short ndv     = this->getDrivingVariablesSize(h);
    const unsigned short nth     = this->getThermodynamicForcesSize(h);
    const unsigned short nstatev = this->getInternalStateVariablesSize(h);
    this->D.resize(nth,ndv);
    if(nstatev==0){
      this->iv.resize(1u,real(0));
    } else {
      this->iv.resize(nstatev);
    }
  } // end of MTestAsterStandardBehaviour::allocate

  MTestStiffnessMatrixType::mtype
  MTestAsterStandardBehaviour::getDefaultStiffnessMatrixType(void) const
  {
    return MTestStiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }
  
  bool
  MTestAsterStandardBehaviour::computePredictionOperator(tfel::math::matrix<real>& Kt,
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
  MTestAsterStandardBehaviour::integrate(tfel::math::matrix<real>& Kt,
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
  } // end of MTestAsterStandardBehaviour::integrate

  MTestAsterStandardBehaviour::~MTestAsterStandardBehaviour()
  {}
  
} // end of namespace mfront









