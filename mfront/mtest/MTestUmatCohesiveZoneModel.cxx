/*! 
 * \file  mfront/mtest/MTestUmatCohesiveZoneModel.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#include<cmath>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/UMAT/UMAT.hxx"
#include"MFront/MTestUmatCohesiveZoneModel.hxx"

namespace mfront
{

  MTestUmatCohesiveZoneModel::MTestUmatCohesiveZoneModel(const tfel::material::ModellingHypothesis::Hypothesis h,
									   const std::string& l,
									   const std::string& b)
    : MTestUmatBehaviourBase(h,l,b)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    const string& nh = ModellingHypothesis::toString(h);
    ELM& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getUMATFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b,nh);
    if(this->type!=3u){
      string msg("MTestUmatCohesiveZoneModel::MTestUmatCohesiveZoneModel : ");
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
      string msg("MTestUmatCohesiveZoneModel::MTestUmatCohesiveZoneModel : "
		 "unsupported symmetry type");
      throw(runtime_error(msg));
    }
  }

  tfel::math::tmatrix<3u,3u,real>
  MTestUmatCohesiveZoneModel::getRotationMatrix(const tfel::math::vector<real>&,
							 const tfel::math::tmatrix<3u,3u,real>& r) const
  {
    using namespace std;
    string msg("MTestUmatCohesiveZoneModel::getRotationMatrix : "
	       "invalid call");
    throw(runtime_error(msg));
    return r;
  } // end of MTestUmatCohesiveZoneModel::getRotationMatrix

  void
  MTestUmatCohesiveZoneModel::allocate(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    const unsigned short ndv     = this->getDrivingVariablesSize(h);
    const unsigned short nth     = this->getThermodynamicForcesSize(h);
    const unsigned short nstatev = this->getInternalStateVariablesSize(h);
    this->D.resize(nth,ndv);
    this->iv.resize(nstatev);
    if(iv.size()==0){
      iv.push_back(0.);
    }
  }

  void
  MTestUmatCohesiveZoneModel::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
  } // end of MTestUmatCohesiveZoneModel::setDrivingVariablesDefaultInitialValue  

  MTestStiffnessMatrixType::mtype
  MTestUmatCohesiveZoneModel::getDefaultStiffnessMatrixType(void) const
  {
    return MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES;
  }
  
  bool
  MTestUmatCohesiveZoneModel::computePredictionOperator(tfel::math::matrix<real>& Kt,
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
    if(ktype==MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      // rotation matrix
      tmatrix<3u,3u,real> drot(0.);
      tmatrix<3u,3u,real>::size_type i,j;
      for(i=0;i!=r.getNbRows();++i){
	for(j=0;j!=r.getNbCols();++j){
	  drot(i,j) = r(j,i);
	}
      }
      this->computeElasticStiffness(Kt,mp,drot,h);
      return true;
    } else {
      string msg("MTestUmatCohesiveZoneModel::computePredictionOperator : "
		 "computation of the tangent operator "
		 "is not supported");
      throw(runtime_error(msg));
    }
    return false;
  } // end of MTestUmatCohesiveZoneModel::computePredictionOperator

  bool
  MTestUmatCohesiveZoneModel::integrate(tfel::math::matrix<real>& Kt,
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
    using namespace std;
    using namespace tfel::math;
    using namespace umat;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    UMATInt ntens;
    UMATInt ndi;
    UMATInt nprops = static_cast<UMATInt>(mp.size());
    UMATInt nstatv;
    if((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
       (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)||
       (h==MH::AXISYMMETRICAL)){
      string msg(" MTestUmatCohesiveZoneModel::integrate : ");
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
      string msg("MTestUmatCohesiveZoneModel::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    if((this->D.getNbRows()!=Kt.getNbRows())||
       (this->D.getNbCols()!=Kt.getNbCols())){
      string msg("MTestUmatCohesiveZoneModel::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    if(((iv0.size()==0)&&(this->iv.size()!=1u))||
       ((iv0.size()!=0)&&(iv0.size()!=this->iv.size()))){
      string msg("MTestUmatCohesiveZoneModel::integrate : ");
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
      ue0[0] = e0[1]      ; ue0[1] = e0[0];
      ude[0] = e1[1]-e0[1]; ude[1] = e1[0]-e0[0];
      s1[0]  = s0[1]; s1[1]  = s0[0];
    }
    if(ntens==3){
      ue0[0] = e0[1]; ue0[1] = e0[2]; ue0[2] = e0[0];
      ude[0] = e1[1]-e0[1]; ude[1] = e1[2]-e0[2]; ude[2] = e1[0]-e0[0];
      s1[0]  = s0[1]; s1[1]  = s0[2]; s1[2]  = s0[0];
    }
    UMATReal ndt(1.);
    (this->fct)(&s1(0),&iv(0),&D(0,0),
		0,0,0,0,0,0,0,
		&ue0(0),&ude(0),0,&dt,
		&ev0(0),&dev(0),
		&ev0(0)+1,&dev(0)+1,
		0,&ndi,0,&ntens,&nstatv,&mp(0),
		&nprops,0,&drot(0,0),&ndt,
		0,0,0,0,0,0,0,0,&kinc,0);
    if(kinc!=1){
      return false;
    }
    if(!iv1.empty()){
      copy(iv.begin(),iv.end(),iv1.begin());
    }
    // tangent operator (...)
    if(ktype!=MTestStiffnessMatrixType::NOSTIFFNESS){ 
      if(ktype==MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
	this->computeElasticStiffness(Kt,mp,drot,h);
      } else {
	string msg("MTestUmatCohesiveZoneModel::integrate : "
		   "computation of the tangent operator "
		   "is not supported");
	throw(runtime_error(msg));
      }
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
  } // end of MTestUmatCohesiveZoneModel::integrate

  void
  MTestUmatCohesiveZoneModel::computeElasticStiffness(tfel::math::matrix<real>& Kt,
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
	string msg("MTestUmatCohesiveZoneModel::integrate : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else if(this->stype==1u){
      string msg("MTestUmatCohesiveZoneModel::integrate : ");
      msg += "invalid behaviour type (orthotropic type is not supported yet)";
      throw(runtime_error(msg));
    } else {
      string msg("MTestUmatCohesiveZoneModel::integrate : ");
      msg += "invalid behaviour type (neither "
	"isotropic or orthotropic)";
      throw(runtime_error(msg));
    }
  }

  void
  MTestUmatCohesiveZoneModel::setOptionalMaterialPropertiesDefaultValues(MTestEvolutionManager& mp,
										  const MTestEvolutionManager& evm) const
  {
    using namespace std;
    MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"MassDensity",0.);
    if(this->stype==0){
      MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"NormalThermalExpansion",0.);
    } else {
      string msg("MTestUmatCohesiveZoneModel::MTestUmatCohesiveZoneModel : "
		 "unsupported symmetry type");
      throw(runtime_error(msg));
    }
  } // end of MTestUmatCohesiveZoneModel::setOptionalMaterialPropertiesDefaultValues
      
  MTestUmatCohesiveZoneModel::~MTestUmatCohesiveZoneModel()
  {}
  
} // end of namespace mfront

