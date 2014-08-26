/*! 
 * \file  MTestUmatCohesiveZoneModelBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#include<cmath>
#include<iostream>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/UMAT/UMAT.hxx"
#include"MFront/MTestUmatCohesiveZoneModelBehaviour.hxx"
#include"MFront/UMAT/UMATComputeStiffnessOperator.hxx"

namespace mfront
{

  MTestUmatCohesiveZoneModelBehaviour::MTestUmatCohesiveZoneModelBehaviour(const tfel::material::ModellingHypothesis::Hypothesis,
									   const std::string& l,
									   const std::string& b)
    : MTestUmatBehaviourBase(l,b)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getUMATFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b);
    if(this->type!=3u){
      string msg("MTestUmatCohesiveZoneModelBehaviour::MTestUmatCohesiveZoneModelBehaviour : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    if(this->stype==0){
      this->mpnames.insert(this->mpnames.begin(),"NormalThermalExpansion");
      this->mpnames.insert(this->mpnames.begin(),"MassDensity");
      // Those are the conventions used by Cast3M. The UMATInterface
      // exchanges the 'NormalStiffness' and the 'TangentialStiffness'
      // material properties to match MFront conventions
      this->mpnames.insert(this->mpnames.begin(),"NormalStiffness");
      this->mpnames.insert(this->mpnames.begin(),"TangentialStiffness");
    } else {
      string msg("MTestUmatCohesiveZoneModelBehaviour::MTestUmatCohesiveZoneModelBehaviour : "
		 "unsupported symmetry type");
      throw(runtime_error(msg));
    }
  }

  void
  MTestUmatCohesiveZoneModelBehaviour::allocate(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    const unsigned short ntens   = this->getProblemSize(h);
    const unsigned short nstatev = this->getInternalStateVariablesSize(h);
    this->D.resize(ntens,ntens);
    this->iv.resize(nstatev);
    if(iv.size()==0){
      iv.push_back(0.);
    }
  }

  MTestStiffnessMatrixType::mtype
  MTestUmatCohesiveZoneModelBehaviour::getDefaultStiffnessMatrixType(void) const
  {
    return MTestStiffnessMatrixType::ELASTIC;
  }
  
  bool
  MTestUmatCohesiveZoneModelBehaviour::computePredictionOperator(tfel::math::matrix<real>& Kt,
								 const tfel::math::tmatrix<3u,3u,real>& r,
								 const tfel::math::vector<real>&,
								 const tfel::math::vector<real>&,
								 const tfel::math::vector<real>& mp,
								 const tfel::math::vector<real>&,
								 const tfel::math::vector<real>&,
								 const tfel::material::ModellingHypothesis::Hypothesis h,
								 const MTestStiffnessMatrixType::mtype ktype) const
  {
    using namespace std;
    using namespace tfel::math;
    // rotation matrix
    tmatrix<3u,3u,real> drot(0.);
    tmatrix<3u,3u,real>::size_type i,j;
    for(i=0;i!=r.getNbRows();++i){
      for(j=0;j!=r.getNbCols();++j){
	drot(i,j) = r(j,i);
      }
    }
    if(ktype==MTestStiffnessMatrixType::ELASTIC){
      this->computeElasticStiffness(Kt,mp,drot,h);
      return true;
    } else {
      string msg("MTestUmatCohesiveZoneModelBehaviour::computePredictionOperator : "
		 "computation of the tangent operator "
		 "is not supported");
      throw(runtime_error(msg));
    }
    return false;
  } // end of MTestUmatCohesiveZoneModelBehaviour::computePredictionOperator

  bool
  MTestUmatCohesiveZoneModelBehaviour::integrate(tfel::math::matrix<real>& Kt,
						 tfel::math::vector<real>& s1,
						 tfel::math::vector<real>& iv1,
						 const tfel::math::tmatrix<3u,3u,real>& r,
						 const tfel::math::vector<real>& e0,
						 const tfel::math::vector<real>& de,
						 const tfel::math::vector<real>& s0,
						 const tfel::math::vector<real>& mp,
						 const tfel::math::vector<real>& iv0,
						 const tfel::math::vector<real>& ev0,
						 const tfel::math::vector<real>& dev,
						 const tfel::material::ModellingHypothesis::Hypothesis h,
						 const real dt,
						 const MTestStiffnessMatrixType::mtype ktype) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace umat;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    using umat::UMATComputeStiffnessOperator;
    UMATInt ntens;
    UMATInt ndi;
    UMATInt nprops = static_cast<UMATInt>(mp.size());
    UMATInt nstatv;
    if((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
       (h==MH::AXISYMMETRICAL)){
      string msg(" MTestUmatCohesiveZoneModelBehaviour::integrate : ");
      msg += "unsupported modelling hypothesis";
      throw(runtime_error(msg));
    } else if (h==MH::PLANESTRESS){
      ndi = -2;
      ntens = 2;
    } else if (h==MH::PLANESTRAIN){
      ndi = -1;
      ntens = 2;
    } else if (h==MH::GENERALISEDPLANESTRAIN){
      ndi = -3;
      ntens = 2;
    } else if (h==MH::TRIDIMENSIONAL){
      ndi = 2;
      ntens = 3;
    } else {
      string msg("MTestUmatCohesiveZoneModelBehaviour::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    if((this->D.getNbRows()!=Kt.getNbRows())||
       (this->D.getNbCols()!=Kt.getNbCols())){
      string msg("MTestUmatCohesiveZoneModelBehaviour::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    if(((iv0.size()==0)&&(this->iv.size()!=1u))||
       ((iv0.size()!=0)&&(iv0.size()!=this->iv.size()))){
      string msg("MTestUmatCohesiveZoneModelBehaviour::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    fill(this->D.begin(),this->D.end(),0.);
    if(iv0.size()!=0){
      copy(iv0.begin(),iv0.end(),
	   this->iv.begin());
    }
    nstatv = static_cast<UMATInt>(iv.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot(0.);
    tmatrix<3u,3u,real>::size_type i,j;
    for(i=0;i!=r.getNbRows();++i){
      for(j=0;j!=r.getNbCols();++j){
	drot(i,j) = r(j,i);
      }
    }
    UMATInt kinc(1);
    tvector<3u,real> ue0(real(0));
    tvector<3u,real> ude(real(0));
    if(ntens==2){
      ue0[0] = e0[1]; ue0[1] = e0[0];
      ude[0] = de[1]; ude[1] = de[0];
      s1[0]  = s0[1]; s1[1]  = s0[0];
    }
    if(ntens==3){
      ue0[0] = e0[1]; ue0[1] = e0[2]; ue0[2] = e0[0];
      ude[0] = de[1]; ude[1] = de[2]; ude[2] = de[0];
      s1[0]  = s0[1]; s1[1]  = s0[2]; s1[2]  = s0[0];
    }
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
    // tangent operator (...)
    if(ktype==MTestStiffnessMatrixType::ELASTIC){
      this->computeElasticStiffness(Kt,mp,drot,h);
    } else {
      string msg("MTestUmatCohesiveZoneModelBehaviour::integrate : "
		 "computation of the tangent operator "
		 "is not supported");
      throw(runtime_error(msg));
    }
    // turning things in standard conventions
    if(ntens==2){
      swap(s1[0],s1[1]);
    }
    if(ntens==3){
      const real tmp = s1[0];
      s1[0] = s1[2];
      s1[2] = s1[1];
      s1[1] = tmp;
    }
    return true;
  } // end of MTestUmatCohesiveZoneModelBehaviour::integrate

  void
  MTestUmatCohesiveZoneModelBehaviour::computeElasticStiffness(tfel::math::matrix<real>& Kt,
							       const tfel::math::vector<real>& mp,
							       const tfel::math::tmatrix<3u,3u,real>&,
							       const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::math;
    using umat::UMATComputeStiffnessOperator;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->stype==0u){
      if((h==MH::PLANESTRESS)||(h==MH::PLANESTRAIN)||
	 (h==MH::GENERALISEDPLANESTRAIN)){
	Kt(0,0) = mp(1);
	Kt(0,1) = real(0);
	Kt(1,0) = real(0);
	Kt(1,1) = mp(0);
      } else if (h==MH::TRIDIMENSIONAL){
	Kt(0,0) = mp(1);
	Kt(1,1) = mp(0);
	Kt(2,2) = mp(0);
	Kt(0,1) = Kt(0,2) = real(0);
	Kt(1,0) = Kt(1,2) = real(0);
	Kt(2,0) = Kt(2,1) = real(0);
      } else {
	string msg("MTestUmatCohesiveZoneModelBehaviour::integrate : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else if(this->stype==1u){
      string msg("MTestUmatCohesiveZoneModelBehaviour::integrate : ");
      msg += "invalid behaviour type (orthotropic type is not supported yet)";
      throw(runtime_error(msg));
    } else {
      string msg("MTestUmatCohesiveZoneModelBehaviour::integrate : ");
      msg += "invalid behaviour type (neither "
	"isotropic or orthotropic)";
      throw(runtime_error(msg));
    }
  }
      
  MTestUmatCohesiveZoneModelBehaviour::~MTestUmatCohesiveZoneModelBehaviour()
  {}
  
} // end of namespace mfront

