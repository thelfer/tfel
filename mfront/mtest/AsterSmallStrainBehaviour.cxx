/*! 
 * \file   mfront/mtest/AsterSmallStrainBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  07 avril 2013
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
#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterComputeStiffnessTensor.hxx"

#include"MTest/UmatNormaliseTangentOperator.hxx"
#include"MTest/AsterSmallStrainBehaviour.hxx"

namespace mfront
{

  AsterSmallStrainBehaviour::AsterSmallStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& l,
								 const std::string& b)
    : AsterStandardBehaviour(h,l,b)
  {}

  void
  AsterSmallStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
  }

  bool
  AsterSmallStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
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
    static const real sqrt2 = sqrt(real(2));
    unsigned short dimension;
    AsterInt ntens;
    AsterInt nprops = mp0.size() == 0 ? 1 : static_cast<AsterInt>(mp0.size());
    AsterInt nstatv;
    AsterInt nummod;
    if (h==MH::AXISYMMETRICAL){
      ntens = 4;
      dimension = 2u;
      nummod = 4u;
    } else if (h==MH::PLANESTRESS){
      ntens = 4;
      dimension = 2u;
      nummod = 5u;
    } else if (h==MH::PLANESTRAIN){
      ntens = 4;
      dimension = 2u;
      nummod = 6u;
    } else if (h==MH::TRIDIMENSIONAL){
      ntens = 6;
      dimension = 3u;
      nummod = 3u;
    } else {
      string msg("AsterSmallStrainBehaviour::call_beahviour : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    fill(this->D.begin(),this->D.end(),0.);
    // choosing the type of stiffness matrix
    UmatBehaviourBase::initializeTangentOperator(ktype,b);
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
      this->ivs[0] = real(0);
    }
    nstatv = static_cast<AsterInt>(this->ivs.size());
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
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i)  /= sqrt2;
      ue0(i) *= sqrt2;
      ude(i) *= sqrt2;
    }
    AsterReal ndt(1.);
    (this->fct)(&s1(0),&(this->ivs(0)),&D(0,0),
		&ue0(0),&ude(0),&dt,
		&ev0(0),&dev(0),
		&ev0(0)+1,&dev(0)+1,
		&ntens,&nstatv,&(this->mps(0)),
		&nprops,&drot(0,0),&ndt,&nummod);
    if(ndt<0.){
      return false;
    }
    if(ktype!=MTestStiffnessMatrixType::NOSTIFFNESS){
      UmatNormaliseTangentOperator::exe(Kt,D,dimension);
    }
    if(b){
      if(!iv0.empty()){
	copy_n(this->ivs.begin(), iv1.size(),iv1.begin());
      }
      // turning things in standard conventions
      for(i=3;i!=static_cast<unsigned short>(ntens);++i){
	s1(i) *= sqrt2;
      }
    }
    return true;
  }

  AsterSmallStrainBehaviour::~AsterSmallStrainBehaviour()
  {}
  
} // end of namespace mfront









