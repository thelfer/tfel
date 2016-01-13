/*! 
 * \file  mfront/mtest/CastemCohesiveZoneModel.cxx
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
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Castem/Castem.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/CastemCohesiveZoneModel.hxx"

namespace mtest
{

  CastemCohesiveZoneModel::CastemCohesiveZoneModel(const tfel::material::ModellingHypothesis::Hypothesis h,
									   const std::string& l,
									   const std::string& b)
    : UmatBehaviourBase(h,l,b)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    const auto& nh = ModellingHypothesis::toString(h);
    auto& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getCastemExternalBehaviourFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b,nh);
    if(this->type!=3u){
      string msg("CastemCohesiveZoneModel::CastemCohesiveZoneModel : ");
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
      string msg("CastemCohesiveZoneModel::CastemCohesiveZoneModel : "
		 "unsupported symmetry type");
      throw(runtime_error(msg));
    }
  }

  tfel::math::tmatrix<3u,3u,real>
  CastemCohesiveZoneModel::getRotationMatrix(const tfel::math::vector<real>&,
					   const tfel::math::tmatrix<3u,3u,real>&) const
  {
    throw(std::runtime_error("CastemCohesiveZoneModel::getRotationMatrix : "
			     "invalid call"));
  } // end of CastemCohesiveZoneModel::getRotationMatrix

  void
  CastemCohesiveZoneModel::allocate(BehaviourWorkSpace& wk,
				    const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    const auto ndv     = this->getDrivingVariablesSize(h);
    const auto nth     = this->getThermodynamicForcesSize(h);
    const auto nstatev = this->getInternalStateVariablesSize(h);
    wk.D.resize(nth,ndv);
    wk.kt.resize(nth,ndv);
    wk.k.resize(nth,ndv);
    wk.ivs.resize(nstatev==0 ? 1u : nstatev,real(0));
    wk.nk.resize(nth,ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    mtest::allocate(wk.cs,*this,h);
  }

  void
  CastemCohesiveZoneModel::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    std::fill(v.begin(),v.end(),real(0));
  } // end of CastemCohesiveZoneModel::setDrivingVariablesDefaultInitialValue  

  StiffnessMatrixType
  CastemCohesiveZoneModel::getDefaultStiffnessMatrixType(void) const
  {
    return StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES;
  }
  
  std::pair<bool,real>
  CastemCohesiveZoneModel::computePredictionOperator(BehaviourWorkSpace& wk,
						     const CurrentState& s,
						     const tfel::material::ModellingHypothesis::Hypothesis h,
						     const StiffnessMatrixType ktype) const
  {
    if(ktype==StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      // rotation matrix
      const auto drot = transpose(s.r);
      this->computeElasticStiffness(wk.k,s.mprops1,drot,h);
      return {true,1};
    }
    throw(std::runtime_error("CastemCohesiveZoneModel::computePredictionOperator : "
			     "computation of the tangent operator "
			     "is not supported"));
  } // end of CastemCohesiveZoneModel::computePredictionOperator

  std::pair<bool,real>
  CastemCohesiveZoneModel::integrate(CurrentState& s,
				     BehaviourWorkSpace& wk,
				     const tfel::material::ModellingHypothesis::Hypothesis h,
				     const real dt,
				     const StiffnessMatrixType ktype) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace castem;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    CastemInt ntens;
    CastemInt ndi;
    CastemInt nprops = static_cast<CastemInt>(s.mprops1.size());
    CastemInt nstatv;
    if((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
       (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)||
       (h==MH::AXISYMMETRICAL)){
      throw(runtime_error("CastemCohesiveZoneModel::integrate: "
			  "unsupported modelling hypothesis"));
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
      throw(runtime_error("CastemCohesiveZoneModel::integrate: "
			  "unsupported hypothesis"));
    }
    if((wk.D.getNbRows()!=wk.k.getNbRows())||
       (wk.D.getNbCols()!=wk.k.getNbCols())){
      throw(runtime_error("CastemCohesiveZoneModel::integrate: "
			  "the memory has not been allocated correctly"));
    }
    if(((s.iv0.size()==0)&&(wk.ivs.size()!=1u))||
       ((s.iv0.size()!=0)&&(s.iv0.size()!=wk.ivs.size()))){
      throw(runtime_error("CastemCohesiveZoneModel::integrate: "
			  "the memory has not been allocated correctly"));
    }
    fill(wk.D.begin(),wk.D.end(),0.);
    if(s.iv0.size()!=0){
      copy(s.iv0.begin(),s.iv0.end(),
	   wk.ivs.begin());
    }
    nstatv = static_cast<CastemInt>(wk.ivs.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot = transpose(s.r);
    CastemInt kinc(1);
    tvector<3u,real> ue0(real(0));
    tvector<3u,real> ude(real(0));
    if(ntens==2){
      ue0[0] = s.e0[1]      ; ue0[1] = s.e0[0];
      ude[0] = s.e1[1]-s.e0[1]; ude[1] = s.e1[0]-s.e0[0];
      s.s1[0]  = s.s0[1]; s.s1[1]  = s.s0[0];
    }
    if(ntens==3){
      ue0[0] = s.e0[1];        ue0[1] = s.e0[2];          ue0[2] = s.e0[0];
      ude[0] = s.e1[1]-s.e0[1]; ude[1] = s.e1[2]-s.e0[2]; ude[2] = s.e1[0]-s.e0[0];
      s.s1[0]  = s.s0[1]; s.s1[1]  = s.s0[2]; s.s1[2]  = s.s0[0];
    }
    CastemReal ndt(1.);
    (this->fct)(&s.s1(0),&wk.ivs(0),&(wk.D(0,0)),
		nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,nullptr,
		&ue0(0),&ude(0),nullptr,&dt,
		&(s.esv0(0))  ,&(s.desv(0)),
		&(s.esv0(0))+1,&(s.desv(0))+1,
		nullptr,&ndi,nullptr,&ntens,&nstatv,&(s.mprops1(0)),
		&nprops,nullptr,&drot(0,0),&ndt,
		nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,nullptr,&kinc,0);
    if(kinc!=1){
      return {false,ndt};
    }
    // tangent operator (...)
    if(ktype!=StiffnessMatrixType::NOSTIFFNESS){ 
      if(ktype==StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
	this->computeElasticStiffness(wk.k,s.mprops1,drot,h);
      } else {
	throw(runtime_error("CastemCohesiveZoneModel::integrate : "
			    "computation of the tangent operator "
			    "is not supported"));
      }
    }
    if(!s.iv1.empty()){
      copy_n(wk.ivs.begin(), s.iv1.size(),s.iv1.begin());
    }
    // turning things in standard conventions
    if(ntens==2){
      swap(s.s1[0],s.s1[1]);
    }
    if(ntens==3){
      const real tmp = s.s1[0];
      s.s1[0] = s.s1[2];
      s.s1[2] = s.s1[1];
      s.s1[1] = tmp;
    }
    return {true,ndt};
  } // end of CastemCohesiveZoneModel::integrate

  void
  CastemCohesiveZoneModel::computeElasticStiffness(tfel::math::matrix<real>& Kt,
							       const tfel::math::vector<real>& mp,
							       const tfel::math::tmatrix<3u,3u,real>&,
							       const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::math;
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
	string msg("CastemCohesiveZoneModel::integrate : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else if(this->stype==1u){
      string msg("CastemCohesiveZoneModel::integrate : ");
      msg += "invalid behaviour type (orthotropic type is not supported yet)";
      throw(runtime_error(msg));
    } else {
      string msg("CastemCohesiveZoneModel::integrate : ");
      msg += "invalid behaviour type (neither "
	"isotropic or orthotropic)";
      throw(runtime_error(msg));
    }
  }

  void
  CastemCohesiveZoneModel::setOptionalMaterialPropertiesDefaultValues(EvolutionManager& mp,
										  const EvolutionManager& evm) const
  {
    using namespace std;
    Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"MassDensity",0.);
    if(this->stype==0){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"NormalThermalExpansion",0.);
    } else {
      string msg("CastemCohesiveZoneModel::CastemCohesiveZoneModel : "
		 "unsupported symmetry type");
      throw(runtime_error(msg));
    }
  } // end of CastemCohesiveZoneModel::setOptionalMaterialPropertiesDefaultValues
      
  CastemCohesiveZoneModel::~CastemCohesiveZoneModel()
  {}
  
} // end of namespace mtest

