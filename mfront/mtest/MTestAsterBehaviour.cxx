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
#include"MFront/Aster/AsterComputeStiffnessTensor.hxx"
#include"MFront/MTestAsterBehaviour.hxx"

namespace mfront
{

  MTestAsterBehaviour::MTestAsterBehaviour(const std::string& l,
					 const std::string& b)
    : MTestUmatBehaviourBase(l,b)
  {
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getAsterFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b);
  }

  bool
  MTestAsterBehaviour::integrate(tfel::math::matrix<real>& Kt,
				tfel::math::tvector<6u,real>& s1,
				tfel::math::vector<real>& iv1,
				const tfel::math::tmatrix<3u,3u,real>& r,
				const tfel::math::tvector<6u,real>& e0,
				const tfel::math::tvector<6u,real>& de,
				const tfel::math::tvector<6u,real>& s0,
				const tfel::math::vector<real>& mp,
				const tfel::math::vector<real>& iv0,
				const tfel::math::vector<real>& ev0,
				const tfel::math::vector<real>& dev,
				const tfel::material::ModellingHypothesis::Hypothesis h,
				const real dt) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace aster;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    static const real sqrt2 = sqrt(real(2));
    AsterInt dimension;
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
      string msg("MTestAsterBehaviour::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    matrix<real> D(Kt.getNbRows(),Kt.getNbCols(),0.);
    vector<real> iv(iv0);
    if(iv.size()==0){
      iv.push_back(0.);
    }
    nstatv = static_cast<AsterInt>(iv.size());
    // rotation matrix
    matrix<real> drot(r.getNbRows(),r.getNbCols(),0.);
    matrix<real>::size_type i,j;
    for(i=0;i!=r.getNbRows();++i){
      for(j=0;j!=r.getNbCols();++j){
	drot(i,j) = r(j,i);
      }
    }
    tvector<6u,real> ue0(e0);
    tvector<6u,real> ude(de);
    copy(s0.begin(),s0.end(),s1.begin());
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i)  /= sqrt2;
      ue0(i) *= sqrt2;
      ude(i) *= sqrt2;
    }
    AsterReal ndt(1.);
    D(0,0)=1.;
    (this->fct)(&s1(0),&iv(0),&D(0,0),
		0,0,0,0,0,0,0,
		&ue0(0),&ude(0),0,&dt,
		&ev0(0),&dev(0),
		&ev0(0)+1,&dev(0)+1,
		0,0,0,&ntens,&nstatv,&mp(0),
		&nprops,0,&drot(0,0),&ndt,
		0,0,0,0,0,0,0,0,0);
    if(!iv1.empty()){
      copy(iv.begin(),iv.end(),iv1.begin());
    }
    // tangent operator (...)
    for(i=0;i!=Kt.getNbRows();++i){
      for(j=0;j!=Kt.getNbCols();++j){
    	Kt(i,j) = D(j,i);
      }
    }
    if(dimension==2u){
      Kt(0,3) *= sqrt2;
      Kt(1,3) *= sqrt2;
      Kt(2,3) *= sqrt2;
      Kt(3,0) *= sqrt2;
      Kt(3,1) *= sqrt2;
      Kt(3,2) *= sqrt2;
      Kt(3,3) *= 2;
    } else if (dimension==3u){
      Kt(0,3) *= sqrt2;
      Kt(1,3) *= sqrt2;
      Kt(2,3) *= sqrt2;
      Kt(0,4) *= sqrt2;
      Kt(1,4) *= sqrt2;
      Kt(2,4) *= sqrt2;
      Kt(0,5) *= sqrt2;
      Kt(1,5) *= sqrt2;
      Kt(2,5) *= sqrt2;
      Kt(3,0) *= sqrt2;
      Kt(3,1) *= sqrt2;
      Kt(3,2) *= sqrt2;
      Kt(4,0) *= sqrt2;
      Kt(4,1) *= sqrt2;
      Kt(4,2) *= sqrt2;
      Kt(5,0) *= sqrt2;
      Kt(5,1) *= sqrt2;
      Kt(5,2) *= sqrt2;
      Kt(3,3) *= 2;
      Kt(3,4) *= 2;
      Kt(3,5) *= 2;
      Kt(4,3) *= 2;
      Kt(4,4) *= 2;
      Kt(4,5) *= 2;
      Kt(5,3) *= 2;
      Kt(5,4) *= 2;
      Kt(5,5) *= 2;
    }
    // turning things in standard conventions
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i) *= sqrt2;
    }
    return true;
  } // end of MTestAsterBehaviour::integrate

  MTestAsterBehaviour::~MTestAsterBehaviour()
  {}
  
} // end of namespace mfront

