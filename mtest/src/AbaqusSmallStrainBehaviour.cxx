/*! 
 * \file   mfront/mtest/AbaqusSmallStrainBehaviour.cxx
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
#include"MFront/Abaqus/Abaqus.hxx"
#include"MFront/Abaqus/AbaqusComputeStiffnessTensor.hxx"

#include"MTest/UmatNormaliseTangentOperator.hxx"
#include"MTest/AbaqusSmallStrainBehaviour.hxx"

namespace mtest
{

  AbaqusSmallStrainBehaviour::AbaqusSmallStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& l,
								 const std::string& b)
    : AbaqusStandardBehaviour(h,l,b)
  {}

  void
  AbaqusSmallStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    std::fill(v.begin(),v.end(),real(0));
  }

  bool
  AbaqusSmallStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
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
					    const StiffnessMatrixType::mtype ktype,
					    const bool b) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace abaqus;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    static const real sqrt2 = sqrt(real(2));
    if(ktype!=StiffnessMatrixType::CONSISTENTTANGENTOPERATOR){
      throw(std::runtime_error("AbaqusSmallStrainBehaviour::call_beahviour : "
			       "abaqus behaviours may only provide the "
			       "consistent tangent operator"));
    }
    unsigned short dimension;
    AbaqusInt ntens;
    const AbaqusInt nprops = mp0.size() == 0 ? 1 : static_cast<AbaqusInt>(mp0.size());
    if (h==MH::AXISYMMETRICAL){
      ntens  = 4;
      dimension = 2u;
    } else if (h==MH::PLANESTRESS){
      ntens  = 3;
      dimension = 2u;
    } else if (h==MH::TRIDIMENSIONAL){
      ntens = 6;
      dimension = 3u;
    } else {
      throw(std::runtime_error("AbaqusSmallStrainBehaviour::call_beahviour : "
			       "unsupported hypothesis"));
    }
    fill(this->D.begin(),this->D.end(),0.);
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
    const auto nstatv = static_cast<AbaqusInt>(this->ivs.size());
    // rotation matrix, set to identity
    const tmatrix<3u,3u,real> drot = {1,0,0,
				      0,1,0,
				      0,0,1};
    stensor<3u,real> ue0(real(0));
    stensor<3u,real> ude(real(0));
    stensor<3u,real> us(real(0));
    copy(e0.begin(),e0.end(),ue0.begin());
    copy(s0.begin(),s0.end(),us.begin());
    for(AbaqusInt i=0;i!=e1.size();++i){
      ude(i) = e1(i)-e0(i);
    }
    ue0.changeBasis(r);
    ude.changeBasis(r);
    us.changeBasis(r);
    // abaqus standard convention
    for(AbaqusInt i=3;i!=e1.size();++i){
      ue0(i) *= sqrt2;
      ude(i) *= sqrt2;
    }
    if (h==MH::PLANESTRESS){
      us[2] = us[3]/sqrt2;
      us[3] = 0;
    } else {
      for(AbaqusInt i=3;i!=static_cast<unsigned short>(ntens);++i){
	us(i) /= sqrt2;
      }
    }
    AbaqusReal ndt(1.);
    (this->fct)(&us(0),&this->ivs(0),&D(0,0),
		nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,
		&ue0(0),&ude(0),nullptr,&dt,
		&ev0(0),&dev(0),
		&ev0(0)+1,&dev(0)+1,
		nullptr,nullptr,nullptr,&ntens,&nstatv,&mps(0),
		&nprops,nullptr,&drot(0,0),&ndt,
		nullptr,nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,nullptr,0);
    if(ndt<1.){
      return false;
    }
    const auto rb = transpose(r);
    // treatin the consistent tangent operator
    if(h!=MH::PLANESTRESS){
      UmatNormaliseTangentOperator::exe(Kt,D,dimension);
      if(h==MH::TRIDIMENSIONAL){
	st2tost2<3u,AbaqusReal> K;
	for(unsigned short i=0;i!=6u;++i){
	  for(unsigned short j=0;j!=6u;++j){
	    K(i,j)=Kt(i,j);
	  }
	}
	const auto nK = change_basis(K,rb);
	for(unsigned short i=0;i!=6u;++i){
	  for(unsigned short j=0;j!=6u;++j){
	    Kt(i,j)=nK(i,j);
	  }
	}
      } else if (h==MH::AXISYMMETRICAL){
	st2tost2<2u,AbaqusReal> K;
	for(unsigned short i=0;i!=4u;++i){
	  for(unsigned short j=0;j!=4u;++j){
	    K(i,j)=Kt(i,j);
	  }
	}
	const auto nK = change_basis(K,rb);
	for(unsigned short i=0;i!=4u;++i){
	  for(unsigned short j=0;j!=4u;++j){
	    Kt(i,j)=nK(i,j);
	  }
	}
      } else {
	throw(std::runtime_error("AbaqusSmallStrainBehaviour::"
				 "call_behaviour: normalise, "
				 "unsupported modelling hypothesis"));
      }
    } else {
      throw(std::runtime_error("AbaqusSmallStrainBehaviour::"
			       "call_behaviour: normalise, "
			       "tangent operator in plane stress "
			       "not implemented yet"));
    }
    if(b){
      // treating internal state variables
      if(!iv0.empty()){
	copy_n(this->ivs.begin(), iv1.size(),iv1.begin());
      }
      // treating stresses
      if (h==MH::PLANESTRESS){
	us[3] = us[2]*sqrt2;
	us[2] = real(0);
      } else {
	// turning things in standard conventions
	for(AbaqusInt i=3;i!=static_cast<unsigned short>(ntens);++i){
	  us[i] *= sqrt2;
	}
      }
      us.changeBasis(rb);
      copy(us.begin(),us.begin()+s1.size(),s1.begin());
    }
    return true;
  }

  AbaqusSmallStrainBehaviour::~AbaqusSmallStrainBehaviour()
  {}
  
} // end of namespace mtest












