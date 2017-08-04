/*! 
 * \file   mfront/mtest/CalculiXFiniteStrainBehaviour.cxx
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
#include<limits>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include"MFront/CalculiX/CalculiX.hxx"
#include"MFront/CalculiX/CalculiXComputeStiffnessTensor.hxx"

#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/UmatNormaliseTangentOperator.hxx"
#include"MTest/CalculiXFiniteStrainBehaviour.hxx"

namespace mtest
{

  CalculiXFiniteStrainBehaviour::CalculiXFiniteStrainBehaviour(const Hypothesis h,
							   const std::string& l,
							   const std::string& b)
    : CalculiXStandardBehaviour(h,l,b)
  {}

  void
  CalculiXFiniteStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    v[0] = v[1] = v[2] = real(1);
    std::fill(v.begin()+3,v.end(),real(0));
  }

  std::pair<bool,real>
  CalculiXFiniteStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
					      CurrentState& s,
					      BehaviourWorkSpace& wk,
					      const real dt,
					      const StiffnessMatrixType ktype,
					      const bool b) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace calculix;
    using tfel::math::vector;
    using TangentOperator = tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    using tfel::material::convert;
    constexpr const auto sqrt2 = Cste<real>::sqrt2;
    const auto h = this->getHypothesis();
#pragma message ("Handle the orthotropic case")
    auto throw_if = [](const bool c, const std::string& m){
      if(c){throw(std::runtime_error("CalculiXSmallStrainBehaviour::"
				     "call_behaviour: "+m));}
    };
    throw_if(ktype!=StiffnessMatrixType::CONSISTENTTANGENTOPERATOR,
	     "CalculiX behaviours only provide the "
	     "consistent tangent operator");
    const CalculiXInt nprops = s.mprops1.empty() ? 1 : static_cast<CalculiXInt>(s.mprops1.size());
    const CalculiXInt ntens = [&h,&throw_if](){
      if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	  (h==ModellingHypothesis::PLANESTRAIN)){
	return 4;
      } else if (h==ModellingHypothesis::PLANESTRESS){
	return 3;
      } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
	return 6;
      } 
      throw_if(true,"unsupported hypothesis");
    }();
    fill(wk.D.begin(),wk.D.end(),0.);
    // using a local copy of material properties to handle the
    // case where s.mprops1 is empty
    copy(s.mprops1.begin(),s.mprops1.end(),wk.mps.begin());
    if(s.mprops1.empty()){
      wk.mps[0] = real(0);
    }
    // using a local copy of internal state variables to handle the
    // case where s.iv0 is empty
    copy(s.iv0.begin(),s.iv0.end(),wk.ivs.begin());
    const auto nstatv = static_cast<CalculiXInt>(wk.ivs.size());
    // rotation matrix, set to identity
    const tmatrix<3u,3u,real> drot = {1,0,0,
				      0,1,0,
				      0,0,1};
    tmatrix<3u,3u,real> uu0(real(0));
    tmatrix<3u,3u,real> uu1(real(0));
    stensor<3u,real> us(real(0));
    uu0(0,0) = s.e0(0); uu1(0,0) = s.e1(0);
    uu0(1,1) = s.e0(1); uu1(1,1) = s.e1(1);
    uu0(2,2) = s.e0(2); uu1(2,2) = s.e1(2);
    if (h==ModellingHypothesis::TRIDIMENSIONAL){
      // uu0 and uu1 must be built using Fortran notations
      uu0(1,0) = s.e0(3); uu1(1,0) = s.e1(3);
      uu0(0,1) = s.e0(4); uu1(0,1) = s.e1(4);
      uu0(2,0) = s.e0(5); uu1(2,0) = s.e1(5);
      uu0(0,2) = s.e0(6); uu1(0,2) = s.e1(6);
      uu0(2,1) = s.e0(7); uu1(2,1) = s.e1(7);
      uu0(1,2) = s.e0(8); uu1(1,2) = s.e1(8);
    } else {
      throw_if(true,"unsupported hypothesis");
    }
    copy(s.s0.begin(),s.s0.end(),us.begin());
    if (h==ModellingHypothesis::PLANESTRESS){
      us[2]  = us[3]/sqrt2;
      us[3]  = 0;
    } else {
      for(CalculiXInt i=3;i!=static_cast<unsigned short>(ntens);++i){
	us(i) /= sqrt2;
      }
    }
    CalculiXReal ndt = std::numeric_limits<CalculiXReal>::max();
    const CalculiXInt KSTEP [3u] = {0,0,1};
    // (this->fct)(&us(0),wk.ivs.size() == 0 ? nullptr : &wk.ivs(0),&(wk.D(0,0)),
    // 		nullptr,nullptr,nullptr,nullptr,
    // 		nullptr,nullptr,nullptr,
    // 		nullptr,nullptr,nullptr,&dt,
    // 		&(s.esv0(0))  ,&(s.desv(0)),
    // 		&(s.esv0(0))+1,&(s.desv(0))+1,
    // 		nullptr,nullptr,nullptr,&ntens,&nstatv,&(wk.mps(0)),
    // 		&nprops,nullptr,&drot(0,0),&ndt,
    // 		nullptr,&uu0(0,0),&uu1(0,0),nullptr,nullptr,
    // 		nullptr,nullptr,KSTEP,nullptr,0);
    if(ndt<1.){
      return {false,ndt};
    }
    // treating the consistent tangent operator
    if(b){
      // treating internal state variables
      if(!s.iv0.empty()){
	copy_n(wk.ivs.begin(),s.iv1.size(),s.iv1.begin());
      }
      // treating stresses
      if (h==ModellingHypothesis::PLANESTRESS){
	us[3] = us[2]*sqrt2;
	us[2] = real(0);
      } else {
	// turning stresses in TFEL conventions
	for(CalculiXInt i=3;i!=static_cast<unsigned short>(ntens);++i){
	  us[i] *= sqrt2;
	}
      }
      copy(us.begin(),us.begin()+s.s1.size(),s.s1.begin());
    }
    if(h==ModellingHypothesis::TRIDIMENSIONAL){
      // st2tost2<3u,CalculiXReal> K;
      // UmatNormaliseTangentOperator::exe(&K(0,0),wk.D,3u);
      // const auto ds = convert<TangentOperator::DSIG_DF,
      // 			      TangentOperator::CALCULIX>(K,tensor<3u,double>(&s.e0[0]),
      // 						       tensor<3u,double>(&s.e1[0]),
      // 						       stensor<3u,double>(&s.s1[0]));
      // std::copy(ds.begin(),ds.end(),Kt.begin());
    } else {
      throw_if(true,"unsupported modelling hypothesis");
    }
    return {true,ndt};
  }

  CalculiXFiniteStrainBehaviour::~CalculiXFiniteStrainBehaviour() = default;
  
} // end of namespace mtest
