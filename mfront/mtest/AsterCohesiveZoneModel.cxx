/*! 
 * \file  mfront/mtest/AsterCohesiveZoneModel.cxx
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
#include"MFront/Aster/Aster.hxx"
#include"MTest/AsterCohesiveZoneModel.hxx"

namespace mfront
{

  AsterCohesiveZoneModel::AsterCohesiveZoneModel(const tfel::material::ModellingHypothesis::Hypothesis h,
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
    this->fct = elm.getAsterFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b,nh);
    if(this->type!=3u){
      string msg("AsterCohesiveZoneModel::AsterCohesiveZoneModel : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    if(this->stype==0){
      // this->mpnames.insert(this->mpnames.begin(),"NormalThermalExpansion");
      // this->mpnames.insert(this->mpnames.begin(),"MassDensity");
      // // Those are the conventions used by Cast3M. The ASTERInterface
      // // exchanges the 'NormalStiffness' and the 'TangentialStiffness'
      // // material properties to match MFront conventions
      // this->mpnames.insert(this->mpnames.begin(),"NormalStiffness");
      // this->mpnames.insert(this->mpnames.begin(),"TangentialStiffness");
    } else {
      string msg("AsterCohesiveZoneModel::AsterCohesiveZoneModel : "
		 "unsupported symmetry type");
      throw(runtime_error(msg));
    }
  }

  tfel::math::tmatrix<3u,3u,real>
  AsterCohesiveZoneModel::getRotationMatrix(const tfel::math::vector<real>&,
							 const tfel::math::tmatrix<3u,3u,real>& r) const
  {
    using namespace std;
    string msg("AsterCohesiveZoneModel::getRotationMatrix : "
	       "invalid call");
    throw(runtime_error(msg));
    return r;
  } // end of AsterCohesiveZoneModel::getRotationMatrix

  void
  AsterCohesiveZoneModel::allocate(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    const auto nstatev = this->getInternalStateVariablesSize(h);
    this->mps.resize(this->mpnames.size()==0 ? 1u : this->mpnames.size(),real(0));
    this->ivs.resize(nstatev==0 ? 1u : nstatev,real(0));
  }

  void
  AsterCohesiveZoneModel::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
  } // end of AsterCohesiveZoneModel::setDrivingVariablesDefaultInitialValue  

  MTestStiffnessMatrixType::mtype
  AsterCohesiveZoneModel::getDefaultStiffnessMatrixType(void) const
  {
    return MTestStiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }
  
  bool
  AsterCohesiveZoneModel::computePredictionOperator(tfel::math::matrix<real>& Kt,
						    const tfel::math::tmatrix<3u,3u,real>& r,
						    const tfel::math::vector<real>& u0,
						    const tfel::math::vector<real>& s0,
						    const tfel::math::vector<real>& mprops0,
						    const tfel::math::vector<real>& iv0,
						    const tfel::math::vector<real>& esv0,
						    const tfel::material::ModellingHypothesis::Hypothesis h,
						    const MTestStiffnessMatrixType::mtype ktype) const
  {
    using namespace tfel::math;
    vector<real> s1(s0);
    vector<real> u1(u0);
    vector<real> iv1(iv0);
    vector<real> desv(esv0.size(),real(0));
    return this->call_behaviour(Kt,s1,iv1,r,u0,u1,s0,
				mprops0,iv0,esv0,desv,
				h,real(1),ktype,false);
  } // end of AsterCohesiveZoneModel::computePredictionOperator

  bool
  AsterCohesiveZoneModel::integrate(tfel::math::matrix<real>& Kt,
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
  } // end of AsterCohesiveZoneModel::integrate

  bool
  AsterCohesiveZoneModel::call_behaviour(tfel::math::matrix<real>& Kt,
					 tfel::math::vector<real>& s1,
					 tfel::math::vector<real>& iv1,
					 const tfel::math::tmatrix<3u,3u,real>& r,
					 const tfel::math::vector<real>& e0,
					 const tfel::math::vector<real>& e1,
					 const tfel::math::vector<real>& s0,
					 const tfel::math::vector<real>& mp0,
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
    AsterInt ntens;
    AsterInt nprops = mp0.size() == 0 ? 1 : static_cast<AsterInt>(mp0.size());
    AsterInt nstatv;
    AsterInt nummod;
    if (h==MH::AXISYMMETRICAL){
      ntens = 4;
      nummod = 4u;
    } else if (h==MH::PLANESTRESS){
      ntens = 4;
      nummod = 5u;
    } else if (h==MH::PLANESTRAIN){
      ntens = 4;
      nummod = 6u;
    } else if (h==MH::TRIDIMENSIONAL){
      ntens = 6;
      nummod = 3u;
    } else {
      string msg("AsterCohesiveZoneModel::call_beahviour : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    fill(Kt.begin(),Kt.end(),0.);
    // choosing the type of stiffness matrix
    if(b){
      if(ktype==MTestStiffnessMatrixType::NOSTIFFNESS){
	// do nothing
      } else if(ktype==MTestStiffnessMatrixType::ELASTIC){
	Kt(0,0) = real(1);
      } else if(ktype==MTestStiffnessMatrixType::SECANTOPERATOR){
	Kt(0,0) = real(2);
      } else if(ktype==MTestStiffnessMatrixType::TANGENTOPERATOR){
	Kt(0,0) = real(3);
      } else if(ktype==MTestStiffnessMatrixType::CONSISTENTTANGENTOPERATOR){
	Kt(0,0) = real(4);
      } else {
	string msg("AsterCohesiveZoneModel::call_behaviour : "
		   "invalid or unspecified stiffness matrix type");
	throw(runtime_error(msg));
      }
    } else {
      if(ktype==MTestStiffnessMatrixType::ELASTIC){
	Kt(0,0) = real(-1);
      } else if(ktype==MTestStiffnessMatrixType::SECANTOPERATOR){
	Kt(0,0) = real(-2);
      } else if(ktype==MTestStiffnessMatrixType::TANGENTOPERATOR){
	Kt(0,0) = real(-3);
      } else {
	string msg("AsterCohesiveZoneModel::call_behaviour : "
		   "invalid or unspecified stiffness matrix type");
	throw(runtime_error(msg));
      }
    }
    // using a local copy of material properties to handle the
    // case where mp0 is empty
    copy(mp0.begin(),mp0.end(),this->mps.begin());
    if(mp0.empty()){
      this->mps[0] = real(0);
    }
    // using a local copy of internal state variables to handle the
    // case where iv0 is empty
    copy(iv0.begin(),iv0.end(),this->ivs.begin());
    if(iv0.empty()){
      ivs[0] = real(0);
    }
    nstatv = static_cast<AsterInt>(ivs.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot;
    tmatrix<3u,3u,real>::size_type i,j;
    for(i=0;i!=3u;++i){
      for(j=0;j!=3u;++j){
	drot(i,j) = r(j,i);
      }
    }
    tvector<3u,real> ue0(real(0));
    tvector<3u,real> ude(real(0));
    copy(e0.begin(),e0.end(),ue0.begin());
    for(i=0;i!=e1.size();++i){
      ude(i) = e1(i)-e0(i);
    }
    copy(s0.begin(),s0.end(),s1.begin());
    AsterReal ndt(1.);
    (this->fct)(&s1(0),&ivs(0),&Kt(0,0),
		&ue0(0),&ude(0),&dt,
		&ev0(0),&dev(0),
		&ev0(0)+1,&dev(0)+1,
		&ntens,&nstatv,&(this->mps(0)),
		&nprops,&drot(0,0),&ndt,&nummod);
    if(ndt<0.){
      return false;
    }
    if(b){
      if(!iv0.empty()){
	copy_n(this->ivs.begin(), iv1.size(),iv1.begin());
      }
    }
    return true;
  }

  void
  AsterCohesiveZoneModel::setOptionalMaterialPropertiesDefaultValues(EvolutionManager&,
										  const EvolutionManager& ) const
  {} // end of AsterCohesiveZoneModel::setOptionalMaterialPropertiesDefaultValues
      
  AsterCohesiveZoneModel::~AsterCohesiveZoneModel()
  {}
  
} // end of namespace mfront

